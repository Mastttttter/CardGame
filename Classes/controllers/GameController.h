#pragma once
#include <map>
#include <memory>
#include <unordered_map>
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardTypes.h"
#include "views/GameView.h"

/** Wires model, views, managers, services, and card controllers. */
class GameController {
  public:
  /** Creates a controller for an existing GameView. */
  explicit GameController(GameView *view);

  /** Destroys owned runtime game objects. */
  ~GameController();

  /** Loads the initial level and binds view callbacks. */
  bool start();

  private:
};
