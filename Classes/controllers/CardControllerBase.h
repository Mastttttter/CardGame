#pragma once
#include <memory>
#include "configs/models/CardTypes.h"
#include "managers/UndoOperation.h"
#include "models/CardModelBase.h"

/** Extension seam for type-specific card actions. */
class CardControlerBase {
  public:
  /** Destroys the type controller. */
  virtual ~CardControlerBase() {}

  /** Performs a card action and returns its undo operation. */
  virtual std::unique_ptr<UndoOperation> doCardAction(CardId cardId) = 0;

  virtual std::shared_ptr<CardModelBase> generateCardModelFromConfig(
      std::shared_ptr<CardConfigBase> cardConfig) = 0;
  virtual void handleCardClick(CardId cardId) = 0;
};
