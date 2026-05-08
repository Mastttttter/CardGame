#pragma once

#include <memory>
#include "cocos2d.h"

class GameController;
class GameView;

/** Scene entry point used by AppDelegate to start the card game. */
class GameScene : public cocos2d::Scene {
  public:
  /** Creates the initial game scene. */
  static cocos2d::Scene *createScene();

  /** Creates an empty scene instance. */
  GameScene();

  /** Destroys controller-owned game state. */
  ~GameScene() override;

  /** Initializes the scene, view, and controller. */
  bool init() override;

  /** Creates a Cocos autoreleased GameScene. */
  CREATE_FUNC(GameScene);

  private:
  GameView *_gameView;
  std::unique_ptr<GameController> _controller;
};
