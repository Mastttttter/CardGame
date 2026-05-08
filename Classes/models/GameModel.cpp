#include "GameModel.h"
#include <algorithm>
#include "configs/models/CardTypes.h"

GameModel::GameModel() : _trayCardId(INVALID_CARD_ID), _nextCardId(0) {}

CardId GameModel::addCard(std::shared_ptr<CardConfigBase> const &config, CardZone zone,
                          int playfieldOrder) {
  CardId const cardId = _nextCardId++;
  _cards.push_back(
      std::make_shared<CardModelBase>(cardId, config, zone, playfieldOrder));
  return cardId;
}

void GameModel::addPlayfieldCardId(CardId cardId) {
  _playfieldCardIds.push_back(cardId);
}

void GameModel::addReserveCardId(CardId cardId) {
  _reserveCardIds.push_back(cardId);
}

void GameModel::setTrayCardId(CardId cardId) {
  _trayCardId = cardId;
}

CardId GameModel::getTrayCardId() const {
  return _trayCardId;
}

bool GameModel::hasTrayCard() const {
  return _trayCardId != INVALID_CARD_ID;
}

std::size_t GameModel::getReserveCount() const {
  return _reserveCardIds.size();
}

std::vector<std::shared_ptr<CardModelBase>> const &GameModel::getCards() const {
  return _cards;
}

std::vector<CardId> const &GameModel::getPlayfieldCardIds() const {
  return _playfieldCardIds;
}

std::vector<CardId> const &GameModel::getReserveCardIds() const {
  return _reserveCardIds;
}

std::shared_ptr<CardModelBase> GameModel::findCard(CardId cardId) {
  for (auto &card: _cards) {
    if (card->getId() == cardId) {
      return card;
    }
  }
  return nullptr;
}

void GameModel::moveCardToTray(CardId cardId,
                               cocos2d::Vec2 const &trayPosition) {
  auto card = findCard(cardId);
  if (!card) {
    return;
  }
  card->setZone(CardZone::Tray);
  card->setPosition(trayPosition);
  _trayCardId = cardId;
}

CardId GameModel::drawReserveCardToTray(cocos2d::Vec2 const &trayPosition) {
  if (_reserveCardIds.empty()) {
    return INVALID_CARD_ID;
  }
  CardId const cardId = _reserveCardIds.front();
  _reserveCardIds.erase(_reserveCardIds.begin());
  auto card = findCard(cardId);
  if (!card) {
    return INVALID_CARD_ID;
  }
  card->setZone(CardZone::Tray);
  card->setPosition(trayPosition);
  _trayCardId = cardId;
  return cardId;
}

void GameModel::restoreCardToPlayfield(CardId cardId, CardId previousTrayId) {
  auto card = findCard(cardId);
  if (!card) {
    return;
  }
  card->setZone(CardZone::Playfield);
  card->setPosition(card->getOriginalPosition());
  _trayCardId = previousTrayId;
}

void GameModel::returnCardToReserveFront(CardId cardId, CardId previousTrayId,
                                         cocos2d::Vec2 const &reservePosition) {
  auto card = findCard(cardId);
  if (!card) {
    return;
  }
  if (std::find(_reserveCardIds.begin(), _reserveCardIds.end(), cardId) ==
      _reserveCardIds.end()) {
    _reserveCardIds.insert(_reserveCardIds.begin(), cardId);
  }
  card->setZone(CardZone::Reserve);
  card->setPosition(reservePosition);
  _trayCardId = previousTrayId;
}
