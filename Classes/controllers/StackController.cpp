#include "controllers/StackController.h"
#include "configs/LayoutConfig.h"
#include "configs/models/CardTypes.h"
#include "models/GameModel.h"

namespace details {

/**
 * @brief Undo operation that returns a drawn reserve card to reserve state.
 */
class ReserveDrawUndoOperation : public UndoOperation {
  public:
  /**
   * @brief Captures model and position state needed to undo a reserve draw.
   *
   * @param model Non-owning model pointer restored during undo.
   * @param drawnCardId Runtime id of the card drawn from reserve.
   * @param previousTrayId Tray card id captured before the draw.
   * @param reservePosition Position used for the undo animation target.
   */
  ReserveDrawUndoOperation(GameModel *model, CardId drawnCardId,
                           CardId previousTrayId,
                           cocos2d::Vec2 const &reservePosition)
      : _model(model),
        _drawnCardId(drawnCardId),
        _previousTrayId(previousTrayId),
        _reservePosition(reservePosition) {}

  /**
   * @brief Restores the drawn card to the reserve pile.
   */
  void undo() override {
    if (_model) {
      _model->returnCardToReserveFront(_drawnCardId, _previousTrayId,
                                       _reservePosition);
    }
  }

  /**
   * @brief Fills animation metadata for returning the card to reserve position.
   *
   * @param animation Output animation metadata to fill.
   * @return True when animation metadata was written.
   */
  bool getUndoAnimation(UndoAnimation *animation) const override {
    if (!animation) {
      return false;
    }

    animation->cardId = _drawnCardId;
    animation->targetPosition = _reservePosition;
    return true;
  }

  private:
  /** @brief Non-owning model pointer restored by this undo operation. */
  GameModel *_model;

  /** @brief Runtime id of the card drawn from reserve. */
  CardId _drawnCardId;

  /** @brief Tray card id captured before the reserve draw. */
  CardId _previousTrayId;

  /** @brief Reserve pile position used by the undo animation. */
  cocos2d::Vec2 _reservePosition;
};

}  // namespace details

StackController::StackController(GameModel *model) : _model(model) {}

std::unique_ptr<UndoOperation> StackController::drawReserveCard(
    CardId *drawnCardId) {
  if (drawnCardId) {
    *drawnCardId = INVALID_CARD_ID;
  }
  if (!_model || _model->getReserveCount() == 0) {
    return std::unique_ptr<UndoOperation>();
  }
  CardId const previousTrayId = _model->getTrayCardId();
  CardId const drawn =
      _model->drawReserveCardToTray(LayoutConfig::trayPosition());
  if (drawn == INVALID_CARD_ID) {
    return std::unique_ptr<UndoOperation>();
  }
  if (drawnCardId) {
    *drawnCardId = drawn;
  }
  return std::unique_ptr<UndoOperation>(new details::ReserveDrawUndoOperation(
      _model, drawn, previousTrayId, LayoutConfig::reservePilePosition()));
}
