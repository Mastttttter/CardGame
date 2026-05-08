#pragma once
#include <memory>
#include "base/CCDirector.h"
#include "configs/LayoutConfig.h"
#include "configs/models/CardConfigBase.h"
#include "configs/models/CardConfigDefault.h"
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"

/**
 * @brief Runtime state for a standard suit-and-rank playing card.
 */
class CardModelDefault : public CardModelBase {
  public:
  /**
   * @brief Creates a default-card model and offsets its playfield position.
   *
   * @param config Static default-card configuration loaded from the level file.
   */
  CardModelDefault(std::shared_ptr<CardConfigBase> config) : Super(config) {
    setOriginalPosition(cocos2d::Vec2(
        config->position.x,
        config->position.y + LayoutConfig::playfieldOffsetFromBottom()));
    setPosition(getOriginalPosition());
  };

  /**
   * @brief Returns the default-card face value.
   *
   * @return Face value where 0 is ace and 12 is king.
   */
  int getFace() const {
    auto configDefault =
        std::dynamic_pointer_cast<CardConfigDefault>(getConfig());
    if (!configDefault) {
      CCLOG("wrong type match in CardModleDefault");
      cocos2d::Director::getInstance()->end();
      exit(1);
    }
    return configDefault->face;
  }

  /**
   * @brief Returns the default-card suit value.
   *
   * @return Suit value where 0 is club, 1 is diamond, 2 is heart, and 3 is spade.
   */
  int getSuit() const {
    auto configDefault =
        std::dynamic_pointer_cast<CardConfigDefault>(getConfig());
    if (!configDefault) {
      CCLOG("wrong type match in CardModleDefault");
      cocos2d::Director::getInstance()->end();
      exit(1);
    }
    return configDefault->suit;
  }
};
