#pragma once
#include "configs/models/CardConfigBase.h"
#include "configs/models/CardTypes.h"

/**
 * @brief Runtime state shared by every card model.
 */
class CardModelBase {
  public:
  /**
   * @brief Creates a runtime card from static card configuration.
   *
   * @param config Static configuration used to initialize identity-independent state.
   */
  CardModelBase(std::shared_ptr<CardConfigBase> config)
      : _id(-1),
        _config(config),
        _zone(),
        _playfieldOrder(-1),
        _originalPosition(config->position),
        _currentPosition(config->position) {
    CCLOG("Success: generate a model of position :%f, %f", _originalPosition.x,
          _originalPosition.y);
  }

  /** @brief Alias for the base model type used by subclasses. */
  using Super = CardModelBase;

  /**
   * @brief Destroys the runtime card through the base type.
   */
  virtual ~CardModelBase() = default;

  /**
   * @brief Returns the stable runtime card id.
   *
   * @return Card identifier assigned by GameModel.
   */
  CardId getId() const {
    return _id;
  }

  /**
   * @brief Updates the stable runtime card id.
   *
   * @param id Card identifier assigned by GameModel.
   */
  void setId(CardId id) {
    _id = id;
  }

  /**
   * @brief Returns the static source configuration for this card.
   *
   * @return Shared card configuration object.
   */
  std::shared_ptr<CardConfigBase> getConfig() const {
    return _config;
  }

  /**
   * @brief Returns the configured behavior type.
   *
   * @return Card type used to select a card controller and view.
   */
  CardType getType() const {
    return _config->type;
  }

  /**
   * @brief Returns the current runtime zone.
   *
   * @return Current location of the card in game state.
   */
  CardZone getZone() const {
    return _zone;
  }

  /**
   * @brief Updates the current runtime zone.
   *
   * @param zone New card zone.
   */
  void setZone(CardZone zone) {
    _zone = zone;
  }

  /**
   * @brief Updates the original playfield order.
   *
   * @param order Playfield order, or -1 for stack cards.
   */
  void setPlayfieldOrder(int order) {
    _playfieldOrder = order;
  }

  /**
   * @brief Returns the original level position for this card.
   *
   * @return Design-space center position before gameplay movement.
   */
  cocos2d::Vec2 const &getOriginalPosition() const {
    return _originalPosition;
  }

  /**
   * @brief Updates the original level position for this card.
   *
   * @param position Design-space center position to restore during undo.
   */
  void setOriginalPosition(cocos2d::Vec2 const &position) {
    _originalPosition = position;
  }

  /**
   * @brief Returns the current design-space center position.
   *
   * @return Current position used by the view layer.
   */
  cocos2d::Vec2 const &getPosition() const {
    return _currentPosition;
  }

  /**
   * @brief Updates the current design-space center position.
   *
   * @param position Current position used by the view layer.
   */
  void setPosition(cocos2d::Vec2 const &position) {
    _currentPosition = position;
  }

  /**
   * @brief Returns the original playfield order, or -1 for stack cards.
   *
   * @return Original ordering value assigned during level setup.
   */
  int getPlayfieldOrder() const {
    return _playfieldOrder;
  }

  private:
  /** @brief Stable runtime id assigned by GameModel. */
  CardId _id;

  /** @brief Static configuration used to create this runtime card. */
  std::shared_ptr<CardConfigBase> _config;

  /** @brief Current zone that controls visibility and interactions. */
  CardZone _zone;

  /** @brief Original playfield order, or -1 for stack cards. */
  int _playfieldOrder;

  /** @brief Position restored when a playfield card is undone. */
  cocos2d::Vec2 _originalPosition;

  /** @brief Current design-space position mirrored by the view. */
  cocos2d::Vec2 _currentPosition;
};
