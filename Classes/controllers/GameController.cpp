#include "GameController.h"
#include "CardControllerDefault.h"
#include "CCPlatformMacros.h"
#include "configs/LayoutConfig.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardResConfig.h"
#include "configs/models/CardTypes.h"
#include "controllers/StackController.h"
#include "editor-support/cocosbuilder/CCControlLoader.h"
#include "utils/GeometryUtils.h"

GameController::GameController(GameView *view) : _view(view), _started(false) {
  _view->setGameController(this);
  _model = std::make_shared<GameModel>();
}

GameController::~GameController() {}

bool GameController::start() {
  if (!_view) {
    return false;
  }
  _stackController.reset(new StackController(_model.get()));
  registerCardController();
  CCLOG("Register Card Controller Success");
  if (!initGameModel() && _model) {
    CCLOG("Failed to init game model!");
    return false;
  }
  CCLOG("Success: initial game model");
  postInitGameModel();
  CCLOG("Success: post initial game model");

  _view->setCardClickCallback(
      [this](CardId cardId) { _handleCardClick(cardId); });
  _view->setReserveClickCallback([this]() { _handleReserveClickk(); });
  _view->setUndoClickCallback([this]() { _handleUndoClick(); });
  CCLOG("Success: set up call backs");
  _view->setup(_model);
  CCLOG("Success: set up views");
  _started = true;
  _refreshView();
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
  return controller->second;
};

bool GameController::initGameModel() {
  auto level =
      LevelConfigLoader::load(CardResConfig::getLevelConfigPath().c_str());
  if (!level) {
    return false;
  }
  int index = 0;
  for (auto cardPtr: level->playfieldCards) {
    if (_cardTypeControllers.find(cardPtr->type) ==
        _cardTypeControllers.end()) {
      CCLOG("Failed load form config missing type controller %d",
            cardPtr->type);
      return false;
    }
    auto curCardId = _model->addCard(
        _cardTypeControllers[cardPtr->type]->generateCardModelFromConfig(
            cardPtr),
        CardZone::Playfield, index++);
    _model->addPlayfieldCardId(curCardId);
  }
  for (auto cardPtr: level->stackCards) {
    if (_cardTypeControllers.find(cardPtr->type) ==
        _cardTypeControllers.end()) {
      CCLOG("Failed load form config missing type controller %d",
            cardPtr->type);
      return false;
    }
    auto curCardId = _model->addCard(
        _cardTypeControllers[cardPtr->type]->generateCardModelFromConfig(
            cardPtr),
        index == level->playfieldCards.size() + level->stackCards.size() - 1
            ? CardZone::Tray
            : CardZone::Reserve,
        index);
    auto curCard = _model->findCard(curCardId);
    if (index == level->playfieldCards.size() + level->stackCards.size() - 1) {
      _model->setTrayCardId(curCardId);
      curCard->setPosition(LayoutConfig::trayPosition());
    } else {
      _model->addReserveCardId(curCardId);
      curCard->setPosition(LayoutConfig::reservePilePosition());
    }
    index++;
  }
  return true;
}

void GameController::_handleReserveClickk() {
  if (!_started) {
    return;
  }

  CardId drawnCardId = INVALID_CARD_ID;
  std::unique_ptr<UndoOperation> operation =
      _stackController->drawReserveCard(&drawnCardId);
  if (!operation || drawnCardId == INVALID_CARD_ID) {
    _refreshView();
    return;
  }

  _undoManager.push(std::move(operation));
  _view->setInputEnabled(false);
  _view->showCardAtPosition(drawnCardId, LayoutConfig::reservePilePosition());
  _view->animateCardToPosition(drawnCardId, LayoutConfig::trayPosition(),
                               [this]() {
                                 _view->setInputEnabled(true);
                                 _refreshView();
                               });
}

void GameController::_handleUndoClick() {
  if (!_started) {
    return;
  }

  UndoAnimation animation;
  if (!_undoManager.undo(&animation)) {
    _refreshView();
    return;
  }

  if (animation.cardId != INVALID_CARD_ID) {
    _view->setInputEnabled(false);
    _view->animateCardToPosition(animation.cardId, animation.targetPosition,
                                 [this]() {
                                   _view->setInputEnabled(true);
                                   _refreshView();
                                 });
    return;
  }

  _refreshView();
}

void GameController::_handleCardClick(CardId cardId) {
  CCLOG("info: receive cardClick");
  if (!_started || !_model) {
    return;
  }

  auto card = _model->findCard(cardId);
  if (!card) {
    return;
  }

  auto controller = getCardControllerOfId(cardId);
  if (!controller) {
    return;
  }
  controller->_handleCardClick(cardId);
  if (_cardManager.isOnTop(cardId) && controller->checkIfClickable(cardId)) {
    auto operation = controller->doCardAction(cardId);
    if (!operation) {
      CCLOG("warning: invalid operation");
      _refreshView();
      return;
    }
    CCLOG("info: create an operation");
    _undoManager.push(std::move(operation));

    _undoManager.push(std::move(operation));
    _view->setInputEnabled(false);
    _view->animateCardToPosition(cardId, LayoutConfig::trayPosition(),
                                 [this]() {
                                   _view->setInputEnabled(true);
                                   _refreshView();
                                 });
  }
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

void GameController::_refreshView() {
  if (!_view || !_model) {
    return;
  }

  _view->refresh(_model, _buildClickability(), _undoManager.canUndo());
}

std::unordered_map<CardId, bool> GameController::_buildClickability() {
  std::unordered_map<CardId, bool> clickability;
  if (!_model) {
    return clickability;
  }
  auto const &playfieldCards = _model->getPlayfieldCardIds();
  if (playfieldCards.empty()) {
    CCLOG("error:no playfield cards");
  }
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
