#pragma once

#include <vector>
#include "CardConfig.h"

/** Static level data loaded from a JSON level file. */
struct LevelConfig {
  /** Cards rendered in playfield order. */
  std::vector<CardConfig> playfieldCards;

  /** Stack cards where index 0 is the initial tray/top card. */
  std::vector<CardConfig> stackCards;
};
