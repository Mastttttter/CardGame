#pragma once

#include "CardTypes.h"
#include "cocos2d.h"

/** Static card data loaded from a level config file. */
class CardConfigBase {
  public:
  virtual ~CardConfigBase() = default;
  /** Card behavior type declared by the level data. */
  CardType type = CardType::Default;

  /** Design-space center position from the level config. */
  cocos2d::Vec2 position;

  /** Original playfield order, or -1 for stack cards. */
  int playfieldOrder = -1;
};
