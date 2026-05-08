#pragma once

#include <string>

/**
 * @brief Provides hard-coded resource paths for card assets and level data.
 */
class CardResConfig {
  public:
  /**
   * @brief Returns the common card background image path.
   *
   * @return Resource path for the shared card background image.
   */
  static std::string const getCardBackgroundPath() {
    return "res/card_general.png";
  }

  /**
   * @brief Returns the folder path for suit images.
   *
   * @return Resource folder path containing suit images.
   */
  static std::string const getSuitFolderPath() {
    return "res/suits/";
  }

  /**
   * @brief Returns the folder path for rank images.
   *
   * @return Resource folder path containing number and rank images.
   */
  static std::string const getNumberFolderPath() {
    return "res/number/";
  }

  /**
   * @brief Returns the bundled level configuration path.
   *
   * @return Resource path for the default level JSON file.
   */
  static std::string const getLevelConfigPath() {
    return "configs/level_1.json";
  }
};
