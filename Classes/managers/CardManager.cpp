#include "CardManager.h"
#include "CCPlatformMacros.h"

bool CardManager::isOnTop(CardId id) const {
  if (!isValidId(id)) {
    return false;
  }
  auto it = _degree.find(id);
  return it->second == 0;
}

void CardManager::addConnection(CardId u, CardId v) {
  _degree[u] = 0;
  _degree[v] = 0;
  _connections.emplace_back(u, v);
}

void CardManager::constructGraph() {
  for (auto &pair: _connections) {
    _originCardGraph[pair.first].emplace_back(pair.second);
    _degree[pair.second]++;
  }
}

void CardManager::removeCard(CardId id) {
  if (!isValidId(id)) {
    return;
  }
  for (auto &v: _originCardGraph[id]) {
    _degree[v]--;
  }
}

void CardManager::addCard(CardId id) {
  if (!isValidId(id)) {
    return;
  }
  for (auto &v: _originCardGraph[id]) {
    _degree[v]++;
  }
}

bool CardManager::isValidId(CardId id) const {
  if (_degree.find(id) == _degree.end()) {
    CCLOG("Warning: try to detect a invalid CardId if on top:%d", id);
    return false;
  }
  return true;
}
