#pragma once
#include <functional>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"

/**
 * @brief Base Cocos node for a clickable runtime card view.
 */
class CardViewBase : public cocos2d::Node {
  public:
  /** @brief Alias for subclass base calls. */
  using Super = CardViewBase;

  /**
   * @brief Destroys the card view through the base type.
   */
  virtual ~CardViewBase() = default;

  /**
   * @brief Initializes the card node and touch listener.
   *
   * @return True when the Cocos node initializes successfully.
   */
  virtual bool init() override;

  /**
   * @brief Configures the view for a runtime card model.
   *
   * @param card Runtime card model displayed by this node.
   */
  virtual void configure(std::shared_ptr<CardModelBase> card);

  /**
   * @brief Sets whether the card accepts clicks.
   *
   * @param clickable Whether touch input should be accepted.
   */
  virtual void setClickable(bool clickable);

  /**
   * @brief Registers the click callback for this card.
   *
   * @param callback Callback invoked with this card id after a valid touch.
   */
  virtual void setClickCallback(std::function<void(CardId)> const &callback);

  /**
   * @brief Returns the runtime card id displayed by this node.
   *
   * @return Runtime card id, or INVALID_CARD_ID before configuration.
   */
  virtual CardId getCardId() const;

  private:
  /**
   * @brief Returns whether a touch falls inside the card bounds.
   *
   * @param touch Touch to test in node-local coordinates.
   * @return True when the touch is inside the card-sized hit area.
   */
  bool _containsTouch(cocos2d::Touch *touch) const;

  /** @brief Adds the one-by-one touch listener used for card clicks. */
  void _addTouchListener();

  /** @brief Runtime id of the card currently displayed by this view. */
  CardId _cardId = INVALID_CARD_ID;

  /** @brief Whether this card currently accepts clicks. */
  bool _clickable = false;

  /** @brief Callback invoked when the card is clicked. */
  std::function<void(CardId)> _clickCallback;
};
