#pragma once
#include <map>
#include <memory>
#include <unordered_map>
#include "CardControllerBase.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardTypes.h"
#include "managers/CardManager.h"
#include "managers/UndoManager.h"
#include "models/GameModel.h"
#include "StackController.h"
#include "views/GameView.h"

/**
 * @brief Wires the game model, view, managers, stack controller, and card
 * controllers.
 */
class GameController {
  public:
  /**
   * @brief Creates a controller for an existing GameView.
   *
   * @param view Non-owning view pointer that receives callbacks and refreshes.
   */
  explicit GameController(GameView *view);

  /**
   * @brief Destroys controller-owned runtime game objects.
   */
  ~GameController();

  /**
   * @brief Loads the initial level, builds dependencies, and binds view
   * callbacks.
   *
   * @return True when startup reaches an interactive state.
   */
  bool start();

  /**
   * @brief Creates runtime cards from the configured level data.
   *
   * @return True when every card is loaded and registered successfully.
   */
  bool initGameModel();

  /**
   * @brief Builds post-load playfield dependency state used for top-card
   * checks.
   */
  void postInitGameModel();

  /**
   * @brief Registers card-type controllers used by the level loader and
   * actions.
   */
  void registerCardController();

  /**
   * @brief Returns the shared runtime game model.
   *
   * @return Shared model owned by the controller.
   */
  std::shared_ptr<GameModel> getGameModel() const {
    return _model;
  }

  /**
   * @brief Returns the card controller for a runtime card id.
   *
   * @param id Runtime card id to resolve.
   * @return Matching card controller, or nullptr when the id cannot be
   * resolved.
   */
  std::shared_ptr<CardControllerBase> getCardControllerOfId(CardId id);

  /**
   * @brief Returns the card controller registered for a card type.
   *
   * @param type Card type to resolve.
   * @return Matching card controller, or nullptr when the type is not
   * registered.
   */
  std::shared_ptr<CardControllerBase> getCardControllerOfType(CardType type);

  /**
   * @brief Returns the playfield dependency manager.
   *
   * @return Read-only card manager reference.
   */
  CardManager &getCardManager() {
    return _cardManager;
  }

  private:
  /**
   * @brief Handles a playfield card click from the view.
   *
   * @param cardId Runtime id of the clicked card.
   */
  void _handleCardClick(CardId cardId);

  /** @brief Handles a reserve pile click from the view. */
  void _handleReserveClickk();

  /** @brief Handles an undo button click from the view. */
  void _handleUndoClick();

  /** @brief Refreshes the view from current model and manager state. */
  void _refreshView();

  /**
   * @brief Builds per-card clickability flags for the current playfield.
   *
   * @return Map from card id to whether that card should be clickable.
   */
  std::unordered_map<CardId, bool> _buildClickability();

  /** @brief Shared runtime game model owned by the controller. */
  std::shared_ptr<GameModel> _model;

  /** @brief Controller that performs reserve draw mutations. */
  std::unique_ptr<StackController> _stackController;

  /** @brief Card-type controllers keyed by CardType. */
  std::unordered_map<CardType, std::shared_ptr<CardControllerBase>>
      _cardTypeControllers;

  /** @brief Non-owning root game view pointer. */
  GameView *_view;

  /** @brief Whether startup completed and user actions should be processed. */
  bool _started;

  /** @brief Tracks playfield card dependencies and top-card availability. */
  CardManager _cardManager;

  /** @brief Stores undo operations created by card and reserve actions. */
  UndoManager _undoManager;
};
