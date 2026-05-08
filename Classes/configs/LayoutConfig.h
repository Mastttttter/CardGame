#pragma once
#include "cocos2d.h"

/**
 * @brief Provides design-space layout constants for the game UI.
 */
class LayoutConfig {
  public:
  /**
   * @brief Returns the root content size used by the game view.
   *
   * @return Size of the design-space game area.
   */
  static cocos2d::Size contentSize();

  /**
   * @brief Returns the visual card image size.
   *
   * @return Size used for card nodes and touch bounds.
   */
  static cocos2d::Size cardSize();

  /**
   * @brief Returns the tray/top-card center position.
   *
   * @return Design-space center position of the tray card.
   */
  static cocos2d::Vec2 trayPosition();

  /**
   * @brief Returns the reserve pile center position.
   *
   * @return Design-space center position of the reserve pile.
   */
  static cocos2d::Vec2 reservePilePosition();

  /**
   * @brief Returns the undo button center position.
   *
   * @return Design-space center position of the undo button.
   */
  static cocos2d::Vec2 undoButtonPosition();

  /**
   * @brief Returns the standard card movement duration.
   *
   * @return Duration in seconds for simple card move animations.
   */
  static float cardMoveDuration();

  /**
   * @brief Returns the game background color.
   *
   * @return RGBA color used by the root game view background.
   */
  static cocos2d::Color4B backgroundColor();

  /**
   * @brief Returns the vertical offset applied to playfield cards.
   *
   * @return Design-space offset from the bottom of the playfield area.
   */
  static float playfieldOffsetFromBottom();
};
