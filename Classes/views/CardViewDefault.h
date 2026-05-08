#pragma once
#include <functional>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"
#include "views/CardViewBase.h"

/** Cocos node that displays one default playing card. */
class CardViewDefault : public CardViewBase {
  public:
  CREATE_FUNC(CardViewDefault);

  virtual ~CardViewDefault() = default;
  /** Configures the displayed card face and suit. */
  virtual void configure(std::shared_ptr<CardModelBase> card) override;
};
