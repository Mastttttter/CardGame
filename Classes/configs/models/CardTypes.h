#pragma once

/**
 * @brief Runtime and configuration card behavior types.
 */
enum class CardType {
  /** @brief Standard playing-card behavior. */
  Default
};

/**
 * @brief Runtime location for a card model.
 */
enum class CardZone {
  /** @brief Card is still visible in the playfield. */
  Playfield,
  /** @brief Card is currently the tray/top card. */
  Tray,
  /** @brief Card is waiting in the reserve pile. */
  Reserve,
  /** @brief Card has been removed from active play. */
  Removed
};

/**
 * @brief Stable runtime card identifier.
 */
using CardId = int;

/**
 * @brief Sentinel value used when no valid card identifier is available.
 */
static CardId const INVALID_CARD_ID = -1;
