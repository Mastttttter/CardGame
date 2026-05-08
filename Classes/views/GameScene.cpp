#include "views/GameScene.h"
#include "controllers/GameController.h"
#include "views/GameView.h"

cocos2d::Scene *GameScene::createScene() {
  return GameScene::create();
}

GameScene::GameScene() : _gameView(nullptr) {}

GameScene::~GameScene() {}

bool GameScene::init() {
  if (!cocos2d::Scene::init()) {
    return false;
  }
  _gameView = GameView::create();
  if (!_gameView) {
    return false;
  }
  addChild(_gameView);
  _controller.reset(new GameController(_gameView));
  if (!_controller->start()) {
    CCLOG("GameScene failed to start GameController.");
  }
  return true;
}
