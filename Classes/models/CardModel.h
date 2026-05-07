#pragma once
#include "configs/models/CardConfig.h"
#include "configs/models/CardTypes.h"

/** runtime card state.*/
class CardModel {
  public:
  /** Creates a runtime card from static card configuration. */
  CardModel(CardId id, CardConfig const &config, CardZone zone,
            int playfieldOrder)
      : _id(id),
        _config(config),
        _zone(zone),
        _playfieldOrder(playfieldOrder),
        _originalPosition(config.position),
        _currentPosition(config.position) {}

  private:
  CardId _id;
  CardConfig _config;
  CardZone _zone;
  int _playfieldOrder;
  cocos2d::Vec2 _originalPosition;
  cocos2d::Vec2 _currentPosition;
};
