#include "configs/loaders/LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include "physics/CCPhysicsHelper.h"

USING_NS_CC;

// the helper fundtions used by this .cpp file, no recommand for other usage
namespace details {
/**
 * @brief parse the Card config from json block
 * the json block should be
 * {
 *             "CardFace": 12,
 *             "CardSuit": 0,
 *             "Position": {"x": 250, "y": 1000}
 *         },
 * @param value card json blodk
 * @return CardConfig, if the json is invalid, it will return the default object
 */
CardConfig parseCardConfig(rapidjson::Value const &value) {
  CardConfig config;
  if (!value.IsObject()) {
    return config;
  }
  if (value.HasMember("CardFace") && value["CardFace"].IsInt()) {
    config.face = static_cast<CardFaceType>(value["CardFace"].GetInt());
  }
  if (value.HasMember("CardSuit") && value["CardSuit"].IsInt()) {
    config.suit = static_cast<CardSuitType>(value["CardSuit"].GetInt());
  }
  if (value.HasMember("Position") && value["Position"].IsObject()) {
    auto const &position = value["Position"];
    if (position.HasMember("x") && position.HasMember("y") &&
        position["x"].IsNumber() && position["y"].IsNumber()) {
      config.position =
          Vec2(position["x"].GetFloat(), position["y"].GetFloat());
    }
  }
  return config;
}

/**
 * @brief load the card array from json block
 *   the json block should be
 *   {
 *   "<arrayName>": [
 *         {
 *             "CardFace": 12,
 *             "CardSuit": 0,
 *             "Position": {"x": 250, "y": 1000}
 *         },
 *       ...
 *       ],
 *    }
 * @param document cards json block
 *         arrayName the name of the list of cards
 * @return std::vector<CardConfig> the CardConfig array or empty if the json
 * block is invalid
 */
std::vector<CardConfig> loadCardArray(rapidjson::Document const &document,
                                      std::string const &arrayName) {
  std::vector<CardConfig> cardArray;
  if (!document.HasMember(arrayName.c_str()) ||
      !document[arrayName.c_str()].IsArray()) {
    return cardArray;
  }
  auto const &jsonArray = document[arrayName.c_str()];
  for (rapidjson::SizeType i = 0; i < jsonArray.Size(); ++i) {
    cardArray.emplace_back(parseCardConfig(jsonArray[i]));
  }
  return cardArray;
}
}  // namespace details

LevelConfig LevelConfigLoader::loadLevelConfig(int levelId) {
  LevelConfig levelConfig;
  std::stringstream path;
  path << "configs/level_" << levelId << ".json";
  std::string const content =
      FileUtils::getInstance()->getStringFromFile(path.str());
  if (content.empty()) {
    CCLOG("Failed to load level config: %s", path.str().c_str());
    return levelConfig;
  }
  rapidjson::Document document;
  document.Parse<0>(content.c_str());
  if (document.HasParseError() || !document.IsObject()) {
    CCLOG("Failed to parse level config: %s", path.str().c_str());
    return levelConfig;
  }
  levelConfig.playfieldCards = details::loadCardArray(document, "Playfield");
  levelConfig.stackCards = details::loadCardArray(document, "Stac");
  return levelConfig;
}
