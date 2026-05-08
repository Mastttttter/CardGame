#pragma once
#include "cocos2d.h"

/** Independent helpers for simple card geometry. */
class GeometryUtils {
  public:
  /** Builds a rectangle from a center point and size. */
  static cocos2d::Rect centeredRect(cocos2d::Vec2 const &center,
                                    cocos2d::Size const &size);

  /** Returns true when two rects overlap with positive area. */
  static bool overlapsWithPositiveArea(cocos2d::Rect const &first,
                                       cocos2d::Rect const &second);
};
