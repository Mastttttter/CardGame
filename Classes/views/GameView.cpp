#include "views/GameView.h"
#include "configs/LayoutConfig.h"

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

  // TODO create _reservePileView

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
