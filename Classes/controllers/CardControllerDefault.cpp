#include "CardControllerDefault.h"
#include "models/CardModelDefault.h"
#include "views/CardViewDefault.h"

std::unique_ptr<UndoOperation> CardControllerDefault::doCardAction(
    CardId cardId) {
  // TODO do card action in default
  return {};
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
