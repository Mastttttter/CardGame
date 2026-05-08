#pragma once

#include <functional>
#include <unordered_map>
#include "CardViewBase.h"
#include "cocos2d.h"
#include "configs/models/CardTypes.h"
#include "models/GameModel.h"

class GameController;
class ReservePileView;

/**
 * @brief Root Cocos UI node for cards, reserve pile, and undo control.
 */
class GameView : public cocos2d::Node {
  public:
  /**
   * @brief Creates an autoreleased GameView instance.
   *
   * @return Autoreleased GameView pointer, or nullptr when initialization fails.
   */
  CREATE_FUNC(GameView);

  /**
   * @brief Initializes the root game UI and static child controls.
   *
   * @return True when the Cocos node initializes successfully.
   */
  bool init() override;

  /**
   * @brief Creates card nodes for a freshly generated model.
   *
   * @param model Runtime model containing cards to display.
   */
  void setup(std::shared_ptr<GameModel> model);

  /**
   * @brief Synchronizes visible state from the model and clickability map.
   *
   * @param model Runtime model that provides card positions and zones.
   * @param clickability Per-card playfield clickability flags.
   * @param canUndo Whether the undo button should be enabled.
   */
  void refresh(std::shared_ptr<GameModel> model,
               std::unordered_map<CardId, bool> const &clickability,
               bool canUndo);

  /**
   * @brief Enables or suppresses user input forwarding.
   *
   * @param enabled Whether card, reserve, and undo callbacks may fire.
   */
  void setInputEnabled(bool enabled);

  /**
   * @brief Registers the playfield card click callback.
   *
   * @param callback Callback invoked with the clicked card id.
   */
  void setCardClickCallback(std::function<void(CardId)> const &callback);

  /**
   * @brief Registers the reserve pile click callback.
   *
   * @param callback Callback invoked when reserve pile is clicked.
   */
  void setReserveClickCallback(std::function<void()> const &callback);

  /**
   * @brief Registers the undo button click callback.
   *
   * @param callback Callback invoked when undo is clicked.
   */
  void setUndoClickCallback(std::function<void()> const &callback);

  /**
   * @brief Stores the non-owning game controller pointer used for view creation.
   *
   * @param controller Non-owning game controller pointer.
   */
  void setGameController(GameController *);

  /**
   * @brief Returns the non-owning game controller pointer.
   *
   * @return Game controller pointer used by this view.
   */
  GameController *getGameController() const;

  /**
   * @brief Shows a card immediately at a design-space position.
   *
   * @param cardId Runtime id of the card view to position.
   * @param position Design-space center position to apply.
   */
  void showCardAtPosition(CardId cardId, cocos2d::Vec2 const &position);

  /**
   * @brief Runs a simple move animation for a card.
   *
   * @param cardId Runtime id of the card view to animate.
   * @param position Design-space target center position.
   * @param completion Optional callback invoked after animation completes.
   */
  void animateCardToPosition(CardId cardId, cocos2d::Vec2 const &position,
                             std::function<void()> const &completion);

  private:
  /**
   * @brief Creates a card view when needed and applies card configuration.
   *
   * @param card Runtime card model to display.
   */
  void _createOrConfigureCardView(std::shared_ptr<CardModelBase> card);

  /**
   * @brief Finds the view node for a runtime card id.
   *
   * @param cardId Runtime card id to look up.
   * @return Card view pointer, or nullptr when the id is not visible.
   */
  CardViewBase *_getCardView(CardId cardId) const;

  /** @brief Card view nodes keyed by runtime card id. */
  std::unordered_map<CardId, CardViewBase *> _cardViews;

  /** @brief Undo menu item controlled by current undo availability. */
  cocos2d::MenuItemLabel *_undoItem = nullptr;

  /** @brief Whether user input callbacks are currently forwarded. */
  bool _inputEnabled = true;

  /** @brief Callback invoked when a visible playfield card is clicked. */
  std::function<void(CardId)> _cardClickCallback;

  /** @brief Callback invoked when the reserve pile is clicked. */
  std::function<void()> _reserveClickCallback;

  /** @brief Callback invoked when the undo button is clicked. */
  std::function<void()> _undoClickCallback;

  /** @brief Non-owning game controller pointer used to create card views. */
  GameController *_gameController;

  /** @brief Child node that displays reserve pile count and input state. */
  ReservePileView *_reservePileView;
};
