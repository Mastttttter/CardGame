#pragma once

#include <memory>
#include "configs/models/CardTypes.h"
#include "managers/UndoOperation.h"

class GameModel;

/** Coordinates reserve/spare-pile draw actions. */
class StackController {
  public:
  /** Creates a stack controller over the runtime model. */
  explicit StackController(GameModel *model);

  /** Draws the next reserve card and returns undo data on success. */
  std::unique_ptr<UndoOperation> drawReserveCard(CardId *drawnCardId);

  private:
  GameModel *_model;
};
