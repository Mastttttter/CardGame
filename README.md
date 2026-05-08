# CardGame

**AI工具使用声明：我使用ClaudeCode辅助完善部分类的编写，项目注释，本文档的编写和后期code review。架构设计以及核心逻辑具体实现由人类古法工艺纯手工打造**

> 开发环境：环境：archlinux kernel:7.0.3-arch-1 gcc 16.1 cmake 4.3.2 源码编译cocos2d-x-3.17-oh分支

## 1. 项目概述

CardGame 是一个基于 cocos2d-x 的纸牌游戏项目。当前实现的核心玩法是：

- 从 JSON 关卡配置加载桌面牌区和备用牌堆。
- 将备用牌堆最后一张初始化为顶部牌，其余备用牌等待抽取。
- 玩家点击桌面牌时，只有“未被遮挡”且能与顶部牌匹配的牌可以移动到顶部牌位置。
- 玩家点击备用牌堆时，从备用牌堆抽一张牌替换当前顶部牌。
- 玩家点击 Undo 时，回退最近一次“桌面牌移动”或“备用牌抽取”。

项目整体使用近似 MVC 的分层方式：

- Model 保存运行时牌状态、区域、位置、顶部牌和备用牌顺序。
- View 负责 cocos2d-x 节点、触摸输入、显示刷新和移动动画。
- Controller 负责启动流程、配置转模型、点击规则、动作执行和回退栈管理。
- Config / Loader 负责从关卡 JSON 生成静态卡牌配置。
- Manager 负责回退栈和桌面牌遮挡关系。

## 2. 目录与模块职责

| 路径 | 职责 |
| --- | --- |
| `Classes/AppDelegate.cpp` | cocos2d-x 应用启动入口，创建并运行 `GameScene`。 |
| `Classes/views/GameScene.cpp` | 游戏根场景，创建 `GameView` 和 `GameController`，并调用 `GameController::start()`。 |
| `Classes/controllers/GameController.h/.cpp` | 游戏主控制器，串联 model、view、配置加载、卡牌控制器、遮挡管理、回退管理和输入回调。 |
| `Classes/models/GameModel.h/.cpp` | 运行时游戏状态，保存所有牌、桌面牌列表、备用牌列表、当前顶部牌和卡牌 id 映射。 |
| `Classes/views/GameView.h/.cpp` | 游戏 UI 根节点，创建牌视图、备用牌堆、Undo 按钮，负责刷新可见性、点击状态和动画。 |
| `Classes/views/CardViewBase.h/.cpp` | 卡牌视图基类，提供触摸监听、点击开关、透明度反馈和卡牌 id 绑定。 |
| `Classes/views/CardViewDefault.h/.cpp` | Default 卡牌视图，拼接并显示牌面背景、数字和花色资源。 |
| `Classes/controllers/CardControllerBase.h` | 卡牌类型控制器基类，定义创建 model/view、点击可行性判断和动作执行接口。 |
| `Classes/controllers/CardControllerDefault.h/.cpp` | Default 卡牌规则实现：生成默认牌模型/视图、判断牌面是否匹配、移动到顶部牌并生成回退操作。 |
| `Classes/controllers/StackController.h/.cpp` | 备用牌堆控制器，负责从备用牌堆抽牌到顶部牌并生成回退操作。 |
| `Classes/managers/UndoOperation.h` | 回退操作接口和可选动画数据结构。 |
| `Classes/managers/UndoManager.h/.cpp` | LIFO 回退栈，保存并执行 `UndoOperation`。 |
| `Classes/managers/CardManager.h/.cpp` | 桌面牌遮挡关系管理，根据依赖度判断某张牌是否在最上层。 |
| `Classes/configs/models/*` | 配置数据结构，例如卡牌类型、基础配置、Default 配置、关卡配置和资源路径配置。 |
| `Classes/configs/loaders/*` | 关卡 JSON 加载器和卡牌配置解析注册表。 |
| `Classes/configs/LayoutConfig.*` | 设计分辨率内的布局常量，例如牌尺寸、备用牌堆位置、顶部牌位置、Undo 按钮位置和动画时长。 |
| `Classes/utils/CardAssetUtils.*` | 根据牌面和花色生成 Default 卡牌资源路径。 |
| `Resources/configs/level_1.json` | 当前默认关卡配置。 |
| `CMakeLists.txt` | cocos2d-x 跨平台构建入口，新 C++ 文件需要在这里注册。 |

## 3. 启动流程与整体架构

### 3.1 启动链路

```text
AppDelegate::applicationDidFinishLaunching
  -> GameScene::createScene
  -> GameScene::init
  -> GameView::create
  -> new GameController(GameView*)
  -> GameController::start
```

`GameController::start()` 是游戏进入可交互状态的核心入口，执行顺序如下：

```text
GameController::start
  -> 创建 StackController
  -> registerCardController()
  -> initGameModel()
  -> postInitGameModel()
  -> 给 GameView 绑定卡牌、备用牌堆、Undo 回调
  -> GameView::setup(model)
  -> _refreshView()
```

启动后的核心对象关系：

```text
GameController
  -> GameModel
  -> GameView
  -> StackController
  -> CardManager
  -> UndoManager
  -> unordered_map<CardType, CardControllerBase>
       -> CardControllerDefault
```

### 3.2 主架构链路

```text
AppDelegate
  -> GameScene
  -> GameController
       -> GameModel
       -> GameView
       -> Managers
          -> CardManager
          -> UndoManager
       -> Controllers
          -> StackController
          -> CardControllerDefault
```

`GameController` 不直接写具体牌面的表现细节，而是通过 `CardType` 找到对应的 `CardControllerBase` 子类。这样新增卡牌类型时，可以把新牌的配置、模型、视图和规则收敛到一组独立类中。

## 4. 配置加载与卡牌生成流程

### 4.1 关卡配置链路

```text
Resources/configs/level_1.json
  -> CardResConfig::getLevelConfigPath()
  -> LevelConfigLoader::load(path)
  -> rapidjson::Document
  -> CardConfigParserRegistry
  -> CardConfigDefault
  -> LevelConfig
  -> GameController::initGameModel()
  -> GameModel::addCard(...)
  -> GameView::setup(model)
```

默认关卡路径由 `Classes/configs/models/CardResConfig.h` 返回：

```text
configs/level_1.json
```

`LevelConfigLoader::load()` 会读取 JSON，并分别解析两个数组：

- `Playfield`：桌面牌区。
- `Stack`：备用牌堆和初始顶部牌。

### 4.2 当前 JSON 卡牌字段

当前 Default 卡牌支持以下字段：

```json
{
  "type": "Default",
  "CardFace": 12,
  "CardSuit": 0,
  "Position": {"x": 250, "y": 1000}
}
```

字段说明：

| 字段 | 说明 |
| --- | --- |
| `type` | 可选字段。缺省时按 Default 解析；值为 `Default` 时也会映射到默认解析器。 |
| `CardFace` | 牌面，`0` 表示 A，`12` 表示 K。 |
| `CardSuit` | 花色，`0` club，`1` diamond，`2` heart，`3` spade。 |
| `Position` | 关卡配置中的设计空间坐标。Default 模型会在 y 方向额外叠加 `LayoutConfig::playfieldOffsetFromBottom()`。 |

`CardConfigParserRegistry` 当前内置 `default` 解析器，负责校验 `CardFace`、`CardSuit` 和 `Position`，并生成 `CardConfigDefault`。

### 4.3 桌面牌、备用牌和顶部牌生成

`GameController::initGameModel()` 分两段创建运行时卡牌：

1. 遍历 `level->playfieldCards`：
   - 按配置顺序生成 model。
   - 初始区域为 `CardZone::Playfield`。
   - 加入 `GameModel::_playfieldCardIds`。
2. 遍历 `level->stackCards`：
   - 除最后一张外，初始区域为 `CardZone::Reserve`，位置设为 `LayoutConfig::reservePilePosition()`。
   - 最后一张作为初始顶部牌，区域为 `CardZone::Tray`，位置设为 `LayoutConfig::trayPosition()`。
   - 备用牌加入 `GameModel::_reserveCardIds`，顶部牌 id 写入 `GameModel::_trayCardId`。

运行时状态集中在 `GameModel`：

```text
_cards          所有运行时卡牌
_cardsMap       CardId -> CardModelBase
_playfieldCardIds
_reserveCardIds
_trayCardId
_nextCardId
```

## 5. MVC / 卡牌类型扩展架构

每一种卡牌类型建议都按四层组织：

```text
CardConfigXxx
  -> CardModelXxx
  -> CardViewXxx
  -> CardControllerXxx
```

各层职责如下：

| 层 | 基类 / 位置 | 职责 |
| --- | --- | --- |
| Config | `CardConfigBase` | 保存从 JSON 读取的静态配置。 |
| Model | `CardModelBase` | 保存运行时状态，例如 id、区域、顺序、当前位置、原始位置。 |
| View | `CardViewBase` | 负责 cocos2d-x 节点表现、触摸输入和可点击状态反馈。 |
| Controller | `CardControllerBase` | 负责配置转模型、模型转视图、点击规则、动作执行和回退对象创建。 |

核心扩展点在 `CardControllerBase`：

- `generateCardModelFromConfig()`：把静态配置转为运行时模型。
- `createCardView()`：为运行时模型创建对应视图。
- `checkIfClickable()`：判断该类型卡牌当前是否允许点击。
- `doCardAction()`：执行正向动作，并返回可回退的 `UndoOperation`。

`GameController` 通过 `registerCardController()` 建立 `CardType -> CardControllerBase` 的映射。加载关卡和处理点击时，都通过这张表找到对应类型控制器。

## 6. 基础功能实现流程

### 6.1 备用牌堆顶部牌替换

用户点击备用牌堆时，链路如下：

```text
点击 ReservePileView
  -> GameView::_reserveClickCallback
  -> GameController::_handleReserveClickk
  -> StackController::drawReserveCard
  -> GameModel::drawReserveCardToTray
  -> UndoManager::push(ReserveDrawUndoOperation)
  -> GameView::showCardAtPosition(reservePosition)
  -> GameView::animateCardToPosition(trayPosition)
  -> GameController::_refreshView
```

状态变化：

1. `StackController::drawReserveCard()` 先记录当前 `previousTrayId`。
2. `GameModel::drawReserveCardToTray()` 从 `_reserveCardIds.back()` 取出一张牌，并从备用牌列表移除。
3. 被抽出的牌区域改为 `CardZone::Tray`，位置改为顶部牌位置。
4. `_trayCardId` 更新为新抽出的牌。
5. `StackController` 返回 `ReserveDrawUndoOperation`，用于之后撤销本次抽牌。
6. `GameController` 暂停输入，先把牌显示在备用牌堆位置，再播放移动到顶部牌位置的动画，动画完成后恢复输入并刷新视图。

视图刷新时，`GameView::refresh()` 只显示：

- 仍在 `CardZone::Playfield` 的桌面牌。
- 当前 `trayCardId` 对应的顶部牌。

备用牌和旧顶部牌不会作为可点击桌面牌显示。

### 6.2 桌面牌与顶部牌匹配

用户点击桌面牌时，链路如下：

```text
点击 CardViewBase
  -> GameView::_cardClickCallback(cardId)
  -> GameController::_handleCardClick(cardId)
  -> CardManager::isOnTop(cardId)
  -> CardControllerDefault::checkIfClickable(cardId)
  -> CardControllerDefault::doCardAction(cardId)
  -> GameModel::moveCardToTray(cardId)
  -> UndoManager::push(DefaultMoveUndoOperation)
  -> GameView::animateCardToPosition(trayPosition)
  -> GameController::_refreshView
```

`GameController::_handleCardClick()` 会先做通用检查：

1. 游戏已经启动。
2. 能通过 `GameModel::findCard(cardId)` 找到该牌。
3. 能通过 `getCardControllerOfId(cardId)` 找到该牌类型控制器。
4. `CardManager::isOnTop(cardId)` 返回 true，表示该牌没有被其他桌面牌压住。
5. 类型控制器的 `checkIfClickable(cardId)` 返回 true。

Default 卡牌的匹配规则在 `CardControllerDefault::checkIfClickable()`：

```text
abs(当前顶部牌 face - 被点击桌面牌 face) == 1
```

当前实现只比较牌面大小，不比较花色，也没有 A/K 首尾相接规则。

匹配成功后，`CardControllerDefault::doCardAction()` 会：

1. 记录移动前的 `previousTrayId`。
2. 记录被点击牌的 `originalPosition`。
3. 调用 `GameModel::moveCardToTray(cardId, trayPosition)`。
4. 返回 `DefaultMoveUndoOperation`，用于把该牌恢复到桌面牌区。

### 6.3 回退功能

Undo 按钮链路如下：

```text
点击 Undo
  -> GameView::_undoClickCallback
  -> GameController::_handleUndoClick
  -> UndoManager::undo(&animation)
  -> UndoOperation::getUndoAnimation(&animation)
  -> UndoOperation::undo()
  -> GameView::animateCardToPosition(animation.targetPosition)
  -> GameController::_refreshView
```

`UndoManager` 使用后进先出顺序：最近一次动作最先回退。

当前已有两类回退：

| 操作 | 回退类 | 恢复内容 | 动画目标 |
| --- | --- | --- | --- |
| 桌面牌移动到顶部牌 | `DefaultMoveUndoOperation` | 调用 `GameModel::restoreCardToPlayfield(cardId, previousTrayId)`，恢复牌区、位置和旧顶部牌 id。 | 被移动牌的原始桌面位置。 |
| 备用牌抽到顶部牌 | `ReserveDrawUndoOperation` | 调用 `GameModel::returnCardToReserveFront(cardId, previousTrayId, reservePosition)`，把牌放回备用牌列表前端并恢复旧顶部牌 id。 | 备用牌堆位置。 |

`UndoOperation::getUndoAnimation()` 是可选接口。如果某个回退没有动画需求，可以保持默认返回 false，`GameController::_handleUndoClick()` 会直接刷新视图。

### 6.4 可点击性判定

可点击性在 `GameController::_buildClickability()` 中统一生成：

```text
每张 playfield card:
  card 存在
  && card->getZone() == CardZone::Playfield
  && CardManager::isOnTop(cardId)
  && 对应 CardController::checkIfClickable(cardId)
```

结果传给 `GameView::refresh()`。视图层还会叠加 `_inputEnabled` 和区域判断：

```text
cardView->setClickable(_inputEnabled && isPlayfield && clickable)
```

因此一张牌真正能响应点击，需要同时满足：

1. 游戏当前允许输入。
2. 该牌仍在桌面牌区。
3. 该牌未被上层牌遮挡。
4. 该牌类型自己的规则允许点击。

### 6.5 遮挡关系构建

遮挡关系在 `GameController::postInitGameModel()` 中启动后一次性构建：

```text
遍历 playfieldCards 中每一对牌
  -> 用 GeometryUtils::centeredRect(...) 生成矩形
  -> 若两个矩形有正面积重叠
  -> playfieldOrder 更大的牌压住更小的牌
  -> CardManager::addConnection(上层牌, 下层牌)
CardManager::constructGraph()
```

`CardManager` 内部维护：

- `_connections`：原始“上层牌 -> 下层牌”关系。
- `_originCardGraph`：邻接表。
- `_degree`：每张牌当前还有多少张上层牌压着它。

`CardManager::isOnTop(cardId)` 判断 `_degree[cardId] == 0`。

## 7. Default 模块设计说明

Default 是当前项目唯一已注册的卡牌类型，也可以作为新增类型的模板。

### 7.1 `CardConfigDefault`

位置：`Classes/configs/models/CardConfigDefault.h`

职责：保存静态配置。

关键字段：

- `type = CardType::Default`
- `face`
- `suit`

`face` 和 `suit` 来自 JSON 的 `CardFace` 和 `CardSuit`。

### 7.2 `CardModelDefault`

位置：`Classes/models/CardModelDefault.h`

职责：保存 Default 牌运行时状态，并提供 Default 牌专属读取接口。

关键行为：

- 构造时继承 `CardModelBase` 的 id、区域、顺序和位置能力。
- 将配置坐标的 y 加上 `LayoutConfig::playfieldOffsetFromBottom()` 作为原始桌面位置。
- 通过 `getFace()` 和 `getSuit()` 从 `CardConfigDefault` 读取牌面和花色。

### 7.3 `CardViewDefault`

位置：`Classes/views/CardViewDefault.h/.cpp`

职责：显示 Default 牌面。

`configure()` 会：

1. 调用 `CardViewBase::configure(card)` 绑定 card id 并清理旧子节点。
2. 将 `CardModelBase` 转为 `CardModelDefault`。
3. 使用 `CardAssetUtils` 生成资源路径：
   - `res/card_general.png`
   - `res/number/small_<color>_<rank>.png`
   - `res/number/big_<color>_<rank>.png`
   - `res/suits/<suit>.png`
4. 创建并摆放背景、大小数字和花色 sprite。

### 7.4 `CardControllerDefault`

位置：`Classes/controllers/CardControllerDefault.h/.cpp`

职责：实现 Default 牌的业务规则。

关键方法：

- `generateCardModelFromConfig()`：创建 `CardModelDefault`。
- `createCardView()`：创建 `CardViewDefault`。
- `checkIfClickable()`：判断与当前顶部牌的 `face` 差值是否为 1。
- `doCardAction()`：把桌面牌移动到顶部牌，并返回 `DefaultMoveUndoOperation`。

Default 的回退类目前定义在 `CardControllerDefault.cpp` 的内部命名空间中，因为它只服务于 Default 牌移动逻辑。

## 8. 如何新增一种卡牌类型

新增卡牌类型时，建议按下面清单执行。

### 8.1 增加类型枚举

修改：`Classes/configs/models/CardTypes.h`

在 `enum class CardType` 中添加新值，例如：

```cpp
enum class CardType {
  Default,
  Xxx
};
```

### 8.2 新增静态配置类

新增：`Classes/configs/models/CardConfigXxx.h`

要求：

- 继承 `CardConfigBase`。
- 使用继承自 `CardConfigBase` 的 `type` 字段标识类型，不要再声明同名字段。
- 保存 JSON 中该类型需要的字段。

示例：

```cpp
struct CardConfigXxx : public CardConfigBase {
  int customValue = 0;
};
```

然后在 parser 中创建配置对象后设置：

```cpp
card->type = CardType::Xxx;
```

### 8.3 注册 JSON parser

修改：`Classes/configs/loaders/CardConfigParserRegistry.cpp`

需要：

1. `#include "configs/models/CardConfigXxx.h"`。
2. 新增 `parseXxxCard(rapidjson::Value const &value)`。
3. 校验新类型所需字段。
4. 创建并返回 `std::shared_ptr<CardConfigXxx>`。
5. 在 `CardConfigParserRegistry::CardConfigParserRegistry()` 中注册：

```cpp
registerParser("Xxx", details::parseXxxCard);
```

如果希望 JSON 写小写类型名，也可以注册小写 key，例如 `"xxx"`。保持 parser 注册名和关卡 JSON 的 `type` 一致即可。

### 8.4 新增运行时模型

新增：`Classes/models/CardModelXxx.h`

要求：

- 继承 `CardModelBase`。
- 构造函数接收 `std::shared_ptr<CardConfigBase>`。
- 如果有类型专属字段，提供只读接口，例如 `getCustomValue()`。
- 如果需要特殊初始位置，在构造时设置 `setOriginalPosition()` 和 `setPosition()`。

### 8.5 新增视图类

新增：`Classes/views/CardViewXxx.h/.cpp`

要求：

- 继承 `CardViewBase`。
- 使用 `CREATE_FUNC(CardViewXxx)`。
- 覆盖 `configure(std::shared_ptr<CardModelBase> card)`。
- 先调用 `Super::configure(card)`，再创建该类型需要的 sprite、label 或其他节点。

### 8.6 新增控制器类

新增：`Classes/controllers/CardControllerXxx.h/.cpp`

要求：

- 继承 `CardControllerBase`。
- 实现 `generateCardModelFromConfig()`，返回 `CardModelXxx`。
- 实现 `createCardView()`，返回 `CardViewXxx::create()`。
- 根据该类型规则实现 `checkIfClickable()`。
- 在 `doCardAction()` 中执行正向动作，并返回对应 `UndoOperation`。

如果该类型点击后不产生动作，应返回 `nullptr`，并确认 `GameController::_handleCardClick()` 对该情况的刷新行为符合预期。

### 8.7 注册控制器

修改：`Classes/controllers/GameController.cpp`

在 `GameController::registerCardController()` 中注册：

```cpp
_cardTypeControllers[CardType::Xxx] =
    std::make_shared<CardControllerXxx>(this);
```

并添加对应头文件 include。

### 8.8 更新关卡 JSON

修改：`Resources/configs/level_1.json` 或新增其他关卡文件。

在需要使用新类型的卡牌上写入：

```json
{
  "type": "Xxx",
  "Position": {"x": 100, "y": 200}
}
```

保留 `Position`，因为 `CardConfigBase` 和当前布局/遮挡逻辑都依赖卡牌位置。

### 8.9 更新构建系统

修改：`CMakeLists.txt`

把新增 `.cpp` 加入 `GAME_SOURCE`，必要时把新增 `.h` 加入 `GAME_HEADER`。如果新增资源文件，放到 `Resources` 下，当前 CMake 会按平台复制 `Resources` 目录。

### 8.10 开发检查清单

新增卡牌类型完成后检查：

- `CardType` 枚举、parser 注册名、JSON `type`、controller 注册是否一致。
- parser 对 JSON 字段做了边界校验。
- model 的位置和区域逻辑与 `GameModel` 兼容。
- view 的 `configure()` 不保留旧子节点导致重复显示。
- controller 的 `checkIfClickable()` 不访问空指针。
- controller 的 `doCardAction()` 修改了完整的正向状态，并返回可恢复这些状态的 undo。
- 新文件已加入 `CMakeLists.txt`。

## 9. 如何新增一种回退类型

回退类型对应一次可撤销的正向动作。新增时建议按下面清单执行。

### 9.1 明确正向动作修改了哪些状态

先列出动作会改动的 model 状态，例如：

- 卡牌 `CardZone`。
- 卡牌当前位置。
- 当前顶部牌 id。
- 备用牌列表顺序。
- 桌面牌遮挡依赖度。
- 其他类型专属状态。

回退操作必须能恢复这些状态。

### 9.2 在执行动作前捕获旧状态

例如 Default 移动牌时会先记录：

- `previousTrayId`
- 被移动牌的 `originalPosition`

备用牌抽取时会先记录：

- `previousTrayId`
- 被抽出的 `drawnCardId`
- 备用牌堆位置

### 9.3 继承 `UndoOperation`

位置：`Classes/managers/UndoOperation.h`

回退类可以放在动作所属 `.cpp` 的内部命名空间中；如果多个模块复用，再拆到独立文件。

基本结构：

```cpp
class XxxUndoOperation : public UndoOperation {
 public:
  void undo() override {
    // 恢复模型状态
  }

  bool getUndoAnimation(UndoAnimation *animation) const override {
    if (!animation) {
      return false;
    }
    animation->cardId = _cardId;
    animation->targetPosition = _targetPosition;
    return true;
  }
};
```

### 9.4 `undo()` 只负责恢复模型状态

当前设计中，动画由 `GameController::_handleUndoClick()` 在 `UndoManager::undo()` 之后统一执行。因此 `undo()` 应专注恢复 model，不直接操作 `GameView`。

### 9.5 如需动画，实现 `getUndoAnimation()`

如果回退需要把某张牌移动到目标位置：

- 设置 `animation->cardId`。
- 设置 `animation->targetPosition`。
- 返回 true。

如果不需要动画，使用基类默认实现即可。

### 9.6 让正向动作返回 `std::unique_ptr<UndoOperation>`

正向动作函数应该在修改 model 后返回 undo 对象，例如：

```cpp
return std::unique_ptr<UndoOperation>(new XxxUndoOperation(...));
```

当前项目中的两个入口：

- 卡牌点击动作：`CardControllerBase::doCardAction()`。
- 备用牌抽取动作：`StackController::drawReserveCard()`。

### 9.7 由 `GameController` 推入 `UndoManager`

动作返回后，由 `GameController` 统一调用：

```cpp
_undoManager.push(std::move(operation));
```

这样 Undo 按钮只需要调用 `UndoManager::undo()`，不需要知道具体动作类型。

### 9.8 回退开发检查清单

新增回退类型完成后检查：

- 正向动作前捕获了所有恢复所需状态。
- `undo()` 恢复 model 后，`GameView::refresh()` 能正确显示最终状态。
- 如果有动画，`getUndoAnimation()` 的目标位置与恢复后的 model 位置一致。
- `UndoOperation` 对 model 的持有方式安全：要么用 `std::shared_ptr` 延长生命周期，要么确认非拥有指针生命周期长于 undo 栈。
- 如果动作会改变遮挡关系，正向动作和 undo 都要同步维护 `CardManager`。

## 10. 构建系统注册与开发检查清单

### 10.1 新文件注册

新增 C++ 源文件后，修改 `CMakeLists.txt`：

- `.cpp` 加入 `GAME_SOURCE`。
- 需要被 IDE 或工程生成器识别的 `.h` 可加入 `GAME_HEADER`。
- `Classes` 已在 include path 中，新文件建议继续放在 `Classes` 下的现有模块目录。

### 10.2 资源注册

新增图片、JSON 或字体资源时，建议放入 `Resources`。当前构建脚本通过 `GAME_RES_FOLDER` 指向 `Resources`，并在 Linux/Windows 下使用 `cocos_copy_target_res()` 复制资源。

### 10.3 功能检查清单

修改或扩展玩法后，建议至少人工检查：

- 游戏能进入 `GameScene`。
- `level_1.json` 能被 `LevelConfigLoader` 成功解析。
- 桌面牌、备用牌堆、顶部牌初始显示正确。
- 只有顶层且规则匹配的桌面牌可点击。
- 点击桌面牌后有移动动画，动画结束后输入恢复。
- 点击备用牌堆后，备用牌数量减少，顶部牌更新。
- Undo 按钮只在有可回退操作时可用。
- Undo 能正确恢复桌面牌移动和备用牌抽取。
- 新增文件已加入 `CMakeLists.txt`。
