#include "controllers/StackController.h"
#include "configs/LayoutConfig.h"
#include "configs/models/CardTypes.h"
#include "models/GameModel.h"

namespace {

class ReserveDrawUndoOperation : public UndoOperation {
  public:
  ReserveDrawUndoOperation(GameModel *model, CardId drawnCardId,
                           CardId previousTrayId,
                           cocos2d::Vec2 const &reservePosition)
      : _model(model),
        _drawnCardId(drawnCardId),
        _previousTrayId(previousTrayId),
        _reservePosition(reservePosition) {}

  void undo() override {
    if (_model) {
      _model->returnCardToReserveFront(_drawnCardId, _previousTrayId,
                                       _reservePosition);
    }
  }

  bool getUndoAnimation(UndoAnimation *animation) const override {
    if (!animation) {
      return false;
    }

    animation->cardId = _drawnCardId;
    animation->targetPosition = _reservePosition;
    return true;
  }

  private:
  GameModel *_model;
  CardId _drawnCardId;
  CardId _previousTrayId;
  cocos2d::Vec2 _reservePosition;
};

}  // namespace

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
  return std::unique_ptr<UndoOperation>(new ReserveDrawUndoOperation(
      _model, drawn, previousTrayId, LayoutConfig::reservePilePosition()));
}
