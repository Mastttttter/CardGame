#include "CardConfigParserRegistry.h"
#include <sstream>
#include "configs/models/CardConfigDefault.h"
#include "rapidxml/rapidxml.hpp"

namespace details {
bool hasIntMember(rapidjson::Value const &value, char const *name) {
  return value.HasMember(name) && value[name].IsInt();
}

bool hasNumberMember(rapidjson::Value const &value, char const *name) {
  return value.HasMember(name) && value[name].IsNumber();
}

std::shared_ptr<CardConfigBase> parseDefaultCard(
    rapidjson::Value const &value) {
  if (!value.IsObject()) {
    CCLOG("Card entry must be a JSON object");
    return nullptr;
  }
  if (!hasIntMember(value, "CardFace")) {
    CCLOG("Default card requires integer CardFace.");
    return nullptr;
  }
  if (!hasIntMember(value, "CardSuit")) {
    CCLOG("Default card requires integer CardSuit.");
    return nullptr;
  }
  if (!value.HasMember("Position") || !value["Position"].IsObject()) {
    CCLOG("Default card rquires Position object.");
    return nullptr;
  }
  rapidjson::Value const &position = value["Position"];
  if (!hasNumberMember(position, "x") || !hasNumberMember(position, "y")) {
    CCLOG("Defualt card Position requires numeric x and y.");
    return nullptr;
  }
  int const face = value["CardFace"].GetInt();
  if (face < 0 || face > 12) {
    CCLOG("Default card CardFace must be between 0 and 12.");
    return nullptr;
  }
  int const suit = value["CardSuit"].GetInt();
  if (suit < 0 || suit > 3) {
    CCLOG("Default card CardSuit must be between 0 and 3.");
    return nullptr;
  }
  auto card = std::make_shared<CardConfigDefault>();
  card->type = CardType::Default;
  card->face = face;
  card->suit = suit;
  card->position = cocos2d::Vec2(static_cast<float>(position["x"].GetDouble()),
                                 static_cast<float>(position["y"].GetDouble()));
  CCLOG("Parse Success, tyep: %d,face:%d,suit: %d, position: %d %d.",
        card->type, card->face, card->suit, card->position.x, card->position.y);
  return card;
}

std::string readCardType(rapidjson::Value const &value) {
  if (!value.HasMember("type")) {
    return "default";
  }
  if (!value["type"].IsString()) {
    CCLOG("Card type must be a string when present.");
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

std::shared_ptr<CardConfigBase> CardConfigParserRegistry::parseCard(
    rapidjson::Value const &value) const {
  auto const typeName = details::readCardType(value);
  if (typeName.empty()) {
    return nullptr;
  }
  auto parser = _parsers.find(typeName);
  if (parser == _parsers.end()) {
    CCLOG("Unsupport card type: %s", typeName.c_str());
    return nullptr;
  }
  return parser->second(value);
}
