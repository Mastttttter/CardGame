#include "configs/loaders/LevelConfigLoader.h"

namespace UnitTest {
void testLoad() {
  LevelConfigLoader loader;
  LevelConfig config;
  std::string error;
  loader.load("configs/level_1.json", &config, &error);
  cocos2d::log("%s", error.c_str());
}
}  // namespace UnitTest
