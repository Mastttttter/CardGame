#include "CardViewDefault.h"
#include <memory>
#include "base/CCDirector.h"
#include "models/CardModelDefault.h"
#include "utils/CardAssetUtils.h"

void CardViewDefault::configure(std::shared_ptr<CardModelBase> card) {
  Super::configure(card);
  auto cardDefault = std::dynamic_pointer_cast<CardModelDefault>(card);
  if (!cardDefault) {
    CCLOG("fatal: wrong type match in CardViewDefault");
    exit(1);
  }
  cocos2d::Sprite *background =
      cocos2d::Sprite::create(CardAssetUtils::cardBackgroundPath());
  if (background) {
    background->setPosition(cocos2d::Vec2::ZERO);
    addChild(background);
  }
  cocos2d::Sprite *smallRank =
      cocos2d::Sprite::create(CardAssetUtils::smallRankPath(
          cardDefault->getFace(), cardDefault->getSuit()));
  if (smallRank) {
    smallRank->setPosition(cocos2d::Vec2(-58.0f, 104.0f));
    addChild(smallRank);
  }
  cocos2d::Sprite *bigRank =
      cocos2d::Sprite::create(CardAssetUtils::bigRankPath(
          cardDefault->getFace(), cardDefault->getSuit()));
  if (bigRank) {
    bigRank->setPosition(cocos2d::Vec2(0.0f, 18.0f));
    addChild(bigRank);
  }
  cocos2d::Sprite *suit =
      cocos2d::Sprite::create(CardAssetUtils::suitPath(cardDefault->getSuit()));
  if (suit) {
    suit->setPosition(cocos2d::Vec2(54.0f, -100.0f));
    addChild(suit);
  }
}
