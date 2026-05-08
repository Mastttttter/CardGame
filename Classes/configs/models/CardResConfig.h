#pragma once

#include <string>

/**
 * @brief hard-code card asset path data
 */
class CardResConfig {
  public:
  // returns the card background image path
  static std::string const getCardBackgroundPath() {
    return "res/card_general.png";
  }

  // returns the suit image folder path
  static std::string const getSuitFolderPath() {
    return "res/suits/";
  }

  // returns the number image folder path
  static std::string const getNumberFolderPath() {
    return "res/number/";
  }

  static std::string const getLevelConfigPath() {
    return "configs/level_1.json";
  }
};
