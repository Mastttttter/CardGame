#pragma once

/** Runtime and config card behavior types. */
enum class CardType {
  Default
};

/** Runtime location for a card model. */
enum class CardZone {
  Playfield,
  Tray,
  Reserve,
  Removed
};

/** Stable runtime card identifier. */
using CardId = int;

/** Invalid card identifier sentinel. */
static CardId const INVALID_CARD_ID = -1;
