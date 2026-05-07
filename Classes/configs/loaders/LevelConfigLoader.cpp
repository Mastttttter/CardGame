#include "configs/loaders/LevelConfigLoader.h"
#include <sstream>
#include "base/CCConsole.h"
#include "cocos2d.h"
#include "configs/loaders/CardConfigParserRegistry.h"
#include "json/document.h"

// the helper fundtions used by this .cpp file, no recommand for other usage
namespace details {
inline void setError(std::string const &message, std::string *error) {
  if (error) {
    *error = message;
  }
}

bool parseCardArray(rapidjson::Value const &root, char const *key,
                    bool playfield, CardConfigParserRegistry const &registry,
                    std::vector<CardConfig> *cards, std::string *error) {
  if (!root.HasMember(key) || !root[key].IsArray()) {
    std::ostringstream stream;
    stream << "Level config requires array" << key << ".";
    setError(stream.str(), error);
    return false;
  }
  rapidjson::Value const &array = root[key];
  for (rapidjson::SizeType index = 0; index < array.Size(); ++index) {
    CardConfig card;
    std::string cardError;
    if (!registry.parseCard(array[index], &card, &cardError)) {
      std::ostringstream stream;
      stream << key << "[" << index << "]:" << cardError;
      setError(stream.str(), error);
      return false;
    }
    card.playfieldOrder = playfield ? static_cast<int>(index) : -1;
    cards->push_back(card);
  }
  cocos2d::log("Parsing %s success", key);
  return true;
}
}  // namespace details

bool LevelConfigLoader::load(std::string const &path, LevelConfig *level,
                             std::string *error) const {
  cocos2d::log("loading level %s ", path.c_str());
  auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
  if (json.empty()) {
    details::setError("failed to read level config: " + path, error);
    return false;
  }
  rapidjson::Document document;
  document.Parse<0>(json.c_str());
  if (document.HasParseError() || !document.IsObject()) {
    details::setError("Level config invalid" + path, error);
    return false;
  }
  LevelConfig parsed;
  CardConfigParserRegistry registry;
  if (!details::parseCardArray(document, "Playfield", true, registry,
                               &parsed.playfieldCards, error)) {
    return false;
  }
  if (!details::parseCardArray(document, "Stack", false, registry,
                               &parsed.playfieldCards, error)) {
    return false;
  }
  *level = parsed;
  cocos2d::log("load level %s success", path.c_str());
  return true;
}
