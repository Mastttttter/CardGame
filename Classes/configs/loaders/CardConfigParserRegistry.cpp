#include "CardConfigParserRegistry.h"
#include <sstream>
#include "rapidxml/rapidxml.hpp"

namespace details {
inline void setError(std::string const &message, std::string *error) {
  if (error) {
    *error = message;
  }
}

bool hasIntMember(rapidjson::Value const &value, char const *name) {
  return value.HasMember(name) && value[name].IsInt();
}

bool hasNumberMember(rapidjson::Value const &value, char const *name) {
  return value.HasMember(name) && value[name].IsNumber();
}

bool parseDefaultCard(rapidjson::Value const &value, CardConfig *card,
                      std::string *error) {
  if (!value.IsObject()) {
    setError("Card entry must be a JSON object", error);
    return false;
  }
  if (!hasIntMember(value, "CardFace")) {
    setError("Default card requires integer CardFace.", error);
    return false;
  }
  if (!hasIntMember(value, "CardSuit")) {
    setError("Default card requires integer CardSuit.", error);
    return false;
  }
  if (!value.HasMember("Position") || !value["Position"].IsObject()) {
    setError("Default card rquires Position object.", error);
    return false;
  }
  rapidjson::Value const &position = value["Position"];
  if (!hasNumberMember(position, "x") || !hasNumberMember(position, "y")) {
    setError("Defualt card Position requires numeric x and y.", error);
    return false;
  }
  int const face = value["CardFace"].GetInt();
  if (face < 0 || face > 12) {
    setError("Default card CardFace must be between 0 and 12.", error);
    return false;
  }
  int const suit = value["CardSuit"].GetInt();
  if (suit < 0 || suit > 3) {
    setError("Default card CardSuit must be between 0 and 3.", error);
    return false;
  }
  card->type = CardType::Default;
  card->face = face;
  card->suit = suit;
  card->position = cocos2d::Vec2(static_cast<float>(position["x"].GetDouble()),
                                 static_cast<float>(position["y"].GetDouble()));
  cocos2d::log("Parse Success, tyep: %d,face:%d,suit: %d, position: %d %d.",
               card->type, card->face, card->suit, card->position.x,
               card->position.y);
  return true;
}

std::string readCardType(rapidjson::Value const &value, std::string *error) {
  if (!value.HasMember("type")) {
    return "default";
  }
  if (!value["type"].IsString()) {
    setError("Card type must be a string when present.", error);
    return std::string();
  }
  std::string const typeName = value["type"].GetString();
  if (typeName == "Default") {
    return "default";
  }
  return typeName;
}
}  // namespace details

CardConfigParserRegistry::CardConfigParserRegistry() {
  registerParser("default", details::parseDefaultCard);
}

void CardConfigParserRegistry::registerParser(std::string const &typeName,
                                              ParserType const &parser) {
  _parsers[typeName] = parser;
}

bool CardConfigParserRegistry::parseCard(rapidjson::Value const &value,
                                         CardConfig *card,
                                         std::string *error) const {
  auto const typeName = details::readCardType(value, error);
  if (typeName.empty()) {
    return false;
  }
  auto parser = _parsers.find(typeName);
  if (parser == _parsers.end()) {
    std::ostringstream stream;
    stream << "Unsupport card type:" << typeName;
    details::setError(stream.str(), error);
    return false;
  }
  return parser->second(value, card, error);
}
