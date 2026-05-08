
#include "LayoutConfig.h"

cocos2d::Size LayoutConfig::contentSize() {
  return cocos2d::Size(1080.0f, 2080.0f);
}

cocos2d::Size LayoutConfig::cardSize() {
  return cocos2d::Size(182.0f, 282.0f);
}

cocos2d::Vec2 LayoutConfig::trayPosition() {
  return cocos2d::Vec2(680.0f, 280.0f);
}

cocos2d::Vec2 LayoutConfig::reservePilePosition() {
  return cocos2d::Vec2(400.0f, 280.0f);
}

cocos2d::Vec2 LayoutConfig::undoButtonPosition() {
  return cocos2d::Vec2(930.0f, 1900.0f);
}

float LayoutConfig::cardMoveDuration() {
  return 0.18f;
}

cocos2d::Color4B LayoutConfig::backgroundColor() {
  return cocos2d::Color4B(20, 110, 65, 255);
}

float LayoutConfig::playfieldOffsetFromBottom() {
  return 600.f;
}
