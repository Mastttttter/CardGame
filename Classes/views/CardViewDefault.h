#pragma once
#include <functional>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"
#include "views/CardViewBase.h"

/**
 * @brief Cocos node that displays one standard suit-and-rank playing card.
 */
class CardViewDefault : public CardViewBase {
  public:
  /**
   * @brief Creates an autoreleased CardViewDefault instance.
   *
   * @return Autoreleased CardViewDefault pointer, or nullptr when initialization fails.
   */
  CREATE_FUNC(CardViewDefault);

  /**
   * @brief Destroys the default card view through the derived type.
   */
  virtual ~CardViewDefault() = default;

  /**
   * @brief Configures rank and suit sprites from a default card model.
   *
   * @param card Runtime card model displayed by this node.
   */
  virtual void configure(std::shared_ptr<CardModelBase> card) override;
};
