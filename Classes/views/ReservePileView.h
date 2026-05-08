#pragma once
#include <cstddef>
#include <functional>
#include "cocos2d.h"

/** Cocos node that displays the reserve/spare pile and count. */
class ReservePileView : public cocos2d::Node {
  public:
  /** Creates a Cocos autoreleased ReservePileView. */
  CREATE_FUNC(ReservePileView);

  /** Initializes the reserve pile node and touch listener. */
  bool init() override;

  /** Sets the visible reserve card count. */
  void setCount(std::size_t count);

  /** Sets whether the reserve pile accepts clicks. */
  void setClickable(bool clickable);

  /** Registers the reserve click callback. */
  void setClickCallback(std::function<void()> const &callback);

  private:
  bool containsTouch(cocos2d::Touch *touch) const;
  void addTouchListener();

  std::size_t _count = 0;
  bool _clickable = false;
  cocos2d::Label *_countLabel = nullptr;
  std::function<void()> _clickCallback;
};
