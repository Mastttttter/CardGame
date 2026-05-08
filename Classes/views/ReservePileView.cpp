#include "views/ReservePileView.h"
#include <sstream>
#include "configs/LayoutConfig.h"
#include "utils/CardAssetUtils.h"

bool ReservePileView::init() {
  if (!cocos2d::Node::init()) {
    return false;
  }
  setContentSize(LayoutConfig::cardSize());
  setCascadeOpacityEnabled(true);
  cocos2d::Sprite *card =
      cocos2d::Sprite::create(CardAssetUtils::cardBackgroundPath());
  if (card) {
    card->setColor(cocos2d::Color3B(130, 130, 130));
    card->setPosition(cocos2d::Vec2::ZERO);
    addChild(card);
  }
  cocos2d::Label *title =
      cocos2d::Label::createWithTTF("Reserve", "fonts/arial.ttf", 28.0f);
  if (title) {
    title->setColor(cocos2d::Color3B::BLACK);
    title->setPosition(cocos2d::Vec2(0.0f, 42.0f));
    addChild(title);
  }
  _countLabel = cocos2d::Label::createWithTTF("0", "fonts/arial.ttf", 48.0f);
  if (_countLabel) {
    _countLabel->setColor(cocos2d::Color3B::BLACK);
    _countLabel->setPosition(cocos2d::Vec2(0.0f, -24.0f));
    addChild(_countLabel);
  }
  _addTouchListener();
  return true;
}

void ReservePileView::setCount(std::size_t count) {
  _count = count;
  if (_countLabel) {
    std::ostringstream stream;
    stream << count;
    _countLabel->setString(stream.str());
  }
}

void ReservePileView::setClickable(bool clickable) {
  _clickable = clickable;
  setOpacity(clickable ? 255 : 140);
}

void ReservePileView::setClickCallback(std::function<void()> const &callback) {
  _clickCallback = callback;
}

bool ReservePileView::_containsTouch(cocos2d::Touch *touch) const {
  cocos2d::Vec2 const local = convertToNodeSpace(touch->getLocation());
  cocos2d::Size const size = LayoutConfig::cardSize();
  cocos2d::Rect const rect(-size.width * 0.5f, -size.height * 0.5f, size.width,
                           size.height);
  return rect.containsPoint(local);
}

void ReservePileView::_addTouchListener() {
  cocos2d::EventListenerTouchOneByOne *listener =
      cocos2d::EventListenerTouchOneByOne::create();
  listener->setSwallowTouches(true);
  listener->onTouchBegan = [this](cocos2d::Touch *touch,
                                  cocos2d::Event *event) {
    return _clickable && _count > 0 && _containsTouch(touch);
  };
  listener->onTouchEnded = [this](cocos2d::Touch *touch,
                                  cocos2d::Event *event) {
    if (_clickable && _count > 0 && _clickCallback && _containsTouch(touch)) {
      _clickCallback();
    }
  };
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
