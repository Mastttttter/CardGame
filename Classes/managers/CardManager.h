#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "cocos2d.h"
#include "configs/models/CardTypes.h"

class CardManager {
  public:
  bool isOnTop(CardId id) const;

  void addConnection(CardId u, CardId v);

  void constructGraph();

  void removeCard(CardId id);

  void addCard(CardId id);

  private:
  bool isValidId(CardId id) const;
  std::vector<std::pair<CardId, CardId>> _connections;
  std::vector<std::vector<CardId>> _originCardGraph;
  std::unordered_map<CardId, int> _degree;
};
