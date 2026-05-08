#pragma once

#include <functional>
#include <unordered_map>
#include "CardViewBase.h"
#include "cocos2d.h"
#include "configs/models/CardTypes.h"
#include "models/GameModel.h"

class GameController;
class ReservePileView;

/** Root Cocos UI node for cards, reserve pile, and undo control. */
class GameView : public cocos2d::Node {
  public:
  /** Creates a Cocos autoreleased GameView. */
  CREATE_FUNC(GameView);

  /** Initializes the root game UI. */
  bool init() override;

  /** Creates card nodes for a freshly generated model. */
  void setup(std::shared_ptr<GameModel> model);

  /** Synchronizes visible state from the model and clickability map. */
  void refresh(std::shared_ptr<GameModel> model,
               std::unordered_map<CardId, bool> const &clickability,
               bool canUndo);

  /** Enables or suppresses user input forwarding. */
  void setInputEnabled(bool enabled);

  /** Registers the playfield card click callback. */
  void setCardClickCallback(std::function<void(CardId)> const &callback);

  void setReserveClickCallback(std::function<void()> const &callback);

  /** Registers the undo button click callback. */
  void setUndoClickCallback(std::function<void()> const &callback);

  void setGameController(GameController *);
  GameController *getGameController() const;

  /** Shows a card immediately at a design-space position. */
  void showCardAtPosition(CardId cardId, cocos2d::Vec2 const &position);

  /** Runs a simple move animation for a card. */
  void animateCardToPosition(CardId cardId, cocos2d::Vec2 const &position,
                             std::function<void()> const &completion);

  private:
  void createOrConfigureCardView(std::shared_ptr<CardModelBase> card);

  CardViewBase *getCardView(CardId cardId) const;

  std::unordered_map<CardId, CardViewBase *> _cardViews;

  cocos2d::MenuItemLabel *_undoItem = nullptr;
  bool _inputEnabled = true;
  std::function<void(CardId)> _cardClickCallback;
  std::function<void()> _reserveClickCallback;
  std::function<void()> _undoClickCallback;
  GameController *_gameController;
  ReservePileView *_reservePileView;
};
