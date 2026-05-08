#pragma once
#include <cstddef>
#include <functional>
#include "cocos2d.h"

/**
 * @brief Cocos node that displays the reserve pile and remaining card count.
 */
class ReservePileView : public cocos2d::Node {
  public:
  /**
   * @brief Creates an autoreleased ReservePileView instance.
   *
   * @return Autoreleased ReservePileView pointer, or nullptr when initialization fails.
   */
  CREATE_FUNC(ReservePileView);

  /**
   * @brief Initializes the reserve pile node and touch listener.
   *
   * @return True when the Cocos node initializes successfully.
   */
  bool init() override;

  /**
   * @brief Sets the visible reserve card count.
   *
   * @param count Number displayed on the reserve pile.
   */
  void setCount(std::size_t count);

  /**
   * @brief Sets whether the reserve pile accepts clicks.
   *
   * @param clickable Whether touch input should be accepted.
   */
  void setClickable(bool clickable);

  /**
   * @brief Registers the reserve click callback.
   *
   * @param callback Callback invoked when the reserve pile is clicked.
   */
  void setClickCallback(std::function<void()> const &callback);

  private:
  /**
   * @brief Returns whether a touch falls inside the reserve pile bounds.
   *
   * @param touch Touch to test in node-local coordinates.
   * @return True when the touch is inside the card-sized hit area.
   */
  bool _containsTouch(cocos2d::Touch *touch) const;

  /** @brief Adds the one-by-one touch listener used for reserve clicks. */
  void _addTouchListener();

  /** @brief Current reserve count displayed by the label. */
  std::size_t _count = 0;

  /** @brief Whether the reserve pile currently accepts clicks. */
  bool _clickable = false;

  /** @brief Label node that displays the current reserve count. */
  cocos2d::Label *_countLabel = nullptr;

  /** @brief Callback invoked when the reserve pile is clicked. */
  std::function<void()> _clickCallback;
};
