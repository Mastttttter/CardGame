#pragma once
#include "cocos2d.h"

/**
 * @brief Provides independent helpers for simple card geometry.
 */
class GeometryUtils {
  public:
  /**
   * @brief Builds a rectangle from a center point and size.
   *
   * @param center Center point of the rectangle.
   * @param size Width and height of the rectangle.
   * @return Rectangle whose center and size match the inputs.
   */
  static cocos2d::Rect centeredRect(cocos2d::Vec2 const &center,
                                    cocos2d::Size const &size);

  /**
   * @brief Returns whether two rectangles overlap with positive area.
   *
   * @param first First rectangle to compare.
   * @param second Second rectangle to compare.
   * @return True when the rectangles share non-zero area.
   */
  static bool overlapsWithPositiveArea(cocos2d::Rect const &first,
                                       cocos2d::Rect const &second);
};
