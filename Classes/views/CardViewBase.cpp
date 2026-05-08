#include "views/CardViewBase.h"
#include "configs/LayoutConfig.h"

bool CardViewBase::init() {
  if (!cocos2d::Node::init()) {
    return false;
  }

  setContentSize(LayoutConfig::cardSize());
  setCascadeOpacityEnabled(true);
  _addTouchListener();
  return true;
}

void CardViewBase::configure(std::shared_ptr<CardModelBase> card) {
  _cardId = card->getId();
  removeAllChildren();
}

void CardViewBase::setClickable(bool clickable) {
  _clickable = clickable;
  setOpacity(clickable ? 255 : 175);
}

void CardViewBase::setClickCallback(
    std::function<void(CardId)> const &callback) {
  _clickCallback = callback;
}

CardId CardViewBase::getCardId() const {
  return _cardId;
}

bool CardViewBase::_containsTouch(cocos2d::Touch *touch) const {
  cocos2d::Vec2 const local = convertToNodeSpace(touch->getLocation());
  cocos2d::Size const size = LayoutConfig::cardSize();
  cocos2d::Rect const rect(-size.width * 0.5f, -size.height * 0.5f, size.width,
                           size.height);
  return rect.containsPoint(local);
}

void CardViewBase::_addTouchListener() {
  cocos2d::EventListenerTouchOneByOne *listener =
      cocos2d::EventListenerTouchOneByOne::create();
  listener->setSwallowTouches(true);
  listener->onTouchBegan = [this](cocos2d::Touch *touch,
                                  cocos2d::Event *event) {
    return _clickable && _containsTouch(touch);
  };
  listener->onTouchEnded = [this](cocos2d::Touch *touch,
                                  cocos2d::Event *event) {
    if (_clickable && _clickCallback && _containsTouch(touch)) {
      _clickCallback(_cardId);
    }
  };
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
