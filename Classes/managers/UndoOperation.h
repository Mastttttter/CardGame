#pragma once
#include "cocos2d.h"
#include "configs/models/CardTypes.h"

/** Optional visual hint for an undo operation. */
struct UndoAnimation {
  /** Card id to animate after model restoration. */
  CardId cardId = INVALID_CARD_ID;

  /** Target design-space center position for the card. */
  cocos2d::Vec2 targetPosition;
};

/** Polymorphic base for typed undo operations. */
class UndoOperation {
  public:
  /** Destroys the undo operation. */
  virtual ~UndoOperation() {}

  /** Restores the model state captured by this operation. */
  virtual void undo() = 0;

  /** Returns an optional animation hint for the restored state. */
  virtual bool getUndoAnimation(UndoAnimation *animation) const {
    return false;
  }
};
