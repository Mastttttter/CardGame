#include "GameController.h"
#include "CardControllerDefault.h"
#include "configs/LayoutConfig.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardResConfig.h"
#include "configs/models/CardTypes.h"
#include "editor-support/cocosbuilder/CCControlLoader.h"
#include "utils/GeometryUtils.h"

GameController::GameController(GameView *view) : _view(view), _started(false) {
  _view->setGameController(this);
}

GameController::~GameController() {}

bool GameController::start() {
  if (!_view) {
    return false;
  }
  registerCardController();
  CCLOG("Register Card Controller Success");
  if (!initGameModel() && _model) {
    CCLOG("Failed to init game model!");
    return false;
  }
  CCLOG("Success: initial game model");
  postInitGameModel();

  _view->setCardClickCallback(
      [this](CardId cardId) { handleCardClick(cardId); });
  _view->setReserveClickCallback([this]() { handleReserveClick(); });
  _view->setUndoClickCallback([this]() { handleUndoClick(); });
  CCLOG("Success: set up call backs");
  _view->setup(_model);
  CCLOG("Success: set up views");
  _started = true;
  return true;
}

void GameController::registerCardController() {
  _cardTypeControllers[CardType::Default] =
      std::make_shared<CardControllerDefault>(this);
}

std::shared_ptr<CardControllerBase> GameController::getCardControllerOfId(
    CardId id) {
  auto card = _model->findCard(id);
  if (!card) {
    CCLOG("Fatal: could not get card for id: %d", id);
    return nullptr;
  }
  return getCardControllerOfType(card->getType());
};

std::shared_ptr<CardControllerBase> GameController::getCardControllerOfType(
    CardType type) {
  auto controller = _cardTypeControllers.find(type);
  if (controller == _cardTypeControllers.end()) {
    CCLOG("Fatal: could not get controller for type: %d", type);
    return nullptr;
  }
  return _cardTypeControllers[type];
};

bool GameController::initGameModel() {
  auto level =
      LevelConfigLoader::load(CardResConfig::getLevelConfigPath().c_str());
  if (!level) {
    return false;
  }
  _model = std::make_shared<GameModel>();
  int index = 0;
  for (auto cardPtr: level->playfieldCards) {
    if (_cardTypeControllers.find(cardPtr->type) ==
        _cardTypeControllers.end()) {
      CCLOG("Failed load form config missing type controller %d",
            cardPtr->type);
      return false;
    }
    _model->addCard(
        _cardTypeControllers[cardPtr->type]->generateCardModelFromConfig(
            cardPtr),
        CardZone::Playfield, index++);
  }
  for (auto cardPtr: level->stackCards) {
    if (_cardTypeControllers.find(cardPtr->type) ==
        _cardTypeControllers.end()) {
      CCLOG("Failed load form config missing type controller %d",
            cardPtr->type);
      return false;
    }
    _model->addCard(
        _cardTypeControllers[cardPtr->type]->generateCardModelFromConfig(
            cardPtr),
        index == level->playfieldCards.size() + level->stackCards.size() - 1
            ? CardZone::Tray
            : CardZone::Reserve,
        index);
    index++;
  }
  return true;
}

void GameController::handleReserveClick() {
  // TODO handleReserveClck
}

void GameController::handleUndoClick() {
  // TODO  handleUndoclic
}

void GameController::handleCardClick(CardId cardId) {
  if (!_started || !_model) {
    return;
  }
  getCardControllerOfId(cardId)->handleCardClick(cardId);

  auto card = _model->findCard(cardId);
  if (!card) {
    return;
  }

  auto controller = _cardTypeControllers.find(card->getType());
  if (controller == _cardTypeControllers.end() || !controller->second) {
    return;
  }

  controller->second->handleCardClick(cardId);

  // TODO undo logic in game controller
}

void GameController::postInitGameModel() {
  auto cards = _model->getPlayfieldCardIds();
  for (int i = 0; i < cards.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      auto cardA = _model->findCard(cards[i]);
      auto cardB = _model->findCard(cards[j]);
      auto rectA = GeometryUtils::centeredRect(cardA->getPosition(),
                                               LayoutConfig::cardSize());
      auto rectB = GeometryUtils::centeredRect(cardB->getPosition(),
                                               LayoutConfig::cardSize());
      if (GeometryUtils::overlapsWithPositiveArea(rectA, rectB)) {
        if (cardA->getPlayfieldOrder() > cardB->getPlayfieldOrder()) {
          _cardManager.addConnection(cards[i], cards[j]);
        } else {
          _cardManager.addConnection(cards[j], cards[i]);
        }
      }
    }
  }
  _cardManager.constructGraph();
  CCLOG("Success: construct card graph");
}

void GameController::refreshView() {
  if (!_view || !_model) {
    return;
  }

  _view->refresh(_model, buildClickability(), _undoManager.canUndo());
}

std::unordered_map<CardId, bool> GameController::buildClickability() {
  std::unordered_map<CardId, bool> clickability;
  if (!_model) {
    return clickability;
  }
  auto const &playfieldCards = _model->getPlayfieldCardIds();
  for (auto &cardId: playfieldCards) {
    auto card = _model->findCard(cardId);
    if (!card || card->getZone() != CardZone::Playfield) {
      clickability[cardId] = false;
      continue;
    }
    clickability[cardId] =
        getCardManager().isOnTop(cardId) &&
        _cardTypeControllers[card->getType()]->checkIfClickable(cardId);
  }
  return clickability;
}
