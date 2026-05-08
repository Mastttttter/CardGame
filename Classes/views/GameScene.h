#pragma once

#include <memory>
#include "cocos2d.h"

class GameController;
class GameView;

/**
 * @brief Scene entry point used by AppDelegate to start the card game.
 */
class GameScene : public cocos2d::Scene {
  public:
  /**
   * @brief Creates the initial game scene.
   *
   * @return Autoreleased Cocos scene pointer.
   */
  static cocos2d::Scene *createScene();

  /**
   * @brief Creates an empty scene instance before Cocos initialization.
   */
  GameScene();

  /**
   * @brief Destroys controller-owned game state.
   */
  ~GameScene() override;

  /**
   * @brief Initializes the scene, view, and controller.
   *
   * @return True when the Cocos scene initializes successfully.
   */
  bool init() override;

  /**
   * @brief Creates an autoreleased GameScene instance.
   *
   * @return Autoreleased GameScene pointer, or nullptr when initialization fails.
   */
  CREATE_FUNC(GameScene);

  private:
  /** @brief Root game view node owned by the scene graph. */
  GameView *_gameView;

  /** @brief Game controller owned by the scene. */
  std::unique_ptr<GameController> _controller;
};
