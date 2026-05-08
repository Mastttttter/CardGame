#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"

/**
 * @brief Tracks playfield card dependencies and top-card availability.
 */
class CardManager {
  public:
  /**
   * @brief Returns whether a playfield card has no covering cards above it.
   *
   * @param id Runtime card id to inspect.
   * @return True when the card is tracked and currently uncovered.
   */
  bool isOnTop(CardId id) const;

  /**
   * @brief Records that one playfield card covers another card.
   *
   * @param u Runtime id of the covering card.
   * @param v Runtime id of the covered card.
   */
  void addConnection(CardId u, CardId v);

  /**
   * @brief Builds the dependency graph from recorded card connections.
   */
  void constructGraph();

  /**
   * @brief Updates dependency counts after a card leaves the playfield.
   *
   * @param id Runtime id of the removed card.
   */
  void removeCard(CardId id);

  /**
   * @brief Restores dependency counts after a card returns to the playfield.
   *
   * @param id Runtime id of the restored card.
   */
  void addCard(CardId id);

  private:
  /**
   * @brief Returns whether an id is tracked by the dependency graph.
   *
   * @param id Runtime card id to validate.
   * @return True when the id exists in the dependency counts map.
   */
  bool _isValidId(CardId id) const;

  /** @brief Raw covering-card to covered-card relationships. */
  std::vector<std::pair<CardId, CardId>> _connections;

  /** @brief Adjacency list from covering cards to covered cards. */
  std::unordered_map<CardId, std::vector<CardId>> _originCardGraph;

  /** @brief Remaining covering-card count for each tracked card. */
  std::unordered_map<CardId, int> _degree;
};
