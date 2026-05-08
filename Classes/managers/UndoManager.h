#pragma once
#include <concepts>
#include <vector>
#include "managers/UndoOperation.h"

/** Controller-owned LIFO storage for undo operations. */
class UndoManager {
  public:
  /** Pushes an undo operation when it is non-null. */
  void push(std::unique_ptr<UndoOperation> operation);

  /** Runs and removes the latest undo operation. */
  bool undo(UndoAnimation *animation);

  /** Returns whether an undo operation is available. */
  bool canUndo() const;

  /** Removes all stored undo operations. */
  void clear();

  private:
  std::vector<std::unique_ptr<UndoOperation>> _operations;
};
