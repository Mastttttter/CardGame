#pragma once

#include "configs/models/LevelConfig.h"

/**
 * @brief load level config data from .json resources
 */
class LevelConfigLoader {
  public:
  /**
   * @brief loads Resources/configs/level_<id>.json
   * @pragma levelId the id if the level
   * @return LevelConfig the LevelConfig object after parsing or default object
   * if pase failed
   */
  static LevelConfig loadLevelConfig(int levelId);
};
