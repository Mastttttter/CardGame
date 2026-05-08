#pragma once

#include "CardConfigBase.h"
#include "CardTypes.h"
#include "cocos2d.h"

/** Static card data loaded from a level config file. */
class CardConfigDefault : public CardConfigBase {
  public:
  /** Card behavior type declared by the level data. */
  CardType type = CardType::Default;

  /** Card face where 0 is ace and 12 is king. */
  int face = 0;

  /** Card suit where 0 club, 1 diamond, 2 heart, and 3 spade. */
  int suit = 0;
};
