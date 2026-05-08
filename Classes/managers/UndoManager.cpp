#include "managers/UndoManager.h"

void UndoManager::push(std::unique_ptr<UndoOperation> operation) {
  if (operation) {
    _operations.push_back(std::move(operation));
  }
}

bool UndoManager::undo(UndoAnimation *animation) {
  if (_operations.empty()) {
    return false;
  }

  std::unique_ptr<UndoOperation> operation = std::move(_operations.back());
  _operations.pop_back();

  if (animation) {
    animation->cardId = INVALID_CARD_ID;
    operation->getUndoAnimation(animation);
  }

  operation->undo();
  return true;
}

bool UndoManager::canUndo() const {
  return !_operations.empty();
}

void UndoManager::clear() {
  _operations.clear();
}
