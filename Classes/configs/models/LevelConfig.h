#pragma once

#include <vector>
#include "CardConfigBase.h"

/** Static level data loaded from a JSON level file. */
struct LevelConfig {
  /** Cards rendered in playfield order. */
  std::vector<std::shared_ptr<CardConfigBase>> playfieldCards;

  /** Stack cards where index 0 is the initial tray/top card. */
  std::vector<std::shared_ptr<CardConfigBase>> stackCards;
};
