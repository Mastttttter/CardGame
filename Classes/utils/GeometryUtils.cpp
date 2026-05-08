#include "utils/GeometryUtils.h"
#include <algorithm>

cocos2d::Rect GeometryUtils::centeredRect(cocos2d::Vec2 const &center,
                                          cocos2d::Size const &size) {
  return cocos2d::Rect(center.x - size.width * 0.5f,
                       center.y - size.height * 0.5f, size.width, size.height);
}

bool GeometryUtils::overlapsWithPositiveArea(cocos2d::Rect const &first,
                                             cocos2d::Rect const &second) {
  float const left = std::max(first.getMinX(), second.getMinX());
  float const right = std::min(first.getMaxX(), second.getMaxX());
  float const bottom = std::max(first.getMinY(), second.getMinY());
  float const top = std::min(first.getMaxY(), second.getMaxY());

  return right > left && top > bottom;
}
