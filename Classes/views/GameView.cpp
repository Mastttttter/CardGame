#include "views/GameView.h"
#include "CCPlatformMacros.h"
#include "configs/LayoutConfig.h"
#include "controllers/GameController.h"
#include "ReservePileView.h"

bool GameView::init() {
  if (!cocos2d::Node::init()) {
    return false;
  }
  setContentSize(LayoutConfig::contentSize());
  cocos2d::LayerColor *background = cocos2d::LayerColor::create(
      LayoutConfig::backgroundColor(), LayoutConfig::contentSize().height,
      LayoutConfig::contentSize().width);
  if (background) {
    background->setPosition(cocos2d::Vec2::ZERO);
    addChild(background, -100);
  }

  _reservePileView = ReservePileView::create();
  if (_reservePileView) {
    _reservePileView->setPosition(LayoutConfig::reservePilePosition());
    _reservePileView->setClickCallback([this]() {
      if (_inputEnabled && _reserveClickCallback) {
        _reserveClickCallback();
      }
    });
    addChild(_reservePileView, 900);
  }

  auto undoLabel =
      cocos2d::Label::createWithTTF("Undo", "fonts/arial.ttf", 46.0f);
  _undoItem =
      cocos2d::MenuItemLabel::create(undoLabel, [this](cocos2d::Ref *sender) {
        if (_inputEnabled && _undoClickCallback) {
          _undoClickCallback();
        }
      });
  if (_undoItem) {
    _undoItem->setPosition(LayoutConfig::undoButtonPosition());
    auto menu = cocos2d::Menu::create(_undoItem, nullptr);
    menu->setPosition(cocos2d::Vec2::ZERO);
    addChild(menu, 1100);
  }
  return true;
}

void GameView::setup(std::shared_ptr<GameModel> model) {
  auto cards = model->getCards();
  for (auto card: cards) {
    createOrConfigureCardView(card);
  }
}

void GameView::createOrConfigureCardView(std::shared_ptr<CardModelBase> card) {
  auto cardView = getCardView(card->getId());
  if (!cardView) {
    cardView = getGameController()
                   ->getCardControllerOfId(card->getId())
                   ->createCardView(card);
    cardView->setClickCallback([this](CardId cardId) {
      if (_inputEnabled && _cardClickCallback) {
        _cardClickCallback(cardId);
      }
    });
    addChild(cardView);
    _cardViews[card->getId()] = cardView;
  }
  cardView->configure(card);
}

CardViewBase *GameView::getCardView(CardId cardId) const {
  auto it = _cardViews.find(cardId);
  if (it == _cardViews.end()) {
    return nullptr;
  }

  return it->second;
}

void GameView::setCardClickCallback(
    std::function<void(CardId)> const &callback) {
  _cardClickCallback = callback;
}

void GameView::setReserveClickCallback(std::function<void()> const &callback) {
  _reserveClickCallback = callback;
}

void GameView::setUndoClickCallback(std::function<void()> const &callback) {
  _undoClickCallback = callback;
}

void GameView::setGameController(GameController *controller) {
  _gameController = controller;
}

GameController *GameView::getGameController() const {
  if (!_gameController) {
    CCLOG("Warning: try to get null GameController");
  }
  return _gameController;
}

void GameView::refresh(std::shared_ptr<GameModel> model,
                       std::unordered_map<CardId, bool> const &clickability,
                       bool canUndo) {
  auto const trayCardId = model->getTrayCardId();
  auto cards = model->getCards();
  for (auto card: cards) {
    auto cardView = getCardView(card->getId());
    if (!cardView) {
      continue;
    }
    bool const isPlayfield = card->getZone() == CardZone::Playfield;
    bool const isTrayTop = card->getId() == trayCardId;
    if (isPlayfield || isTrayTop) {
      cardView->setVisible(true);
      cardView->setPosition(card->getPosition());
      cardView->setLocalZOrder(isPlayfield ? 10 + card->getPlayfieldOrder()
                                           : 1000);
    } else {
      cardView->setVisible(false);
    }
    bool clickable = false;
    auto clickableIt = clickability.find(card->getId());
    if (clickableIt != clickability.end()) {
      clickable = clickableIt->second;
    }
    cardView->setClickable(_inputEnabled && isPlayfield && clickable);
  }

  if (_reservePileView) {
    _reservePileView->setCount(model->getReserveCount());
    _reservePileView->setClickable(_inputEnabled &&
                                   model->getReserveCount() > 0);
  }

  if (_undoItem) {
    _undoItem->setEnabled(_inputEnabled && canUndo);
    _undoItem->setOpacity(canUndo ? 255 : 130);
  }
}
