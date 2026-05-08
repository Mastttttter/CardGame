#pragma once

#include <memory>
#include "configs/models/CardTypes.h"
#include "managers/UndoOperation.h"

class GameModel;

/**
 * @brief Coordinates reserve-pile draw actions against the runtime model.
 */
class StackController {
  public:
  /**
   * @brief Creates a stack controller over the runtime model.
   *
   * @param model Non-owning model pointer that stores reserve and tray state.
   */
  explicit StackController(GameModel *model);

  /**
   * @brief Draws the next reserve card and captures undo state.
   *
   * @param drawnCardId Optional output pointer that receives the drawn card id.
   * @return Undo operation for the draw, or nullptr when no card is drawn.
   */
  std::unique_ptr<UndoOperation> drawReserveCard(CardId *drawnCardId);

  private:
  /** @brief Non-owning runtime model pointer used for reserve mutations. */
  GameModel *_model;
};
