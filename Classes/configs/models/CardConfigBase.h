#pragma once

#include "CardTypes.h"
#include "cocos2d.h"

/**
 * @brief Base static card data loaded from a level configuration file.
 */
class CardConfigBase {
  public:
  /**
   * @brief Destroys static card configuration through the base type.
   */
  virtual ~CardConfigBase() = default;

  /** @brief Card behavior type declared by the level data. */
  CardType type = CardType::Default;

  /** @brief Design-space center position from the level configuration. */
  cocos2d::Vec2 position;

  /** @brief Original playfield order, or -1 for stack cards. */
  int playfieldOrder = -1;
};
