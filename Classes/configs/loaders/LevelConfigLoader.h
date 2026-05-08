#pragma once

#include <string>
#include "configs/models/LevelConfig.h"

/**
 * @brief Loads static level configuration through Cocos resource paths.
 */
class LevelConfigLoader {
  public:
  /**
   * @brief Loads a level JSON file into a LevelConfig object.
   *
   * @param path Cocos resource path for the level JSON file.
   * @return Parsed level configuration, or nullptr when loading or parsing fails.
   */
  static std::shared_ptr<LevelConfig> load(std::string const &path);
};
