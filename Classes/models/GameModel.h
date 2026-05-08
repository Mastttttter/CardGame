#pragma once
#include <cstddef>
#include <vector>
#include "configs/models/CardTypes.h"
#include "models/CardModelBase.h"

/**
 * @brief Owns runtime card state, tray identity, and reserve/playfield order.
 */
class GameModel {
  public:
  /**
   * @brief Creates an empty game model.
   */
  GameModel();

  /**
   * @brief Adds a card to the model and assigns its runtime id.
   *
   * @param cardModel Runtime card model to store.
   * @param zone Initial zone for the card.
   * @param playfieldOrder Initial playfield order, or stack ordering value.
   * @return Newly assigned stable card id.
   */
  CardId addCard(std::shared_ptr<CardModelBase> const, CardZone zone,
                 int playfieldOrder);

  /**
   * @brief Tracks a card id in original playfield order.
   *
   * @param cardId Card id to append to the playfield list.
   */
  void addPlayfieldCardId(CardId cardId);

  /**
   * @brief Tracks a card id in current reserve draw order.
   *
   * @param cardId Card id to append to the reserve list.
   */
  void addReserveCardId(CardId cardId);

  /**
   * @brief Sets the current tray/top card id.
   *
   * @param cardId Card id to mark as the tray card.
   */
  void setTrayCardId(CardId cardId);

  /**
   * @brief Returns the current tray/top card id.
   *
   * @return Tray card id, or INVALID_CARD_ID when no tray card exists.
   */
  CardId getTrayCardId() const;

  /**
   * @brief Returns whether a tray/top card is present.
   *
   * @return True when the tray card id is valid.
   */
  bool hasTrayCard() const;

  /**
   * @brief Returns the number of cards available in reserve.
   *
   * @return Current reserve card count.
   */
  std::size_t getReserveCount() const;

  /**
   * @brief Returns all runtime cards.
   *
   * @return Cards in creation order.
   */
  std::vector<std::shared_ptr<CardModelBase>> const &getCards() const;

  /**
   * @brief Returns original playfield ids in configuration order.
   *
   * @return Playfield card id list.
   */
  std::vector<CardId> const &getPlayfieldCardIds() const;

  /**
   * @brief Returns current reserve ids in draw order.
   *
   * @return Reserve card id list.
   */
  std::vector<CardId> const &getReserveCardIds() const;

  /**
   * @brief Finds a card by id.
   *
   * @param cardId Card id to look up.
   * @return Matching card model, or nullptr when the id is not found.
   */
  std::shared_ptr<CardModelBase> findCard(CardId cardId);

  /**
   * @brief Moves a card into the tray/top zone.
   *
   * @param cardId Card id to move.
   * @param trayPosition Design-space position to assign to the card.
   */
  void moveCardToTray(CardId cardId, cocos2d::Vec2 const &trayPosition);

  /**
   * @brief Draws the next reserve card into the tray/top zone.
   *
   * @param trayPosition Design-space tray position to assign to the drawn card.
   * @return Drawn card id, or INVALID_CARD_ID when reserve draw fails.
   */
  CardId drawReserveCardToTray(cocos2d::Vec2 const &trayPosition);

  /**
   * @brief Restores a moved playfield card and previous tray/top id.
   *
   * @param cardId Card id to return to its original playfield position.
   * @param previousTrayId Tray card id captured before the move.
   */
  void restoreCardToPlayfield(CardId cardId, CardId previousTrayId);

  /**
   * @brief Returns a drawn card to the front of reserve.
   *
   * @param cardId Drawn card id to return to reserve.
   * @param previousTrayId Tray card id captured before the draw.
   * @param reservePosition Design-space reserve position to assign to the card.
   */
  void returnCardToReserveFront(CardId cardId, CardId previousTrayId,
                                cocos2d::Vec2 const &reservePosition);

  private:
  /** @brief Runtime cards indexed by stable card id. */
  std::unordered_map<CardId, std::shared_ptr<CardModelBase>> _cardsMap;

  /** @brief Runtime cards in creation order for view synchronization. */
  std::vector<std::shared_ptr<CardModelBase>> _cards;

  /** @brief Card ids that originated in the playfield. */
  std::vector<CardId> _playfieldCardIds;

  /** @brief Card ids currently available in reserve draw order. */
  std::vector<CardId> _reserveCardIds;

  /** @brief Current tray/top card id, or INVALID_CARD_ID when empty. */
  CardId _trayCardId;

  /** @brief Next card id to assign when a runtime card is added. */
  CardId _nextCardId;
};
