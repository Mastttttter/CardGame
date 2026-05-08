#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardResConfig.h"
#include "configs/models/CardTypes.h"

GameController::GameController(GameView *view) : _view(view), _started(false) {}

GameController::~GameController() {}

bool GameController::start() {
  if (!_view) {
    return false;
  }
  registerCardController();
  if (!initGameModel()) {
    CCLOG("Failed to init game model!");
    return false;
  }

  // TODO set click callback
  // TODO set up card views
  _started = true;
  return true;
}

void GameController::registerCardController() {}

bool GameController::initGameModel() {
  auto level =
      LevelConfigLoader::load(CardResConfig::getLevelConfigPath().c_str());
  if (!level) {
    return false;
  }
  auto model = std::make_shared<GameModel>();
  int index = 0;
  for (auto cardPtr: level->playfieldCards) {
    if (_cardTypeControllers.find(cardPtr->type) ==
        _cardTypeControllers.end()) {
      CCLOG("Failed load form config missing type controller %d",
            cardPtr->type);
      return false;
    }
    model->addCard(
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
    model->addCard(
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
