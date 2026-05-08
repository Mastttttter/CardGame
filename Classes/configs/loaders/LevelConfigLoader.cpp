#include "configs/loaders/LevelConfigLoader.h"
#include <sstream>
#include "base/CCConsole.h"
#include "cocos2d.h"
#include "configs/loaders/CardConfigParserRegistry.h"
#include "json/document.h"

// the helper fundtions used by this .cpp file, no recommand for other usage
namespace details {
std::vector<std::shared_ptr<CardConfigBase>> parseCardArray(
    rapidjson::Value const &root, char const *key, bool playfield,
    CardConfigParserRegistry const &registry) {
  if (!root.HasMember(key) || !root[key].IsArray()) {
    CCLOG("Level config requires array %s .", key);
    return {};
  }
  rapidjson::Value const &array = root[key];
  std::vector<std::shared_ptr<CardConfigBase>> cards;
  for (rapidjson::SizeType index = 0; index < array.Size(); ++index) {
    auto card = registry.parseCard(array[index]);
    if (!card) {
      return {};
    }
    card->playfieldOrder = playfield ? static_cast<int>(index) : -1;
    cards.push_back(card);
  }
  cocos2d::log("Parsing %s success", key);
  return cards;
}
}  // namespace details

std::shared_ptr<LevelConfig> LevelConfigLoader::load(std::string const &path) {
  cocos2d::log("loading level %s ", path.c_str());
  auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
  if (json.empty()) {
    CCLOG("failed to read level config: %s", path.c_str());
    return nullptr;
  }
  rapidjson::Document document;
  document.Parse<0>(json.c_str());
  if (document.HasParseError() || !document.IsObject()) {
    CCLOG("Level config invalid: %s", path.c_str());
    return nullptr;
  }
  auto parsed = std::make_shared<LevelConfig>();
  CardConfigParserRegistry registry;
  parsed->playfieldCards =
      details::parseCardArray(document, "Playfield", true, registry);
  if (parsed->playfieldCards.empty()) {
    return nullptr;
  }
  parsed->stackCards =
      details::parseCardArray(document, "Stack", true, registry);
  if (parsed->stackCards.empty()) {
    return nullptr;
  }
  cocos2d::log("load level %s success", path.c_str());
  return parsed;
}
