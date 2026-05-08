#pragma once
#include <map>
#include <memory>
#include <unordered_map>
#include "CardControllerBase.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardTypes.h"
#include "models/GameModel.h"
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

  void registerCardController();

  std::shared_ptr<CardControllerBase> getCardControllerOfId(CardId id);

  std::shared_ptr<CardControllerBase> getCardControllerOfType(CardType type);

  private:
  void handleCardClick(CardId cardId);

  void handleReserveClick();
  void handleUndoClick();
  std::shared_ptr<GameModel> _model;
  std::unordered_map<CardType, std::shared_ptr<CardControllerBase>>
      _cardTypeControllers;
  GameView *_view;
  bool _started;
};
