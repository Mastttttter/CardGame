#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "configs/models/CardConfig.h"
#include "json/document.h"

/** Registry for card-type JSON parsers used by level loading*/
class CardConfigParserRegistry {
  public:
  /** Parser finction for one card JSON object */
  using ParserType = std::function<bool(rapidjson::Value const &, CardConfig *,
                                        std::string *)>;
  /** Creates a registry with the default-card parser installed */
  CardConfigParserRegistry();
  /** Registry a parser for a card type name*/
  void registerParser(std::string const &typeName, ParserType const &parser);
  /** Parses on card object, treating missing types as default*/
  bool parseCard(rapidjson::Value const &value, CardConfig *card,
                 std::string *error) const;

  private:
  std::unordered_map<std::string, ParserType> _parsers;
};
