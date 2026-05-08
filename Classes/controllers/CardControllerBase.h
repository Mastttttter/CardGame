#pragma once
#include <cstddef>
#include <memory>
#include "configs/models/CardTypes.h"
#include "managers/UndoOperation.h"
#include "models/CardModelBase.h"
#include "views/CardViewBase.h"

class GameController;

/** Extension seam for type-specific card actions. */
class CardControllerBase {
  public:
  CardControllerBase(GameController *controller)
      : _gameController(controller) {}

  /** Destroys the type controller. */
  virtual ~CardControllerBase() {}

  /** Performs a card action and returns its undo operation. */
  virtual std::unique_ptr<UndoOperation> doCardAction(CardId cardId) = 0;

  virtual std::shared_ptr<CardModelBase> generateCardModelFromConfig(
      std::shared_ptr<CardConfigBase> cardConfig) = 0;
  virtual void handleCardClick(CardId cardId) = 0;

  GameController *getGameController() {
    return _gameController;
  };

  virtual CardViewBase *createCardView(std::shared_ptr<CardModelBase> card) = 0;

  private:
  GameController *_gameController = nullptr;
};
