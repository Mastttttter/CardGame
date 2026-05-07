#pragma once

#include <string>
#include "configs/models/LevelConfig.h"

/** Loads static level configuration through Cocos resource paths. */
class LevelConfigLoader {
  public:
  /** Loads a level JSON file into a LevelConfig. */
  bool load(std::string const &path, LevelConfig *level,
            std::string *error) const;
};
