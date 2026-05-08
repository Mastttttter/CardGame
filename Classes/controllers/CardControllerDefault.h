#pragma once
#include "CardControllerBase.h"
#include "CCPlatformMacros.h"
#include "controllers/GameController.h"
class GameModel;

class CardControllerDefault : public CardControllerBase {
  public:
  explicit CardControllerDefault(GameController *controller)
      : Super(controller) {
    _model = controller->getGameModel();
  }

  virtual std::unique_ptr<UndoOperation> doCardAction(CardId cardId) override;

  virtual std::shared_ptr<CardModelBase> generateCardModelFromConfig(
      std::shared_ptr<CardConfigBase> cardConfig) override;

  virtual CardViewBase *createCardView(
      std::shared_ptr<CardModelBase> card) override;

  virtual bool checkIfClickable(CardId cardId) const override;

  private:
  std::shared_ptr<GameModel> _model;
};
