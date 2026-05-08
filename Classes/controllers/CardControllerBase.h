#pragma once
#include <cstddef>
#include <memory>
#include "CCPlatformMacros.h"
#include "configs/models/CardTypes.h"
#include "managers/UndoOperation.h"
#include "models/CardModelBase.h"
#include "views/CardViewBase.h"

class GameController;

/**
 * @brief Base interface for card-type-specific model, view, and click behavior.
 */
class CardControllerBase {
  public:
  /** @brief Alias for subclass base construction. */
  using Super = CardControllerBase;

  /**
   * @brief Creates a card controller tied to the owning game controller.
   *
   * @param controller Non-owning game controller pointer used to reach shared state.
   */
  CardControllerBase(GameController *controller)
      : _gameController(controller) {}

  /**
   * @brief Destroys the type controller through the base type.
   */
  virtual ~CardControllerBase() {}

  /**
   * @brief Performs a card action and returns its undo operation.
   *
   * @param cardId Runtime id of the clicked card.
   * @return Undo operation for the action, or nullptr when no action is produced.
   */
  virtual std::unique_ptr<UndoOperation> doCardAction(CardId cardId) = 0;

  /**
   * @brief Creates a runtime card model from static configuration.
   *
   * @param cardConfig Static card configuration loaded from the level file.
   * @return Runtime card model owned by shared pointer.
   */
  virtual std::shared_ptr<CardModelBase> generateCardModelFromConfig(
      std::shared_ptr<CardConfigBase> cardConfig) = 0;

  /**
   * @brief Returns whether the card is currently allowed to react to clicks.
   *
   * @param cardId Runtime id of the card to test.
   * @return True when the card can be clicked.
   */
  virtual bool checkIfClickable(CardId) const {
    return true;
  }

  /**
   * @brief Handles a card click before the game controller applies the action.
   *
   * @param cardId Runtime id of the clicked card.
   */
  virtual void _handleCardClick(CardId cardId) {
    CCLOG("info: cardControllerBase receive click");
  };

  /**
   * @brief Returns the owning game controller pointer.
   *
   * @return Non-owning game controller pointer.
   */
  GameController *getGameController() const {
    return _gameController;
  };

  /**
   * @brief Creates the Cocos view node used to display a card model.
   *
   * @param card Runtime card model that the view will display.
   * @return Autoreleased card view node pointer.
   */
  virtual CardViewBase *createCardView(std::shared_ptr<CardModelBase> card) = 0;

  protected:
  /** @brief Non-owning pointer to the game controller that owns this controller. */
  GameController *_gameController = nullptr;
};
