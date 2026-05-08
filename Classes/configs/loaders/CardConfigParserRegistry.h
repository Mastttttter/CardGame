#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "configs/models/CardConfigBase.h"
#include "json/document.h"

/**
 * @brief Registry of JSON parsers used to build card configuration objects.
 */
class CardConfigParserRegistry {
  public:
  /**
   * @brief Parser callback for one card JSON object.
   *
   * @param value JSON value that describes a card.
   * @return Parsed card configuration, or nullptr when validation fails.
   */
  using ParserType =
      std::function<std::shared_ptr<CardConfigBase>(rapidjson::Value const &)>;

  /**
   * @brief Creates a registry with the default-card parser installed.
   */
  CardConfigParserRegistry();

  /**
   * @brief Registers a parser for a card type name.
   *
   * @param typeName Type name read from level JSON data.
   * @param parser Parser callback used for that type.
   */
  void registerParser(std::string const &typeName, ParserType const &parser);

  /**
   * @brief Parses one card object, treating missing type names as default cards.
   *
   * @param value JSON value that describes a card.
   * @return Parsed card configuration, or nullptr when no parser can handle it.
   */
  std::shared_ptr<CardConfigBase> parseCard(
      rapidjson::Value const &value) const;

  private:
  /** @brief Parser callbacks keyed by card type name. */
  std::unordered_map<std::string, ParserType> _parsers;
};
