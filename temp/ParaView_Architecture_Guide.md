# ParaView 程序构建架构完整指南

## 📋 目录
1. [整体架构概览](#1-整体架构概览)
2. [模块化 UI 设计](#2-模块化-ui-设计)
3. [主窗口构建流程](#3-主窗口构建流程)
4. [DockWidget 系统](#4-dockwidget-系统)
5. [菜单与工具栏构建](#5-菜单与工具栏构建)
6. [插件与扩展机制](#6-插件与扩展机制)
7. [完整构建示例](#7-完整构建示例)

---

## 1. 整体架构概览

### 1.1 ParaView 的分层架构

```
┌─────────────────────────────────────────────────────────────┐
│                    应用层 (Application)                      │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  ParaViewMainWindow                                  │  │
│  │  - 主窗口框架                                         │  │
│  │  - 菜单栏、工具栏、状态栏                             │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────────┐
│                  组件层 (Components)                         │
│  ┌──────────────┬──────────────┬──────────────────────┐   │
│  │ DockWidgets  │ Panels       │ Dialogs              │   │
│  │ - Pipeline   │ - Properties │ - Welcome Dialog     │   │
│  │ - Properties │ - ColorMap   │ - Settings Dialog    │   │
│  │ - ColorMap   │ - Information│ - About Dialog       │   │
│  │ - Python     │              │                      │   │
│  └──────────────┴──────────────┴──────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────────┐
│                  核心层 (Core)                               │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  pqApplicationCore                                   │  │
│  │  - 应用程序核心管理                                   │  │
│  │  - 对象管理器、事件管理器                             │  │
│  │  - 插件管理器、设置管理器                             │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────────┐
│                  服务器管理层 (ServerManager)                │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  vtkSMProxyManager                                   │  │
│  │  - 代理对象管理                                       │  │
│  │  - 客户端-服务器通信                                  │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────────┐
│                  渲染层 (VTK)                                │
│  ┌──────────────────────────────────────────────────────┐  │
│  │  VTK Pipeline                                        │  │
│  │  - 数据处理管道                                       │  │
│  │  - 渲染引擎                                           │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 关键设计理念

**核心思想：模块化 + 可扩展**

1. **主窗口只是容器** - MainWindow 不包含具体业务逻辑
2. **DockWidget 承载功能** - 每个功能面板都是独立的 DockWidget
3. **UI 文件分离** - 每个组件有自己的 .ui 文件
4. **动态加载** - 面板可以动态显示/隐藏、停靠/浮动
5. **插件化** - 通过插件系统扩展功能

---

## 2. 模块化 UI 设计

### 2.1 UI 文件组织结构

```
Clients/ParaView/
├── ParaViewMainWindow.ui          # 主窗口框架
├── ParaViewMainWindow.h
└── ParaViewMainWindow.cxx

Qt/ApplicationComponents/
├── Resources/UI/
│   ├── pqColorOpacityEditorWidget.ui    # 颜色编辑器
│   ├── pqPropertiesPanel.ui             # 属性面板
│   ├── pqPipelineBrowserWidget.ui       # 管线浏览器
│   ├── pqMemoryInspectorPanel.ui        # 内存检查器
│   └── ...                              # 其他组件 UI
├── pqColorOpacityEditorWidget.h
├── pqColorOpacityEditorWidget.cxx
├── pqPropertiesPanel.h
├── pqPropertiesPanel.cxx
└── ...
```

### 2.2 为什么不用一个大 UI 文件？

#### ❌ 单一 UI 文件的问题

```xml
<!-- 不推荐：所有控件都在一个文件里 -->
<ui version="4.0">
 <widget class="QMainWindow">
  <widget name="pipelineBrowser">
    <!-- 500 行代码 -->
  </widget>
  <widget name="propertiesPanel">
    <!-- 1000 行代码 -->
  </widget>
  <widget name="colorMapEditor">
    <!-- 800 行代码 -->
  </widget>
  <!-- 总共 10000+ 行... -->
 </widget>
</ui>
```

**问题：**
- 文件过大，难以维护
- 多人协作冲突频繁
- 加载速度慢
- 无法复用组件
- 难以测试单个组件

#### ✅ 模块化 UI 的优势

```xml
<!-- ParaViewMainWindow.ui - 只定义框架 -->
<ui version="4.0">
 <widget class="QMainWindow">
  <widget class="QDockWidget" name="pipelineBrowserDock">
   <widget class="pqPipelineBrowserWidget" name="pipelineBrowser"/>
  </widget>
  <widget class="QDockWidget" name="propertiesDock">
   <widget class="pqPropertiesPanel" name="propertiesPanel"/>
  </widget>
  <!-- 只定义容器，不定义内部细节 -->
 </widget>
</ui>
```

```xml
<!-- pqPropertiesPanel.ui - 独立的组件 UI -->
<ui version="4.0">
 <widget class="QWidget" name="PropertiesPanel">
  <!-- 属性面板的具体实现 -->
  <layout>
    <widget class="QScrollArea">
      <!-- 详细内容 -->
    </widget>
  </layout>
 </widget>
</ui>
```

**优势：**
- ✅ 每个文件小而专注
- ✅ 组件可独立开发和测试
- ✅ 可在多个地方复用
- ✅ 团队协作无冲突
- ✅ 按需加载，性能更好

---

## 3. 主窗口构建流程

### 3.1 主窗口的职责

**ParaViewMainWindow 只负责：**
1. 创建窗口框架（菜单栏、工具栏、状态栏）
2. 创建 DockWidget 容器
3. 设置布局和停靠关系
4. 连接信号槽
5. 初始化行为管理器

**不负责：**
- ❌ 具体业务逻辑
- ❌ 数据处理
- ❌ 渲染控制

### 3.2 构建步骤详解

#### 步骤 1: 定义主窗口 UI 框架

```xml
<!-- ParaViewMainWindow.ui -->
<ui version="4.0">
 <class>pqClientMainWindow</class>
 <widget class="QMainWindow" name="pqClientMainWindow">
  
  <!-- 中央窗口：渲染视图 -->
  <widget class="QWidget" name="centralwidget">
   <layout class="QGridLayout">
    <item row="0" column="0">
     <widget class="pqTabbedMultiViewWidget" name="MultiViewWidget"/>
    </item>
   </layout>
  </widget>
  
  <!-- 菜单栏 -->
  <widget class="QMenuBar" name="menubar">
   <widget class="QMenu" name="menu_File">
    <property name="title"><string>&amp;File</string></property>
   </widget>
   <widget class="QMenu" name="menu_Edit">
    <property name="title"><string>&amp;Edit</string></property>
   </widget>
   <!-- 其他菜单... -->
  </widget>
  
  <!-- 状态栏 -->
  <widget class="pqStatusBar" name="statusbar"/>
  
  <!-- DockWidget 1: Pipeline Browser -->
  <widget class="QDockWidget" name="pipelineBrowserDock">
   <property name="windowTitle"><string>Pipeline Browser</string></property>
   <attribute name="dockWidgetArea"><number>1</number></attribute>
   <widget class="pqPipelineBrowserWidget" name="pipelineBrowser"/>
  </widget>
  
  <!-- DockWidget 2: Properties Panel -->
  <widget class="QDockWidget" name="propertiesDock">
   <property name="windowTitle"><string>Properties</string></property>
   <attribute name="dockWidgetArea"><number>1</number></attribute>
   <widget class="pqPropertiesPanel" name="propertiesPanel"/>
  </widget>
  
  <!-- DockWidget 3: Color Map Editor -->
  <widget class="QDockWidget" name="colorMapEditorDock">
   <property name="windowTitle"><string>Color Map Editor</string></property>
   <attribute name="dockWidgetArea"><number>2</number></attribute>
   <widget class="pqColorMapEditor" name="colorMapEditorPanel"/>
  </widget>
  
  <!-- 更多 DockWidgets... -->
  
 </widget>
 
 <!-- 自定义控件声明 -->
 <customwidgets>
  <customwidget>
   <class>pqPipelineBrowserWidget</class>
   <extends>QWidget</extends>
   <header>pqPipelineBrowserWidget.h</header>
  </customwidget>
  <customwidget>
   <class>pqPropertiesPanel</class>
   <extends>QWidget</extends>
   <header>pqPropertiesPanel.h</header>
  </customwidget>
  <!-- 更多自定义控件... -->
 </customwidgets>
</ui>
```

#### 步骤 2: 实现主窗口类

```cpp
// ParaViewMainWindow.h
#ifndef ParaViewMainWindow_h
#define ParaViewMainWindow_h

#include <QMainWindow>

class ParaViewMainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  ParaViewMainWindow();
  ~ParaViewMainWindow() override;
  
protected:
  void showEvent(QShowEvent* evt) override;
  void closeEvent(QCloseEvent* evt) override;
  
private:
  class pqInternals;
  pqInternals* Internals;
};

#endif
```

```cpp
// ParaViewMainWindow.cxx
#include "ParaViewMainWindow.h"
#include "ui_ParaViewMainWindow.h"  // 由 UIC 生成
#include "pqParaViewBehaviors.h"
#include "pqParaViewMenuBuilders.h"
#include "pqApplicationCore.h"

// 内部实现类（PIMPL 模式）
class ParaViewMainWindow::pqInternals : public Ui::pqClientMainWindow
{
public:
  bool FirstShow;
  
  pqInternals() : FirstShow(true) {}
};

// 构造函数
ParaViewMainWindow::ParaViewMainWindow()
{
  // 1. 创建内部实现对象
  this->Internals = new pqInternals();
  
  // 2. 设置 UI（加载 .ui 文件）
  this->Internals->setupUi(this);
  
  // 3. 初始化隐藏某些面板
  this->Internals->outputWidgetDock->hide();
  this->Internals->pythonShellDock->hide();
  this->Internals->colorMapEditorDock->hide();
  
  // 4. 设置停靠窗口布局
  this->setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
  this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
  this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
  
  // 5. 将多个 DockWidget 合并为标签页
  this->tabifyDockWidget(
    this->Internals->colorMapEditorDock, 
    this->Internals->memoryInspectorDock);
  this->tabifyDockWidget(
    this->Internals->colorMapEditorDock, 
    this->Internals->lightInspectorDock);
  
  // 6. 设置属性面板的标签页
  this->tabifyDockWidget(
    this->Internals->propertiesDock, 
    this->Internals->viewPropertiesDock);
  this->tabifyDockWidget(
    this->Internals->propertiesDock, 
    this->Internals->displayPropertiesDock);
  
  // 7. 显示默认面板
  this->Internals->propertiesDock->show();
  this->Internals->propertiesDock->raise();
  
  // 8. 注册管理器（供其他组件访问）
  pqApplicationCore::instance()->registerManager(
    "COLOR_EDITOR_PANEL", this->Internals->colorMapEditorDock);
  
  // 9. 构建菜单
  pqParaViewMenuBuilders::buildFileMenu(*this->Internals->menu_File);
  pqParaViewMenuBuilders::buildEditMenu(*this->Internals->menu_Edit);
  pqParaViewMenuBuilders::buildSourcesMenu(*this->Internals->menuSources, this);
  pqParaViewMenuBuilders::buildFiltersMenu(*this->Internals->menuFilters, this);
  pqParaViewMenuBuilders::buildViewMenu(*this->Internals->menu_View, *this);
  
  // 10. 构建工具栏
  pqParaViewMenuBuilders::buildToolbars(*this);
  
  // 11. 初始化行为管理器（处理各种交互行为）
  new pqParaViewBehaviors(this, this);
}

ParaViewMainWindow::~ParaViewMainWindow()
{
  delete this->Internals;
}
```

#### 步骤 3: PIMPL 模式的优势

**为什么使用 pqInternals？**

```cpp
// 不使用 PIMPL（不推荐）
class ParaViewMainWindow : public QMainWindow
{
  Q_OBJECT
  
private:
  Ui::pqClientMainWindow ui;  // 直接暴露 UI 成员
  QTimer* timer;
  QFont defaultFont;
  // ... 更多私有成员
};
```

**问题：**
- 头文件包含过多依赖
- 修改私有成员需要重新编译所有依赖文件
- 头文件暴露实现细节

```cpp
// 使用 PIMPL（推荐）
class ParaViewMainWindow : public QMainWindow
{
  Q_OBJECT
  
private:
  class pqInternals;  // 前向声明
  pqInternals* Internals;  // 指针，隐藏实现
};
```

**优势：**
- ✅ 减少头文件依赖
- ✅ 加快编译速度
- ✅ 隐藏实现细节
- ✅ 二进制兼容性更好

---

## 4. DockWidget 系统

### 4.1 DockWidget 的作用

**QDockWidget 是 Qt 提供的可停靠窗口：**
- 可以停靠在主窗口的四个边缘
- 可以浮动为独立窗口
- 可以与其他 DockWidget 合并为标签页
- 可以动态显示/隐藏

### 4.2 ParaView 中的 DockWidget 列表

| DockWidget 名称 | 功能 | 默认位置 | 默认状态 |
|----------------|------|---------|---------|
| pipelineBrowserDock | 管线浏览器 | 左侧 | 显示 |
| propertiesDock | 属性面板 | 左侧 | 显示 |
| displayPropertiesDock | 显示属性 | 左侧 | 隐藏（标签页） |
| viewPropertiesDock | 视图属性 | 左侧 | 隐藏（标签页） |
| informationDock | 信息面板 | 左侧 | 隐藏（标签页） |
| colorMapEditorDock | 颜色映射编辑器 | 右侧 | 隐藏 |
| memoryInspectorDock | 内存检查器 | 右侧 | 隐藏（标签页） |
| lightInspectorDock | 光照检查器 | 右侧 | 隐藏（标签页） |
| multiBlockInspectorDock | 多块检查器 | 右侧 | 隐藏（标签页） |
| findDataDock | 查找数据 | 右侧 | 隐藏（标签页） |
| pythonShellDock | Python 控制台 | 底部 | 隐藏 |
| outputWidgetDock | 输出消息 | 底部 | 隐藏（标签页） |
| timeManagerDock | 时间管理器 | 底部 | 隐藏 |
| statisticsDock | 统计检查器 | 底部 | 隐藏（标签页） |

### 4.3 DockWidget 布局管理

#### 停靠区域代码

```cpp
// Qt::DockWidgetArea 枚举值
// 1 = Qt::LeftDockWidgetArea   (左侧)
// 2 = Qt::RightDockWidgetArea  (右侧)
// 4 = Qt::TopDockWidgetArea    (顶部)
// 8 = Qt::BottomDockWidgetArea (底部)
```

#### 设置停靠位置

```xml
<!-- 在 UI 文件中设置 -->
<widget class="QDockWidget" name="pipelineBrowserDock">
  <attribute name="dockWidgetArea">
    <number>1</number>  <!-- 左侧 -->
  </attribute>
</widget>
```

```cpp
// 在代码中设置
this->addDockWidget(Qt::LeftDockWidgetArea, pipelineBrowserDock);
```

#### 合并为标签页

```cpp
// 将 colorMapEditorDock 和 memoryInspectorDock 合并为标签页
this->tabifyDockWidget(
  this->Internals->colorMapEditorDock,
  this->Internals->memoryInspectorDock
);

// 继续添加更多标签页
this->tabifyDockWidget(
  this->Internals->colorMapEditorDock,
  this->Internals->lightInspectorDock
);
```

**效果：**
```
┌─────────────────────────────────┐
│ [ColorMap] [Memory] [Light]     │  ← 标签页
├─────────────────────────────────┤
│                                 │
│   当前选中标签的内容             │
│                                 │
└─────────────────────────────────┘
```

#### 设置角落归属

```cpp
// 设置窗口四个角落的归属
this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
```

**效果：**
```
┌──────────┬────────────┬──────────┐
│          │            │          │
│   左侧   │   中央区   │   右侧   │
│  Dock    │   域       │  Dock    │
│          │            │          │
├──────────┴────────────┴──────────┤
│          底部 Dock 区域           │
└──────────────────────────────────┘
```

### 4.4 创建自定义 DockWidget

#### 示例：创建一个新的面板

```cpp
// 1. 创建 DockWidget
QDockWidget* myCustomDock = new QDockWidget("My Custom Panel", this);
myCustomDock->setObjectName("myCustomDock");
myCustomDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

// 2. 创建面板内容（自定义 Widget）
MyCustomWidget* customWidget = new MyCustomWidget(this);
myCustomDock->setWidget(customWidget);

// 3. 添加到主窗口
this->addDockWidget(Qt::RightDockWidgetArea, myCustomDock);

// 4. 初始隐藏
myCustomDock->hide();

// 5. 添加到 View 菜单（可选）
QAction* toggleAction = myCustomDock->toggleViewAction();
this->Internals->menu_View->addAction(toggleAction);
```

---

## 5. 菜单与工具栏构建

### 5.1 菜单构建器模式

ParaView 使用 **Builder 模式** 构建菜单，而不是在 UI 文件中硬编码。

#### 为什么不在 UI 文件中定义菜单项？

**❌ 在 UI 文件中硬编码（不推荐）：**
```xml
<widget class="QMenu" name="menu_File">
  <addaction name="actionOpen"/>
  <addaction name="actionSave"/>
  <addaction name="actionExit"/>
</widget>
```

**问题：**
- 菜单项固定，无法动态调整
- 插件无法添加菜单项
- 难以根据上下文启用/禁用菜单项

**✅ 使用 Builder 模式（推荐）：**
```cpp
// 菜单在代码中动态构建
pqParaViewMenuBuilders::buildFileMenu(*this->Internals->menu_File);
```

**优势：**
- 可以根据配置动态添加菜单项
- 插件可以注入自己的菜单项
- 可以根据状态动态启用/禁用

### 5.2 菜单构建示例

```cpp
// pqParaViewMenuBuilders.cxx
void pqParaViewMenuBuilders::buildFileMenu(QMenu& menu)
{
  // 1. 添加 "New" 动作
  QAction* action = menu.addAction("&New");
  new pqLoadDataReaction(action);
  
  // 2. 添加 "Open" 动作
  action = menu.addAction("&Open...");
  action->setShortcut(QKeySequence::Open);
  new pqLoadDataReaction(action);
  
  // 3. 添加分隔符
  menu.addSeparator();
  
  // 4. 添加 "Save" 动作
  action = menu.addAction("&Save State...");
  action->setShortcut(QKeySequence::Save);
  new pqSaveStateReaction(action);
  
  // 5. 添加最近文件子菜单
  QMenu* recentMenu = menu.addMenu("Recent Files");
  pqRecentFilesMenu* recentFilesMenu = new pqRecentFilesMenu(*recentMenu);
  
  // 6. 添加退出动作
  menu.addSeparator();
  action = menu.addAction("E&xit");
  action->setShortcut(QKeySequence::Quit);
  QObject::connect(action, &QAction::triggered, 
                   qApp, &QApplication::quit);
}
```

### 5.3 Reaction 模式

**Reaction 是 ParaView 的核心设计模式：**

```cpp
// Reaction 将 QAction 与业务逻辑连接
class pqLoadDataReaction : public pqReaction
{
  Q_OBJECT
  
public:
  pqLoadDataReaction(QAction* parent)
    : pqReaction(parent)
  {
  }
  
protected:
  void onTriggered() override
  {
    // 执行加载数据的业务逻辑
    pqFileDialog dialog(nullptr, pqCoreUtilities::mainWidget(),
                        tr("Open File"), QString(),
                        "All Files (*)");
    if (dialog.exec() == QDialog::Accepted)
    {
      QStringList files = dialog.getSelectedFiles();
      // 加载文件...
    }
  }
};
```

**使用方式：**
```cpp
QAction* openAction = new QAction("Open", this);
new pqLoadDataReaction(openAction);  // Reaction 自动连接信号槽
```

**优势：**
- 业务逻辑与 UI 分离
- 可复用（同一个 Reaction 可用于菜单和工具栏）
- 易于测试

### 5.4 工具栏构建

```cpp
void pqParaViewMenuBuilders::buildToolbars(QMainWindow& mainWindow)
{
  // 1. 创建主工具栏
  QToolBar* mainToolBar = new QToolBar("Main Controls", &mainWindow);
  mainToolBar->setObjectName("MainControlsToolbar");
  mainWindow.addToolBar(Qt::TopToolBarArea, mainToolBar);
  
  // 2. 添加工具按钮
  QAction* action = mainToolBar->addAction(
    QIcon(":/pqWidgets/Icons/pqOpen.svg"), "Open");
  new pqLoadDataReaction(action);
  
  action = mainToolBar->addAction(
    QIcon(":/pqWidgets/Icons/pqSave.svg"), "Save");
  new pqSaveStateReaction(action);
  
  mainToolBar->addSeparator();
  
  // 3. 创建 VCR 控制工具栏（播放/暂停/停止）
  QToolBar* vcrToolBar = new QToolBar("VCR Controls", &mainWindow);
  vcrToolBar->setObjectName("VCRToolbar");
  mainWindow.addToolBar(Qt::TopToolBarArea, vcrToolBar);
  
  action = vcrToolBar->addAction(
    QIcon(":/pqWidgets/Icons/pqVcrFirst.svg"), "First Frame");
  new pqVCRController(action, pqVCRController::FIRST);
  
  action = vcrToolBar->addAction(
    QIcon(":/pqWidgets/Icons/pqVcrPlay.svg"), "Play");
  new pqVCRController(action, pqVCRController::PLAY);
  
  // 4. 创建相机工具栏
  QToolBar* cameraToolBar = new QToolBar("Camera Controls", &mainWindow);
  cameraToolBar->setObjectName("CameraToolbar");
  mainWindow.addToolBar(Qt::TopToolBarArea, cameraToolBar);
  
  action = cameraToolBar->addAction(
    QIcon(":/pqWidgets/Icons/pqResetCamera.svg"), "Reset Camera");
  new pqCameraReaction(action, pqCameraReaction::RESET_CAMERA);
}
```

---

## 6. 插件与扩展机制

### 6.1 插件系统架构

ParaView 的插件可以扩展：
- 新的数据源（Sources）
- 新的过滤器（Filters）
- 新的视图（Views）
- 新的面板（Panels）
- 新的工具栏（Toolbars）
- 新的菜单项（Menu Items）

### 6.2 插件如何添加 UI 组件

```cpp
// 插件的 pqMyPluginStarter.cxx
class pqMyPluginStarter : public QObject, public pqAutoStartInterface
{
  Q_OBJECT
  Q_INTERFACES(pqAutoStartInterface)
  
public:
  void startup() override
  {
    // 1. 创建自定义 DockWidget
    QDockWidget* myDock = new QDockWidget("My Plugin Panel");
    MyPluginWidget* widget = new MyPluginWidget();
    myDock->setWidget(widget);
    
    // 2. 添加到主窗口
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(
      pqCoreUtilities::mainWidget());
    mainWindow->addDockWidget(Qt::RightDockWidgetArea, myDock);
    
    // 3. 添加菜单项
    QMenu* toolsMenu = /* 获取 Tools 菜单 */;
    QAction* action = toolsMenu->addAction("My Plugin Action");
    connect(action, &QAction::triggered, this, &pqMyPluginStarter::onMyAction);
  }
  
  void shutdown() override
  {
    // 清理资源
  }
};
```

---

## 7. 完整构建示例

### 7.1 从零构建一个简化版 ParaView

#### 项目结构

```
MyParaView/
├── CMakeLists.txt
├── main.cpp
├── MyMainWindow.h
├── MyMainWindow.cpp
├── MyMainWindow.ui
└── Panels/
    ├── MyPropertiesPanel.h
    ├── MyPropertiesPanel.cpp
    └── MyPropertiesPanel.ui
```

#### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.12)
project(MyParaView)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(ParaView REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

add_executable(MyParaView
  main.cpp
  MyMainWindow.cpp
  MyMainWindow.h
  MyMainWindow.ui
  Panels/MyPropertiesPanel.cpp
  Panels/MyPropertiesPanel.h
  Panels/MyPropertiesPanel.ui
)

target_link_libraries(MyParaView
  Qt5::Core
  Qt5::Widgets
  ParaView::pqApplicationComponents
  ParaView::pqCore
)
```

#### main.cpp

```cpp
#include <QApplication>
#include "MyMainWindow.h"
#include "pqApplicationCore.h"
#include "pqPVApplicationCore.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, char** argv);
  
  // 初始化 ParaView 核心
  pqPVApplicationCore appCore(argc, argv);
  
  // 创建主窗口
  MyMainWindow mainWindow;
  mainWindow.show();
  
  return app.exec();
}
```

#### MyMainWindow.ui

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MyMainWindow</class>
 <widget class="QMainWindow" name="MyMainWindow">
  <widget class="QWidget" name="centralwidget">
   <layout class="QGridLayout">
    <item row="0" column="0">
     <widget class="pqTabbedMultiViewWidget" name="viewWidget"/>
    </item>
   </layout>
  </widget>
  
  <widget class="QMenuBar" name="menubar">
   <widget class="QMenu" name="menuFile">
    <property name="title"><string>File</string></property>
   </widget>
  </widget>
  
  <widget class="QDockWidget" name="propertiesDock">
   <property name="windowTitle"><string>Properties</string></property>
   <attribute name="dockWidgetArea"><number>1</number></attribute>
   <widget class="MyPropertiesPanel" name="propertiesPanel"/>
  </widget>
 </widget>
 
 <customwidgets>
  <customwidget>
   <class>pqTabbedMultiViewWidget</class>
   <extends>QWidget</extends>
   <header>pqTabbedMultiViewWidget.h</header>
  </customwidget>
  <customwidget>
   <class>MyPropertiesPanel</class>
   <extends>QWidget</extends>
   <header>Panels/MyPropertiesPanel.h</header>
  </customwidget>
 </customwidgets>
</ui>
```

#### MyMainWindow.cpp

```cpp
#include "MyMainWindow.h"
#include "ui_MyMainWindow.h"
#include "pqParaViewMenuBuilders.h"

class MyMainWindow::pqInternals : public Ui::MyMainWindow
{
};

MyMainWindow::MyMainWindow()
{
  this->Internals = new pqInternals();
  this->Internals->setupUi(this);
  
  // 构建菜单
  pqParaViewMenuBuilders::buildFileMenu(*this->Internals->menuFile);
  
  // 显示属性面板
  this->Internals->propertiesDock->show();
}

MyMainWindow::~MyMainWindow()
{
  delete this->Internals;
}
```

---

## 8. 总结

### 8.1 ParaView UI 构建的核心原则

1. **模块化设计** - 每个功能面板是独立的组件
2. **UI 文件分离** - 主窗口只定义框架，组件有自己的 UI 文件
3. **DockWidget 系统** - 灵活的面板布局管理
4. **Builder 模式** - 动态构建菜单和工具栏
5. **Reaction 模式** - 业务逻辑与 UI 分离
6. **PIMPL 模式** - 隐藏实现细节，加快编译
7. **插件化** - 通过插件扩展功能

### 8.2 关键技术栈

- **Qt Widgets** - UI 框架
- **Qt Designer** - UI 设计工具
- **CMake** - 构建系统
- **VTK** - 可视化引擎
- **ServerManager** - 客户端-服务器架构

### 8.3 学习路径

1. 理解 Qt 的 DockWidget 系统
2. 学习 ParaView 的 Reaction 模式
3. 研究现有组件的实现
4. 创建自己的自定义面板
5. 开发插件扩展功能

---

**文档完成！** 🎉

现在你应该完全理解 ParaView 是如何通过模块化的方式构建复杂 UI 界面的了。

