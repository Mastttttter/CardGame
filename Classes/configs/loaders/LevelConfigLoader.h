#pragma once

#include <string>
#include "configs/models/LevelConfig.h"

/** Loads static level configuration through Cocos resource paths. */
class LevelConfigLoader {
  public:
  /** Loads a level JSON file into a LevelConfig. */
  std::shared_ptr<LevelConfig> load(std::string const &path) const;
};
