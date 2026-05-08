#pragma once

#include <vector>
#include "CardConfigBase.h"

/**
 * @brief Static level data loaded from a JSON level file.
 */
struct LevelConfig {
  /** @brief Cards rendered in playfield order. */
  std::vector<std::shared_ptr<CardConfigBase>> playfieldCards;

  /** @brief Stack cards where index 0 is the deepest reserve card. */
  std::vector<std::shared_ptr<CardConfigBase>> stackCards;
};
