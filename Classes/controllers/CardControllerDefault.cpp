#include "CardControllerDefault.h"
#include <memory>
#include "CCPlatformMacros.h"
#include "configs/LayoutConfig.h"
#include "GameController.h"
#include "models/CardModelDefault.h"
#include "views/CardViewDefault.h"

namespace details {

/**
 * @brief Undo operation that restores a default card moved from playfield to tray.
 */
class DefaultMoveUndoOperation : public UndoOperation {
  public:
  /**
   * @brief Captures model and position state needed to undo a default-card move.
   *
   * @param model Shared model that will be restored during undo.
   * @param cardId Runtime id of the moved card.
   * @param previousTrayId Tray card id captured before the move.
   * @param originalPosition Position used for the undo animation target.
   */
  DefaultMoveUndoOperation(std::shared_ptr<GameModel> model, CardId cardId,
                           CardId previousTrayId,
                           cocos2d::Vec2 const &originalPosition)
      : _model(model),
        _cardId(cardId),
        _previousTrayId(previousTrayId),
        _originalPosition(originalPosition) {}

  /**
   * @brief Restores the moved card to playfield state.
   */
  void undo() override {
    if (_model) {
      _model->restoreCardToPlayfield(_cardId, _previousTrayId);
    }
  }

  /**
   * @brief Fills animation metadata for returning the card to its original position.
   *
   * @param animation Output animation metadata to fill.
   * @return True when animation metadata was written.
   */
  bool getUndoAnimation(UndoAnimation *animation) const override {
    if (!animation) {
      return false;
    }

    animation->cardId = _cardId;
    animation->targetPosition = _originalPosition;
    return true;
  }

  private:
  /** @brief Shared model restored by this undo operation. */
  std::shared_ptr<GameModel> _model;

  /** @brief Runtime id of the card moved to the tray. */
  CardId _cardId;

  /** @brief Tray card id captured before the move. */
  CardId _previousTrayId;

  /** @brief Original playfield position used by the undo animation. */
  cocos2d::Vec2 _originalPosition;
};

}  // namespace details

std::unique_ptr<UndoOperation> CardControllerDefault::doCardAction(
    CardId cardId) {
  CCLOG("info: CardControllerDefault doing action");
  CardId const previousTrayId = _model->getTrayCardId();
  auto card = _model->findCard(cardId);
  cocos2d::Vec2 const originalPosition = card->getOriginalPosition();
  _model->moveCardToTray(cardId, LayoutConfig::trayPosition());

  return std::unique_ptr<UndoOperation>(new details::DefaultMoveUndoOperation(
      _model, cardId, previousTrayId, originalPosition));
}

std::shared_ptr<CardModelBase>
CardControllerDefault::generateCardModelFromConfig(
    std::shared_ptr<CardConfigBase> cardConfig) {
  return std::make_shared<CardModelDefault>(cardConfig);
}

CardViewBase *CardControllerDefault::createCardView(
    std::shared_ptr<CardModelBase> card) {
  return CardViewDefault::create();
}

bool CardControllerDefault::checkIfClickable(CardId cardId) const {
  auto model = getGameController()->getGameModel();
  auto trayCard = model->findCard(model->getTrayCardId());
  auto card = model->findCard(cardId);
  auto defaultCard = std::dynamic_pointer_cast<CardModelDefault>(trayCard);
  if (!defaultCard) {
    return true;
  }
  auto cardModel = std::dynamic_pointer_cast<CardModelDefault>(card);
  if (!cardModel) {
    CCLOG("Error: un matched type when CardModelDefault check if clickable");
    return false;
  }
  return std::abs(defaultCard->getFace() - cardModel->getFace()) == 1;
}
