#pragma once

#include <vector>
#include "configs/models/CardConfig.h"

/**
 * @brief store level data
 */
class LevelConfig {
  public:
  // cards in playfield
  std::vector<CardConfig> playfieldCards;
  // cards in stack field
  std::vector<CardConfig> stackCards;
};
