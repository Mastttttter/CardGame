#pragma once
#include <cstddef>
#include <vector>
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"

/** Runtime model for cards, zones, tray, and reserve order. */
class GameModel {
  public:
  /** Creates an empty game model. */
  GameModel();

  /** Adds a card and returns its generated id. */
  CardId addCard(std::shared_ptr<CardModelBase> const, CardZone zone,
                 int playfieldOrder);

  /** Tracks an id in original playfield order. */
  void addPlayfieldCardId(CardId cardId);

  /** Tracks an id in current reserve draw order. */
  void addReserveCardId(CardId cardId);

  /** Sets the current tray card id. */
  void setTrayCardId(CardId cardId);

  /** Returns the current tray/top card id. */
  CardId getTrayCardId() const;

  /** Returns true when a tray/top card is present. */
  bool hasTrayCard() const;

  /** Returns the number of cards available in reserve. */
  std::size_t getReserveCount() const;

  /** Returns all runtime cards. */
  std::vector<std::shared_ptr<CardModelBase>> const &getCards() const;

  /** Returns original playfield ids in config order. */
  std::vector<CardId> const &getPlayfieldCardIds() const;

  /** Returns current reserve ids in draw order. */
  std::vector<CardId> const &getReserveCardIds() const;

  /** Finds a card by id. */
  std::shared_ptr<CardModelBase> findCard(CardId cardId);

  /** Moves a card into the tray/top zone. */
  void moveCardToTray(CardId cardId, cocos2d::Vec2 const &trayPosition);

  /** Draws the next reserve card into tray/top and returns its id. */
  CardId drawReserveCardToTray(cocos2d::Vec2 const &trayPosition);

  /** Restores a moved playfield card and previous tray/top id. */
  void restoreCardToPlayfield(CardId cardId, CardId previousTrayId);

  /** Returns a drawn card to the front of reserve. */
  void returnCardToReserveFront(CardId cardId, CardId previousTrayId,
                                cocos2d::Vec2 const &reservePosition);

  private:
  std::unordered_map<CardId, std::shared_ptr<CardModelBase>> _cardsMap;
  std::vector<std::shared_ptr<CardModelBase>> _cards;
  std::vector<CardId> _playfieldCardIds;
  std::vector<CardId> _reserveCardIds;
  CardId _trayCardId;
  CardId _nextCardId;
};
