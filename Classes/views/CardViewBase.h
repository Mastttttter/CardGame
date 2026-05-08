#pragma once
#include <functional>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"

/** Cocos node that displays one default playing card. */
class CardViewBase : public cocos2d::Node {
  public:
  using Super = CardViewBase;
  virtual ~CardViewBase() = default;
  /** Initializes the card node and touch listener. */
  virtual bool init() override;
  /** Configures the displayed card face and suit. */
  virtual void configure(std::shared_ptr<CardModelBase> card);

  /** Sets whether the card accepts clicks. */
  virtual void setClickable(bool clickable);
  /** Registers the click callback for this card. */
  virtual void setClickCallback(std::function<void(CardId)> const &callback);

  /** Returns the runtime card id displayed by this node. */
  virtual CardId getCardId() const;

  private:
  bool containsTouch(cocos2d::Touch *touch) const;
  void addTouchListener();

  CardId _cardId = INVALID_CARD_ID;
  bool _clickable = false;
  std::function<void(CardId)> _clickCallback;
};
