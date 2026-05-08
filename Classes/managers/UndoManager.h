#pragma once
#include <concepts>
#include <vector>
#include "managers/UndoOperation.h"

/**
 * @brief Controller-owned LIFO storage for undo operations.
 */
class UndoManager {
  public:
  /**
   * @brief Pushes an undo operation when it is non-null.
   *
   * @param operation Operation to take ownership of.
   */
  void push(std::unique_ptr<UndoOperation> operation);

  /**
   * @brief Runs and removes the latest undo operation.
   *
   * @param animation Optional output animation metadata for the restored state.
   * @return True when an operation was undone.
   */
  bool undo(UndoAnimation *animation);

  /**
   * @brief Returns whether an undo operation is available.
   *
   * @return True when the undo stack is not empty.
   */
  bool canUndo() const;

  /**
   * @brief Removes all stored undo operations.
   */
  void clear();

  private:
  /** @brief Owned undo operations in execution order, newest at the back. */
  std::vector<std::unique_ptr<UndoOperation>> _operations;
};
