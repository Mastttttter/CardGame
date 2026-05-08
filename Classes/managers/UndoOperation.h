#pragma once
#include "cocos2d.h"
#include "configs/models/CardTypes.h"

/**
 * @brief Optional visual hint for an undo operation.
 */
struct UndoAnimation {
  /** @brief Card id to animate after model restoration. */
  CardId cardId = INVALID_CARD_ID;

  /** @brief Target design-space center position for the card. */
  cocos2d::Vec2 targetPosition;
};

/**
 * @brief Polymorphic base interface for typed undo operations.
 */
class UndoOperation {
  public:
  /**
   * @brief Destroys the undo operation through the base type.
   */
  virtual ~UndoOperation() {}

  /**
   * @brief Restores the model state captured by this operation.
   */
  virtual void undo() = 0;

  /**
   * @brief Returns optional animation metadata for the restored state.
   *
   * @param animation Output animation metadata to fill when available.
   * @return True when animation metadata was written.
   */
  virtual bool getUndoAnimation(UndoAnimation *animation) const {
    return false;
  }
};
