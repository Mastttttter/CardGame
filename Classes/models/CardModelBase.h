#pragma once
#include "configs/models/CardConfigBase.h"
#include "configs/models/CardTypes.h"

/** runtime card state.*/
class CardModelBase {
  public:
  /** Creates a runtime card from static card configuration. */
  CardModelBase(CardId id, std::shared_ptr<CardConfigBase> const &config,
                CardZone zone, int playfieldOrder)
      : _id(id),
        _config(config),
        _zone(zone),
        _playfieldOrder(playfieldOrder),
        _originalPosition(config->position),
        _currentPosition(config->position) {}

  virtual ~CardModelBase() = default;

  /** Returns the stable runtime card id. */
  CardId getId() const {
    return _id;
  }

  /** Returns the static source configuration for this card. */
  std::shared_ptr<CardConfigBase> const &getConfig() const {
    return _config;
  }

  /** Returns the configured behavior type. */
  CardType getType() const {
    return _config->type;
  }

  /** Returns the current runtime zone. */
  CardZone getZone() const {
    return _zone;
  }

  /** Updates the current runtime zone. */
  void setZone(CardZone zone) {
    _zone = zone;
  }

  /** Returns the original level position for this card. */
  cocos2d::Vec2 const &getOriginalPosition() const {
    return _originalPosition;
  }

  /** Returns the current design-space center position. */
  cocos2d::Vec2 const &getPosition() const {
    return _currentPosition;
  }

  /** Updates the current design-space center position. */
  void setPosition(cocos2d::Vec2 const &position) {
    _currentPosition = position;
  }

  /** Returns the original playfield order, or -1 for stack cards. */
  int getPlayfieldOrder() const {
    return _playfieldOrder;
  }

  private:
  CardId _id;
  std::shared_ptr<CardConfigBase> _config;
  CardZone _zone;
  int _playfieldOrder;
  cocos2d::Vec2 _originalPosition;
  cocos2d::Vec2 _currentPosition;
};
