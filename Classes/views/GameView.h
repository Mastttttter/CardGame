#pragma once

#include <functional>
#include <unordered_map>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"

/** Root Cocos UI node for cards, reserve pile, and undo control. */
class GameView : public cocos2d::Node {
  public:
  /** Creates a Cocos autoreleased GameView. */
  CREATE_FUNC(GameView);

  /** Initializes the root game UI. */
  bool init() override;

  private:
};
