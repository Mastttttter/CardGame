#pragma once

#include "CardTypes.h"
#include "cocos2d.h"

/** Static card data loaded from a level config file. */
struct CardConfig {
  /** Card behavior type declared by the level data. */
  CardType type = CardType::Default;

  /** Card face where 0 is ace and 12 is king. */
  int face = 0;

  /** Card suit where 0 club, 1 diamond, 2 heart, and 3 spade. */
  int suit = 0;

  /** Design-space center position from the level config. */
  cocos2d::Vec2 position;

  /** Original playfield order, or -1 for stack cards. */
  int playfieldOrder = -1;
};
