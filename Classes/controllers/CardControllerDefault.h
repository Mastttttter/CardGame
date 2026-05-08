#pragma once
#include "CardControllerBase.h"

class CardControllerDefault : public CardControllerBase {
  public:
  explicit CardControllerDefault(GameController *controller)
      : Super(controller) {}

  virtual std::unique_ptr<UndoOperation> doCardAction(CardId cardId) override;

  virtual std::shared_ptr<CardModelBase> generateCardModelFromConfig(
      std::shared_ptr<CardConfigBase> cardConfig) override;

  virtual CardViewBase *createCardView(
      std::shared_ptr<CardModelBase> card) override;
};
