#include "CardControllerDefault.h"
#include <memory>
#include "CCPlatformMacros.h"
#include "configs/LayoutConfig.h"
#include "GameController.h"
#include "models/CardModelDefault.h"
#include "views/CardViewDefault.h"

namespace details {

class DefaultMoveUndoOperation : public UndoOperation {
  public:
  DefaultMoveUndoOperation(std::shared_ptr<GameModel> model, CardId cardId,
                           CardId previousTrayId,
                           cocos2d::Vec2 const &originalPosition)
      : _model(model),
        _cardId(cardId),
        _previousTrayId(previousTrayId),
        _originalPosition(originalPosition) {}

  void undo() override {
    if (_model) {
      _model->restoreCardToPlayfield(_cardId, _previousTrayId);
    }
  }

  bool getUndoAnimation(UndoAnimation *animation) const override {
    if (!animation) {
      return false;
    }

    animation->cardId = _cardId;
    animation->targetPosition = _originalPosition;
    return true;
  }

  private:
  std::shared_ptr<GameModel> _model;
  CardId _cardId;
  CardId _previousTrayId;
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
