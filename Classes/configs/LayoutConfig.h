#pragma once
#include "cocos2d.h"

/** Stateless design-space layout constants for game UI. */
class LayoutConfig {
  public:
  static cocos2d::Size contentSize();

  /** Returns the visual card image size. */
  static cocos2d::Size cardSize();

  /** Returns the tray/top card center position. */
  static cocos2d::Vec2 trayPosition();

  /** Returns the reserve pile center position. */
  static cocos2d::Vec2 reservePilePosition();

  /** Returns the undo button center position. */
  static cocos2d::Vec2 undoButtonPosition();

  /** Returns the standard card movement duration. */
  static float cardMoveDuration();

  static cocos2d::Color4B backgroundColor();

  static float playfieldOffsetFromBottom();
};
