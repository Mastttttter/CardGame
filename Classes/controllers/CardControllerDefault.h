#pragma once
#include "CardControllerBase.h"
#include "CCPlatformMacros.h"
#include "controllers/GameController.h"
class GameModel;

/**
 * @brief Card controller for standard suit-and-rank cards.
 */
class CardControllerDefault : public CardControllerBase {
  public:
  /**
   * @brief Creates a default-card controller bound to a game controller.
   *
   * @param controller Non-owning game controller pointer used to access shared state.
   */
  explicit CardControllerDefault(GameController *controller)
      : Super(controller) {
    _model = controller->getGameModel();
  }

  /**
   * @brief Moves a clicked playfield card to the tray and captures undo state.
   *
   * @param cardId Runtime id of the clicked card.
   * @return Undo operation that restores the card and previous tray id.
   */
  virtual std::unique_ptr<UndoOperation> doCardAction(CardId cardId) override;

  /**
   * @brief Creates the default runtime card model for static card data.
   *
   * @param cardConfig Static card configuration loaded from the level file.
   * @return Runtime default-card model.
   */
  virtual std::shared_ptr<CardModelBase> generateCardModelFromConfig(
      std::shared_ptr<CardConfigBase> cardConfig) override;

  /**
   * @brief Creates the Cocos view node used for a default card.
   *
   * @param card Runtime card model that the view will display.
   * @return Autoreleased default card view node pointer.
   */
  virtual CardViewBase *createCardView(
      std::shared_ptr<CardModelBase> card) override;

  /**
   * @brief Checks whether a playfield card can match the current tray card.
   *
   * @param cardId Runtime id of the playfield card to test.
   * @return True when the card can be clicked under default-card rules.
   */
  virtual bool checkIfClickable(CardId cardId) const override;

  private:
  /** @brief Shared runtime game model observed by this controller. */
  std::shared_ptr<GameModel> _model;
};
