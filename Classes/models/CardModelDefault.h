#pragma once
#include <memory>
#include "base/CCDirector.h"
#include "configs/models/CardConfigBase.h"
#include "configs/models/CardConfigDefault.h"
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"

/** runtime card state.*/
class CardModelDefault : public CardModelBase {
  public:
  CardModelDefault(std::shared_ptr<CardConfigBase> config) : Super(config) {};

  /** Returns the default-card face value. */
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

  /** Returns the default-card suit value. */
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
