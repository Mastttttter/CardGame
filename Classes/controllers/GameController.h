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

/** Wires model, views, managers, services, and card controllers. */
class GameController {
  public:
  /** Creates a controller for an existing GameView. */
  explicit GameController(GameView *view);

  /** Destroys owned runtime game objects. */
  ~GameController();

  /** Loads the initial level and binds view callbacks. */
  bool start();

  bool initGameModel();

  void postInitGameModel();

  void registerCardController();

  std::shared_ptr<GameModel> getGameModel() const {
    return _model;
  }

  std::shared_ptr<CardControllerBase> getCardControllerOfId(CardId id);

  std::shared_ptr<CardControllerBase> getCardControllerOfType(CardType type);

  CardManager const &getCardManager() const {
    return _cardManager;
  }

  private:
  void handleCardClick(CardId cardId);
  void handleReserveClick();
  void handleUndoClick();
  void refreshView();
  std::unordered_map<CardId, bool> buildClickability();
  std::shared_ptr<GameModel> _model;
  std::unique_ptr<StackController> _stackController;
  std::unordered_map<CardType, std::shared_ptr<CardControllerBase>>
      _cardTypeControllers;
  GameView *_view;
  bool _started;
  CardManager _cardManager;
  UndoManager _undoManager;
};
