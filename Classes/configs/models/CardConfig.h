#pragma once

#include <cocos2d.h>
#include "configs/models/CardTypes.h"

/**
 * @brief store card data
 */
class CardConfig {
  public:
  CardConfig()
      : face(CFT_NONE),
        suit(CST_NONE),
        position(cocos2d::Vec2::ZERO) {}

  // face type of a card
  CardFaceType face;
  // suit type of a card
  CardSuitType suit;
  // the position of a card
  cocos2d::Vec2 position;
};
