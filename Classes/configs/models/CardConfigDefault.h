#pragma once

#include "CardConfigBase.h"
#include "CardTypes.h"
#include "cocos2d.h"

/**
 * @brief Static configuration for a standard suit-and-rank playing card.
 */
struct CardConfigDefault : public CardConfigBase {
  /** @brief Card behavior type declared by the level data. */
  CardType type = CardType::Default;

  /** @brief Card face where 0 is ace and 12 is king. */
  int face = 0;

  /** @brief Card suit where 0 is club, 1 is diamond, 2 is heart, and 3 is spade. */
  int suit = 0;
};
