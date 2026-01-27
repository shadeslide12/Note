# ParaView 颜色映射预设选择器 - 完整搭建指南

## 📋 目录
1. [系统架构概览](#1-系统架构概览)
2. [核心组件详解](#2-核心组件详解)
3. [完整实现步骤](#3-完整实现步骤)
4. [代码示例](#4-代码示例)
5. [关键技术点](#5-关键技术点)

---

## 1. 系统架构概览

### 1.1 四层架构设计

```
┌─────────────────────────────────────────────────────────┐
│                    UI 展示层 (Qt)                        │
│  ┌──────────────────────────────────────────────────┐  │
│  │  pqColorOpacityEditorWidget                      │  │
│  │  - DefaultPresetsComboBox (QComboBox)            │  │
│  │  - pqColorMapDelegate (自定义渲染)                │  │
│  └──────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────┐
│                  预设分组管理层                          │
│  ┌──────────────────────────────────────────────────┐  │
│  │  pqPresetGroupsManager                           │  │
│  │  - 管理预设分组 (Default, User, etc.)            │  │
│  │  - 从 JSON 加载分组配置                          │  │
│  └──────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────┐
│                  预设存储管理层                          │
│  ┌──────────────────────────────────────────────────┐  │
│  │  vtkSMTransferFunctionPresets (单例)             │  │
│  │  - 内置预设 (BuiltinPresets)                     │  │
│  │  - 自定义预设 (CustomPresets)                    │  │
│  │  - JSON 格式存储                                 │  │
│  └──────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
                          ↓↑
┌─────────────────────────────────────────────────────────┐
│                  渲染引擎层 (VTK)                        │
│  ┌──────────────────────────────────────────────────┐  │
│  │  pqPresetToPixmap                                │  │
│  │  - vtkScalarsToColors (颜色传输函数)             │  │
│  │  - vtkPiecewiseFunction (不透明度函数)           │  │
│  │  - 生成 QPixmap 预览图                           │  │
│  └──────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
```

### 1.2 数据流向

```
用户操作
   ↓
QComboBox::currentTextChanged 信号
   ↓
pqColorOpacityEditorWidget::槽函数
   ↓
vtkSMTransferFunctionProxy::ApplyPreset()
   ↓
更新 VTK 传输函数
   ↓
触发渲染更新
```

---

## 2. 核心组件详解

### 2.1 预设数据结构 (JSON)

```json
{
  "Name": "Cool to Warm",
  "Creator": "Kenneth Moreland",
  "RGBPoints": [
    0.0, 0.23137254902, 0.298039215686, 0.752941176471,
    0.5, 0.865, 0.865, 0.865,
    1.0, 0.705882352941, 0.0156862745098, 0.149019607843
  ],
  "ColorSpace": "Diverging",
  "NanColor": [1.0, 1.0, 0.0],
  "Points": [
    0.0, 0.0, 0.5, 0.0,
    1.0, 1.0, 0.5, 0.0
  ]
}
```

**字段说明：**
- `Name`: 预设名称
- `RGBPoints`: 颜色控制点 [位置, R, G, B, 位置, R, G, B, ...]
- `Points`: 不透明度控制点 [位置, 不透明度, 中点, 锐度, ...]
- `ColorSpace`: 颜色空间 (RGB, HSV, Lab, Diverging)
- `NanColor`: NaN 值的颜色

### 2.2 分组配置 (JSON)

```json
[
  {
    "groupName": "Default",
    "presets": [
      "Cool to Warm",
      "Viridis",
      "Inferno",
      "Rainbow Uniform"
    ]
  },
  {
    "groupName": "User",
    "presets": [
      "My Custom Preset 1"
    ]
  }
]
```

---

## 3. 完整实现步骤

### 步骤 1: 创建预设管理器 (后端)

#### 文件结构
```
Remoting/Views/
├── vtkSMTransferFunctionPresets.h
├── vtkSMTransferFunctionPresets.cxx
└── vtkSMTransferFunctionPresetsColorMaps.json (嵌入到代码中)
```

#### vtkSMTransferFunctionPresets.h

```cpp
#ifndef vtkSMTransferFunctionPresets_h
#define vtkSMTransferFunctionPresets_h

#include "vtkSMObject.h"
#include <vtk_jsoncpp_fwd.h>

class vtkSMTransferFunctionPresets : public vtkSMObject
{
public:
  static vtkSMTransferFunctionPresets* New();
  static vtkSMTransferFunctionPresets* GetInstance();
  
  // 获取预设数量
  unsigned int GetNumberOfPresets();
  
  // 获取预设
  const Json::Value& GetPreset(unsigned int index);
  std::string GetPresetName(unsigned int index);
  
  // 添加/删除预设
  bool AddPreset(const char* name, const Json::Value& preset);
  std::string AddUniquePreset(const Json::Value& preset, const char* prefix = nullptr);
  bool RemovePreset(unsigned int index);
  
  // 判断预设类型
  bool IsPresetBuiltin(unsigned int index);
  bool GetPresetHasOpacities(unsigned int index);
  bool GetPresetHasIndexedColors(unsigned int index);
  
  // 导入/导出
  bool ImportPresets(const char* filename);
  
protected:
  vtkSMTransferFunctionPresets();
  ~vtkSMTransferFunctionPresets() override;
  
private:
  class vtkInternals;
  vtkInternals* Internals;
  static vtkSmartPointer<vtkSMTransferFunctionPresets> Instance;
};

#endif
```


#### vtkSMTransferFunctionPresets.cxx (核心实现)

```cpp
#include "vtkSMTransferFunctionPresets.h"
#include "vtkSMSettings.h"
#include <vtk_jsoncpp.h>
#include <vector>

// 内部实现类
class vtkSMTransferFunctionPresets::vtkInternals
{
public:
  std::vector<Json::Value> BuiltinPresets;   // 内置预设
  std::vector<Json::Value> CustomPresets;    // 自定义预设
  std::vector<Json::Value> Presets;          // 合并后的所有预设
  bool CustomPresetsLoaded = false;
  
  void LoadBuiltinPresets()
  {
    if (!this->BuiltinPresets.empty()) return;
    
    // 从嵌入的 JSON 字符串加载内置预设
    char* rawJSON = vtkSMTransferFunctionPresetsColorMapsJSON();
    
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value value;
    std::string errors;
    
    if (reader->parse(rawJSON, rawJSON + strlen(rawJSON), &value, &errors))
    {
      for (auto const& preset : value)
      {
        this->BuiltinPresets.push_back(preset);
      }
    }
    delete[] rawJSON;
  }
  
  void LoadCustomPresets()
  {
    if (this->CustomPresetsLoaded) return;
    this->CustomPresetsLoaded = true;
    
    // 从 vtkSMSettings 加载自定义预设
    const char* settingsKey = "TransferFunctionPresets.CustomPresets";
    vtkSMSettings* settings = vtkSMSettings::GetInstance();
    
    if (settings && settings->HasSetting(settingsKey))
    {
      std::string presetJSON = settings->GetSettingAsString(settingsKey, "");
      const char* input = presetJSON.c_str();
      
      Json::CharReaderBuilder builder;
      std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
      Json::Value value;
      std::string errors;
      
      if (!presetJSON.empty() && 
          reader->parse(input, input + strlen(input), &value, &errors))
      {
        for (auto const& preset : value)
        {
          this->CustomPresets.push_back(preset);
        }
      }
    }
  }
  
  void UpdatePresetsList()
  {
    this->Presets.clear();
    this->Presets.insert(this->Presets.end(), 
                         this->BuiltinPresets.begin(), 
                         this->BuiltinPresets.end());
    this->Presets.insert(this->Presets.end(), 
                         this->CustomPresets.begin(), 
                         this->CustomPresets.end());
  }
};

// 单例实现
vtkSmartPointer<vtkSMTransferFunctionPresets> 
  vtkSMTransferFunctionPresets::Instance = nullptr;

vtkSMTransferFunctionPresets* vtkSMTransferFunctionPresets::GetInstance()
{
  if (!Instance)
  {
    Instance = vtkSmartPointer<vtkSMTransferFunctionPresets>::New();
  }
  return Instance;
}

unsigned int vtkSMTransferFunctionPresets::GetNumberOfPresets()
{
  this->Internals->LoadBuiltinPresets();
  this->Internals->LoadCustomPresets();
  this->Internals->UpdatePresetsList();
  return static_cast<unsigned int>(this->Internals->Presets.size());
}

const Json::Value& vtkSMTransferFunctionPresets::GetPreset(unsigned int index)
{
  this->GetNumberOfPresets();  // 确保预设已加载
  return this->Internals->Presets[index];
}

std::string vtkSMTransferFunctionPresets::GetPresetName(unsigned int index)
{
  const Json::Value& preset = this->GetPreset(index);
  return preset.get("Name", "").asString();
}

bool vtkSMTransferFunctionPresets::IsPresetBuiltin(unsigned int index)
{
  this->Internals->LoadBuiltinPresets();
  return index < static_cast<unsigned int>(this->Internals->BuiltinPresets.size());
}

bool vtkSMTransferFunctionPresets::GetPresetHasOpacities(unsigned int index)
{
  const Json::Value& preset = this->GetPreset(index);
  return preset.isMember("Points") && preset["Points"].isArray();
}
```

---

### 步骤 2: 创建分组管理器

#### 文件结构
```
Qt/Components/
├── pqPresetGroupsManager.h
└── pqPresetGroupsManager.cxx
```

#### pqPresetGroupsManager.h

```cpp
#ifndef pqPresetGroupsManager_h
#define pqPresetGroupsManager_h

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>

class pqPresetGroupsManager : public QObject
{
  Q_OBJECT
  
public:
  pqPresetGroupsManager(QObject* parent = nullptr);
  ~pqPresetGroupsManager() override;
  
  // 查询接口
  int numberOfGroups();
  QList<QString> groupNames();
  QList<QString> presetsInGroup(const QString& groupName);
  int presetRankInGroup(const QString& presetName, const QString& groupName);
  
  // 管理接口
  void addToGroup(const QString& groupName, const QString& presetName);
  void removeFromGroup(const QString& groupName, const QString& presetName);
  void removeFromAllGroups(const QString& presetName);
  
  // 加载/保存
  void loadGroups(const QString& json);
  void replaceGroups(const QString& json);
  bool loadGroupsFromSettings();
  
public Q_SLOTS:
  void saveGroupsToSettings();
  
Q_SIGNALS:
  void groupsUpdated();
  
private:
  void clearGroups();
  
  QList<QString> GroupNames;
  QMap<QString, QList<QString>> Groups;
};

#endif
```

#### pqPresetGroupsManager.cxx

```cpp
#include "pqPresetGroupsManager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>

pqPresetGroupsManager::pqPresetGroupsManager(QObject* parent)
  : QObject(parent)
{
  // 加载默认分组
  if (!loadGroupsFromSettings())
  {
    // 如果没有保存的设置，加载默认分组
    QString defaultGroups = R"([
      {
        "groupName": "Default",
        "presets": [
          "Cool to Warm",
          "Viridis",
          "Inferno",
          "Black-Body Radiation",
          "X Ray"
        ]
      }
    ])";
    loadGroups(defaultGroups);
  }
}

int pqPresetGroupsManager::numberOfGroups()
{
  return this->GroupNames.size();
}

QList<QString> pqPresetGroupsManager::groupNames()
{
  return this->GroupNames;
}

QList<QString> pqPresetGroupsManager::presetsInGroup(const QString& groupName)
{
  return this->Groups.value(groupName, QList<QString>());
}

int pqPresetGroupsManager::presetRankInGroup(
  const QString& presetName, const QString& groupName)
{
  QList<QString> presets = this->Groups.value(groupName);
  return presets.indexOf(presetName);  // 返回 -1 如果不存在
}

void pqPresetGroupsManager::addToGroup(
  const QString& groupName, const QString& presetName)
{
  if (!this->Groups.contains(groupName))
  {
    this->GroupNames.append(groupName);
    this->Groups[groupName] = QList<QString>();
  }
  
  if (!this->Groups[groupName].contains(presetName))
  {
    this->Groups[groupName].append(presetName);
    Q_EMIT this->groupsUpdated();
  }
}

void pqPresetGroupsManager::removeFromGroup(
  const QString& groupName, const QString& presetName)
{
  if (this->Groups.contains(groupName))
  {
    this->Groups[groupName].removeAll(presetName);
    Q_EMIT this->groupsUpdated();
  }
}

void pqPresetGroupsManager::removeFromAllGroups(const QString& presetName)
{
  for (auto& presets : this->Groups)
  {
    presets.removeAll(presetName);
  }
  Q_EMIT this->groupsUpdated();
}

void pqPresetGroupsManager::loadGroups(const QString& json)
{
  QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
  if (!doc.isArray()) return;
  
  QJsonArray groupsArray = doc.array();
  
  for (const QJsonValue& groupValue : groupsArray)
  {
    QJsonObject groupObj = groupValue.toObject();
    QString groupName = groupObj["groupName"].toString();
    QJsonArray presetsArray = groupObj["presets"].toArray();
    
    QList<QString> presets;
    for (const QJsonValue& presetValue : presetsArray)
    {
      presets.append(presetValue.toString());
    }
    
    if (!this->Groups.contains(groupName))
    {
      this->GroupNames.append(groupName);
    }
    else
    {
      // 追加模式：合并预设列表
      QList<QString> existing = this->Groups[groupName];
      for (const QString& preset : presets)
      {
        if (!existing.contains(preset))
        {
          existing.append(preset);
        }
      }
      presets = existing;
    }
    
    this->Groups[groupName] = presets;
  }
  
  Q_EMIT this->groupsUpdated();
}

void pqPresetGroupsManager::replaceGroups(const QString& json)
{
  this->clearGroups();
  this->loadGroups(json);
}

void pqPresetGroupsManager::clearGroups()
{
  this->GroupNames.clear();
  this->Groups.clear();
}

bool pqPresetGroupsManager::loadGroupsFromSettings()
{
  QSettings settings("ParaView", "PresetGroups");
  QString json = settings.value("groups", "").toString();
  if (!json.isEmpty())
  {
    this->loadGroups(json);
    return true;
  }
  return false;
}

void pqPresetGroupsManager::saveGroupsToSettings()
{
  QJsonArray groupsArray;
  
  for (const QString& groupName : this->GroupNames)
  {
    QJsonObject groupObj;
    groupObj["groupName"] = groupName;
    
    QJsonArray presetsArray;
    for (const QString& preset : this->Groups[groupName])
    {
      presetsArray.append(preset);
    }
    groupObj["presets"] = presetsArray;
    
    groupsArray.append(groupObj);
  }
  
  QJsonDocument doc(groupsArray);
  QSettings settings("ParaView", "PresetGroups");
  settings.setValue("groups", QString(doc.toJson()));
}
```

---

### 步骤 3: 创建预设渲染器

#### 文件结构
```
Qt/Components/
├── pqPresetToPixmap.h
└── pqPresetToPixmap.cxx
```

#### pqPresetToPixmap.h

```cpp
#ifndef pqPresetToPixmap_h
#define pqPresetToPixmap_h

#include <QObject>
#include <QPixmap>
#include <QSize>
#include <QScopedPointer>
#include <vtk_jsoncpp_fwd.h>

class vtkScalarsToColors;
class vtkPiecewiseFunction;

class pqPresetToPixmap : public QObject
{
  Q_OBJECT
  
public:
  pqPresetToPixmap(QObject* parent = nullptr);
  ~pqPresetToPixmap() override;
  
  // 渲染预设为 QPixmap
  QPixmap render(const Json::Value& preset, const QSize& resolution) const;
  
protected:
  // 渲染连续色彩映射
  QPixmap renderColorTransferFunction(
    vtkScalarsToColors* stc, 
    vtkPiecewiseFunction* pf, 
    const QSize& resolution) const;
  
  // 渲染离散色彩映射
  QPixmap renderIndexedColorTransferFunction(
    vtkScalarsToColors* stc, 
    const QSize& resolution) const;
  
private:
  class pqInternals;
  QScopedPointer<pqInternals> Internals;
};

#endif
```


#### pqPresetToPixmap.cxx

```cpp
#include "pqPresetToPixmap.h"
#include "pqActiveObjects.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkNew.h"
#include "vtkPVDiscretizableColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkSMPropertyHelper.h"
#include "vtkSMSession.h"
#include "vtkSMSessionProxyManager.h"
#include "vtkSMTransferFunctionPresets.h"
#include "vtkSMTransferFunctionProxy.h"
#include "vtkSmartPointer.h"
#include "vtkUnsignedCharArray.h"

#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

class pqPresetToPixmap::pqInternals
{
public:
  vtkSmartPointer<vtkSMProxy> PVLookupTable;
  vtkSmartPointer<vtkSMProxy> PiecewiseFunction;
  
  pqInternals()
  {
    vtkSMSessionProxyManager* pxm = pqActiveObjects::instance().proxyManager();
    
    // 创建不透明度函数代理
    this->PiecewiseFunction.TakeReference(
      pxm->NewProxy("piecewise_functions", "PiecewiseFunction"));
    this->PiecewiseFunction->SetLocation(vtkSMSession::CLIENT);
    this->PiecewiseFunction->UpdateVTKObjects();
    
    // 创建颜色查找表代理
    this->PVLookupTable.TakeReference(
      pxm->NewProxy("lookup_tables", "PVLookupTable"));
    this->PVLookupTable->SetLocation(vtkSMSession::CLIENT);
    vtkSMPropertyHelper(this->PVLookupTable, "ScalarOpacityFunction")
      .Set(this->PiecewiseFunction);
  }
  
  vtkSMProxy* lookupTable() const
  {
    this->PVLookupTable->ResetPropertiesToXMLDefaults();
    this->PVLookupTable->UpdateVTKObjects();
    return this->PVLookupTable;
  }
  
  vtkSMProxy* piecewiseFunction() const
  {
    this->PiecewiseFunction->ResetPropertiesToXMLDefaults();
    this->PiecewiseFunction->UpdateVTKObjects();
    return this->PiecewiseFunction;
  }
};

pqPresetToPixmap::pqPresetToPixmap(QObject* parent)
  : QObject(parent)
  , Internals(new pqPresetToPixmap::pqInternals())
{
}

pqPresetToPixmap::~pqPresetToPixmap() = default;

QPixmap pqPresetToPixmap::render(const Json::Value& preset, const QSize& resolution) const
{
  if (resolution.width() <= 0 || resolution.height() <= 0)
  {
    return QPixmap();
  }
  
  pqInternals& internals = (*this->Internals);
  vtkSMProxy* lutProxy = internals.lookupTable();
  
  // 应用预设到查找表
  vtkSMTransferFunctionProxy::ApplyPreset(lutProxy, preset);
  vtkSMTransferFunctionProxy::RescaleTransferFunction(lutProxy, 1, 100, false);
  
  vtkScalarsToColors* stc = 
    vtkScalarsToColors::SafeDownCast(lutProxy->GetClientSideObject());
  
  // 判断是离散还是连续色彩映射
  if (stc->GetIndexedLookup())
  {
    return this->renderIndexedColorTransferFunction(stc, resolution);
  }
  else
  {
    vtkPiecewiseFunction* pf = nullptr;
    auto presets = vtkSMTransferFunctionPresets::GetInstance();
    
    // 如果预设包含不透明度信息
    if (presets->GetPresetHasOpacities(preset))
    {
      vtkSMProxy* pfProxy = internals.piecewiseFunction();
      vtkSMTransferFunctionProxy::ApplyPreset(pfProxy, preset);
      vtkSMTransferFunctionProxy::RescaleTransferFunction(pfProxy, 1, 100, false);
      pf = vtkPiecewiseFunction::SafeDownCast(pfProxy->GetClientSideObject());
    }
    
    return this->renderColorTransferFunction(stc, pf, resolution);
  }
}

QPixmap pqPresetToPixmap::renderColorTransferFunction(
  vtkScalarsToColors* stc, vtkPiecewiseFunction* pf, const QSize& resolution) const
{
  // 采样数量（最多256个点）
  int numSamples = std::min(256, std::max(2, resolution.width()));
  
  vtkNew<vtkFloatArray> data;
  data->SetNumberOfTuples(numSamples);
  
  const double* range = stc->GetRange();
  int isUsingLog = stc->UsingLogScale();
  double lrange[2];
  
  if (isUsingLog)
  {
    lrange[0] = log10(range[0]);
    lrange[1] = log10(range[1]);
  }
  
  // 在范围内均匀采样
  for (vtkIdType cc = 0, max = numSamples; cc < max; ++cc)
  {
    double normVal = static_cast<double>(cc) / (max - 1);
    double val;
    
    if (isUsingLog)
    {
      double lval = lrange[0] + normVal * (lrange[1] - lrange[0]);
      val = pow(10.0, lval);
    }
    else
    {
      val = (range[1] - range[0]) * normVal + range[0];
    }
    
    data->SetValue(cc, val);
  }
  
  // 映射到颜色
  vtkSmartPointer<vtkUnsignedCharArray> colors;
  colors.TakeReference(vtkUnsignedCharArray::SafeDownCast(
    stc->MapScalars(data.GetPointer(), VTK_COLOR_MODE_MAP_SCALARS, 0)));
  
  // 创建 QImage
  QImage image(numSamples, 1, QImage::Format_RGB888);
  for (int cc = 0; cc < numSamples; ++cc)
  {
    unsigned char* ptr = colors->GetPointer(4 * cc);
    image.setPixel(cc, 0, qRgb(ptr[0], ptr[1], ptr[2]));
  }
  
  // 如果有不透明度函数，绘制不透明度曲线
  if (pf)
  {
    image = image.scaled(image.width(), resolution.height());
    QPixmap pixmap = QPixmap::fromImage(image);
    
    QPainterPath path;
    path.moveTo(0, 0);
    
    for (vtkIdType cc = 0, max = numSamples; cc < max; cc += 10)
    {
      float x = data->GetValue(cc);
      int y = static_cast<int>(resolution.height() * (1.0 - pf->GetValue(x)));
      path.lineTo(cc, y);
    }
    
    QPainter painter(&pixmap);
    painter.setPen(Qt::black);
    QPen pen = painter.pen();
    pen.setWidth(2);
    painter.strokePath(path, pen);
    
    return pixmap;
  }
  else
  {
    // 缩放到目标分辨率
    image = image.scaled(resolution);
    return QPixmap::fromImage(image);
  }
}

QPixmap pqPresetToPixmap::renderIndexedColorTransferFunction(
  vtkScalarsToColors* stc, const QSize& size) const
{
  vtkPVDiscretizableColorTransferFunction* dct =
    vtkPVDiscretizableColorTransferFunction::SafeDownCast(stc);
  
  int numSwatches = dct->GetNumberOfIndexedColorsInFullSet();
  if (numSwatches < 1)
  {
    return QPixmap();
  }
  
  // 为每个索引颜色添加注释（必需）
  dct->ResetAnnotationsInFullSet();
  for (int cc = 0; cc < numSwatches; cc++)
  {
    dct->SetAnnotationInFullSet(vtkVariant(cc), "");
  }
  dct->Build();
  
  // 创建 QPixmap
  QPixmap palette(size);
  QPainter painter(&palette);
  
  // 清空背景
  painter.fillRect(0, 0, size.width(), size.height(), QColor("white"));
  
  // 计算色块布局
  int swatchSize = std::min(size.width() / numSwatches, size.height());
  
  // 绘制色块
  for (int i = 0; i < numSwatches; ++i)
  {
    double rgba[4];
    dct->GetIndexedColorInFullSet(i, rgba);
    
    QColor color = QColor::fromRgbF(rgba[0], rgba[1], rgba[2]);
    painter.fillRect(i * swatchSize, 0, swatchSize, size.height(), color);
  }
  
  return palette;
}
```

---

### 步骤 4: 创建 UI 界面

#### 文件结构
```
Qt/ApplicationComponents/
├── Resources/UI/
│   └── pqColorOpacityEditorWidget.ui
├── pqColorOpacityEditorWidget.h
└── pqColorOpacityEditorWidget.cxx
```

#### pqColorOpacityEditorWidget.ui (关键部分)

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>ColorOpacityEditorWidget</class>
 <widget class="QWidget" name="ColorOpacityEditorWidget">
  <layout class="QVBoxLayout" name="mainLayout">
   
   <!-- 预设选择下拉框 -->
   <item>
    <widget class="QComboBox" name="DefaultPresetsComboBox">
     <property name="toolTip">
      <string>Select a color map from default presets</string>
     </property>
    </widget>
   </item>
   
   <!-- 其他控件... -->
   
  </layout>
 </widget>
</ui>
```

#### pqColorOpacityEditorWidget.h

```cpp
#ifndef pqColorOpacityEditorWidget_h
#define pqColorOpacityEditorWidget_h

#include "pqPropertyWidget.h"
#include <QWidget>

class vtkSMPropertyGroup;
class vtkSMProxy;

class pqColorOpacityEditorWidget : public pqPropertyWidget
{
  Q_OBJECT
  
public:
  pqColorOpacityEditorWidget(
    vtkSMProxy* proxy, 
    vtkSMPropertyGroup* smgroup, 
    QWidget* parent = nullptr);
  ~pqColorOpacityEditorWidget() override;
  
protected Q_SLOTS:
  // 更新预设列表
  void updateDefaultPresetsList();
  
  // 预设应用后的回调
  void presetApplied();
  
  // 重置下拉框
  void resetColorMapComboBox();
  
private:
  class pqInternals;
  pqInternals* Internals;
};

#endif
```


#### pqColorOpacityEditorWidget.cxx (核心实现)

```cpp
#include "pqColorOpacityEditorWidget.h"
#include "ui_pqColorOpacityEditorWidget.h"
#include "pqPresetGroupsManager.h"
#include "pqPresetToPixmap.h"
#include "pqApplicationCore.h"
#include "vtkSMTransferFunctionPresets.h"
#include "vtkSMTransferFunctionProxy.h"
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPainter>

namespace
{
// 自定义 Delegate 用于渲染下拉框项
class pqColorMapDelegate : public QStyledItemDelegate
{
public:
  pqColorMapDelegate(QObject* parent = nullptr)
    : QStyledItemDelegate(parent)
  {
  }
  
  void paint(QPainter* painter, const QStyleOptionViewItem& option, 
             const QModelIndex& index) const override
  {
    painter->save();
    auto opt = option;
    this->initStyleOption(&opt, index);
    
    // 布局：左半部分显示预览图，右半部分显示文本
    int const pixmapHorizontalMargins = 5;
    auto const pixmapRect = QRect(
      opt.rect.x() + pixmapHorizontalMargins, 
      opt.rect.y() + 0.125 * opt.rect.height(),
      opt.rect.width() / 2 - 2 * pixmapHorizontalMargins, 
      opt.rect.height() * 0.75
    );
    
    auto const textRect = QRect(
      opt.rect.x() + opt.rect.width() / 2, 
      opt.rect.y(), 
      opt.rect.width() / 2, 
      opt.rect.height()
    );
    
    // 处理选中状态
    if (opt.state & QStyle::State_Selected)
    {
      painter->fillRect(opt.rect, opt.palette.color(QPalette::Highlight));
      QPen pen = painter->pen();
      pen.setColor(opt.palette.color(QPalette::HighlightedText));
      painter->setPen(pen);
    }
    else
    {
      painter->fillRect(opt.rect, painter->brush());
    }
    
    // 第一项是占位符，只显示文本
    if (index.row() != 0)
    {
      // 绘制文本
      painter->drawText(QRectF(textRect), Qt::AlignVCenter, index.data().toString());
      
      // 获取预设并渲染为 QPixmap
      auto transferFunctionPresets = vtkSMTransferFunctionPresets::GetInstance();
      QPixmap pixmap = PresetToPixmap.render(
        transferFunctionPresets->GetPreset(index.data(Qt::UserRole).toInt()), 
        opt.rect.size()
      );
      
      // 绘制预览图
      painter->drawPixmap(pixmapRect, pixmap);
    }
    else
    {
      // 占位符只显示文本
      painter->drawText(opt.rect, Qt::AlignVCenter, index.data().toString());
    }
    
    painter->restore();
  }
  
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex&) const override
  {
    return QSize{ option.rect.width(), option.fontMetrics.height() * 2 };
  }
  
private:
  pqPresetToPixmap PresetToPixmap;
};

} // anonymous namespace

// 内部实现类
class pqColorOpacityEditorWidget::pqInternals
{
public:
  Ui::ColorOpacityEditorWidget Ui;
  vtkWeakPointer<vtkSMPropertyGroup> PropertyGroup;
  
  pqInternals(pqColorOpacityEditorWidget* self, vtkSMPropertyGroup* group)
    : PropertyGroup(group)
  {
    this->Ui.setupUi(self);
  }
};

// 构造函数
pqColorOpacityEditorWidget::pqColorOpacityEditorWidget(
  vtkSMProxy* smproxy, vtkSMPropertyGroup* smgroup, QWidget* parentObject)
  : pqPropertyWidget(smproxy, parentObject)
  , Internals(new pqInternals(this, smgroup))
{
  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  
  // 设置自定义 Delegate
  ui.DefaultPresetsComboBox->setItemDelegate(
    new pqColorMapDelegate(ui.DefaultPresetsComboBox));
  
  // 初始化预设列表
  this->updateDefaultPresetsList();
  
  // 连接信号：当用户选择预设时
  QObject::connect(ui.DefaultPresetsComboBox, &QComboBox::currentTextChanged,
    [=](const QString& presetName)
    {
      if (ui.DefaultPresetsComboBox->currentIndex() == 0)
      {
        return;  // 跳过占位符
      }
      
      // 应用预设到传输函数
      bool presetApplied = vtkSMTransferFunctionProxy::ApplyPreset(
        smproxy, presetName.toStdString().c_str());
      
      if (presetApplied)
      {
        Q_EMIT this->presetApplied();
      }
    });
  
  // 监听分组更新
  auto groupManager = qobject_cast<pqPresetGroupsManager*>(
    pqApplicationCore::instance()->manager("PRESET_GROUP_MANAGER"));
  
  this->connect(groupManager, &pqPresetGroupsManager::groupsUpdated, 
                this, &pqColorOpacityEditorWidget::updateDefaultPresetsList);
}

pqColorOpacityEditorWidget::~pqColorOpacityEditorWidget()
{
  delete this->Internals;
  this->Internals = nullptr;
}

// 更新预设列表
void pqColorOpacityEditorWidget::updateDefaultPresetsList()
{
  auto& defaultPresetsComboBox = this->Internals->Ui.DefaultPresetsComboBox;
  auto transferFunctionPresets = vtkSMTransferFunctionPresets::GetInstance();
  auto groupManager = qobject_cast<pqPresetGroupsManager*>(
    pqApplicationCore::instance()->manager("PRESET_GROUP_MANAGER"));
  
  const QString currentPreset = defaultPresetsComboBox->currentText();
  defaultPresetsComboBox->blockSignals(true);
  defaultPresetsComboBox->clear();
  
  // 添加占位符（不可选择）
  defaultPresetsComboBox->addItem(tr("Select a color map from default presets"), -1);
  QStandardItemModel* model = 
    qobject_cast<QStandardItemModel*>(defaultPresetsComboBox->model());
  QStandardItem* item = model->item(0);
  item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
  
  // 获取 "Default" 组中的预设
  QMap<QString, unsigned int> availableGroupPresets;
  for (unsigned int index = 0; index < transferFunctionPresets->GetNumberOfPresets(); ++index)
  {
    auto presetName = QString::fromStdString(
      transferFunctionPresets->GetPresetName(index));
    
    if (groupManager->presetRankInGroup(presetName, "Default") != -1)
    {
      availableGroupPresets[presetName] = index;
    }
  }
  
  // 按照分组中的顺序添加预设
  const auto groupPresets = groupManager->presetsInGroup("Default");
  for (auto const& presetName : groupPresets)
  {
    auto iter = availableGroupPresets.find(presetName);
    if (iter != availableGroupPresets.end())
    {
      defaultPresetsComboBox->addItem(presetName, iter.value());
    }
  }
  
  // 恢复之前的选择
  const int currentPresetIndex = defaultPresetsComboBox->findText(currentPreset);
  defaultPresetsComboBox->setCurrentIndex(currentPresetIndex == -1 ? 0 : currentPresetIndex);
  defaultPresetsComboBox->blockSignals(false);
}

// 重置下拉框（当传输函数被外部修改时）
void pqColorOpacityEditorWidget::resetColorMapComboBox()
{
  auto& defaultPresetsComboBox = this->Internals->Ui.DefaultPresetsComboBox;
  defaultPresetsComboBox->blockSignals(true);
  defaultPresetsComboBox->setCurrentIndex(0);  // 重置为占位符
  defaultPresetsComboBox->blockSignals(false);
}

void pqColorOpacityEditorWidget::presetApplied()
{
  // 触发渲染更新
  // 实现根据具体需求
}
```

---

## 4. 完整代码示例

### 4.1 最小可运行示例

```cpp
// main.cpp
#include <QApplication>
#include <QMainWindow>
#include <QComboBox>
#include "pqPresetGroupsManager.h"
#include "vtkSMTransferFunctionPresets.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  
  // 初始化预设管理器
  auto presetManager = vtkSMTransferFunctionPresets::GetInstance();
  
  // 初始化分组管理器
  pqPresetGroupsManager groupManager;
  
  // 创建主窗口
  QMainWindow window;
  QComboBox* comboBox = new QComboBox(&window);
  
  // 填充预设列表
  const auto presets = groupManager.presetsInGroup("Default");
  for (const QString& presetName : presets)
  {
    comboBox->addItem(presetName);
  }
  
  window.setCentralWidget(comboBox);
  window.show();
  
  return app.exec();
}
```

---

## 5. 关键技术点

### 5.1 单例模式

```cpp
// 确保全局只有一个预设管理器实例
vtkSmartPointer<vtkSMTransferFunctionPresets> Instance = nullptr;

vtkSMTransferFunctionPresets* GetInstance()
{
  if (!Instance)
  {
    Instance = vtkSmartPointer<vtkSMTransferFunctionPresets>::New();
  }
  return Instance;
}
```

### 5.2 自定义 ItemDelegate

**关键点：**
- 继承 `QStyledItemDelegate`
- 重写 `paint()` 方法自定义绘制
- 重写 `sizeHint()` 方法控制项高度

### 5.3 信号阻塞

```cpp
// 防止在程序更新时触发用户事件
comboBox->blockSignals(true);
comboBox->setCurrentIndex(0);
comboBox->blockSignals(false);
```

### 5.4 VTK 与 Qt 集成

```cpp
// VTK 传输函数 → 采样 → QImage → QPixmap
vtkScalarsToColors* stc = ...;
vtkUnsignedCharArray* colors = stc->MapScalars(data, ...);
QImage image(numSamples, 1, QImage::Format_RGB888);
for (int i = 0; i < numSamples; ++i)
{
  unsigned char* ptr = colors->GetPointer(4 * i);
  image.setPixel(i, 0, qRgb(ptr[0], ptr[1], ptr[2]));
}
QPixmap pixmap = QPixmap::fromImage(image.scaled(targetSize));
```

### 5.5 JSON 数据持久化

```cpp
// 保存到 QSettings
QJsonDocument doc(jsonArray);
QSettings settings;
settings.setValue("PresetGroups", QString(doc.toJson()));

// 从 QSettings 加载
QString json = settings.value("PresetGroups", "").toString();
QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
```

---

## 6. 构建配置

### CMakeLists.txt

```cmake
# 添加 Qt 支持
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# 添加 VTK 支持
find_package(VTK REQUIRED)

# 添加 jsoncpp 支持
find_package(jsoncpp REQUIRED)

# 源文件
set(SOURCES
  vtkSMTransferFunctionPresets.cxx
  pqPresetGroupsManager.cxx
  pqPresetToPixmap.cxx
  pqColorOpacityEditorWidget.cxx
)

set(HEADERS
  vtkSMTransferFunctionPresets.h
  pqPresetGroupsManager.h
  pqPresetToPixmap.h
  pqColorOpacityEditorWidget.h
)

set(UI_FILES
  pqColorOpacityEditorWidget.ui
)

# Qt MOC 和 UIC
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# 创建库
add_library(ColorMapPresetSelector
  ${SOURCES}
  ${MOC_SOURCES}
  ${UI_HEADERS}
)

target_link_libraries(ColorMapPresetSelector
  Qt5::Core
  Qt5::Widgets
  ${VTK_LIBRARIES}
  jsoncpp
)
```

---

## 7. 测试与调试

### 7.1 单元测试示例

```cpp
#include <gtest/gtest.h>
#include "vtkSMTransferFunctionPresets.h"

TEST(PresetManagerTest, LoadPresets)
{
  auto manager = vtkSMTransferFunctionPresets::GetInstance();
  ASSERT_GT(manager->GetNumberOfPresets(), 0);
}

TEST(PresetManagerTest, GetPresetName)
{
  auto manager = vtkSMTransferFunctionPresets::GetInstance();
  std::string name = manager->GetPresetName(0);
  ASSERT_FALSE(name.empty());
}
```

### 7.2 调试技巧

```cpp
// 打印所有预设名称
auto manager = vtkSMTransferFunctionPresets::GetInstance();
for (unsigned int i = 0; i < manager->GetNumberOfPresets(); ++i)
{
  qDebug() << "Preset" << i << ":" 
           << QString::fromStdString(manager->GetPresetName(i));
}

// 打印分组信息
auto groupManager = new pqPresetGroupsManager();
for (const QString& groupName : groupManager->groupNames())
{
  qDebug() << "Group:" << groupName;
  for (const QString& preset : groupManager->presetsInGroup(groupName))
  {
    qDebug() << "  -" << preset;
  }
}
```

---

## 8. 常见问题与解决方案

### 问题 1: 预设列表为空

**原因：** 内置预设 JSON 未正确嵌入

**解决：**
```cpp
// 确保 vtkSMTransferFunctionPresetsColorMapsJSON() 函数存在
// 使用 CMake 的 vtkEncodeString 将 JSON 文件嵌入代码
```

### 问题 2: 预览图不显示

**原因：** VTK 代理对象未正确初始化

**解决：**
```cpp
// 确保在 Qt 应用初始化后创建 VTK 对象
vtkSMSessionProxyManager* pxm = pqActiveObjects::instance().proxyManager();
if (!pxm)
{
  qWarning() << "ProxyManager not initialized!";
}
```

### 问题 3: 下拉框项高度不正确

**原因：** ItemDelegate 的 sizeHint 未正确实现

**解决：**
```cpp
QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex&) const override
{
  // 高度为字体高度的 2 倍
  return QSize{ option.rect.width(), option.fontMetrics.height() * 2 };
}
```

---

## 9. 扩展功能

### 9.1 添加搜索功能

```cpp
// 在 UI 中添加搜索框
QLineEdit* searchBox = new QLineEdit();
connect(searchBox, &QLineEdit::textChanged, [=](const QString& text)
{
  for (int i = 0; i < comboBox->count(); ++i)
  {
    bool match = comboBox->itemText(i).contains(text, Qt::CaseInsensitive);
    comboBox->setItemData(i, !match, Qt::UserRole + 1);  // 隐藏不匹配项
  }
});
```

### 9.2 添加收藏功能

```cpp
void addToFavorites(const QString& presetName)
{
  auto groupManager = getGroupManager();
  groupManager->addToGroup("Favorites", presetName);
  groupManager->saveGroupsToSettings();
}
```

### 9.3 导出预设

```cpp
void exportPreset(const QString& presetName, const QString& filename)
{
  auto manager = vtkSMTransferFunctionPresets::GetInstance();
  int idx = -1;
  const Json::Value& preset = manager->GetFirstPresetWithName(
    presetName.toStdString().c_str(), idx);
  
  if (idx != -1)
  {
    Json::StreamWriterBuilder builder;
    std::ofstream file(filename.toStdString());
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(preset, &file);
  }
}
```

---

## 10. 总结

### 核心要点

1. **四层架构**：UI 层 → 分组管理层 → 预设存储层 → 渲染引擎层
2. **单例模式**：确保预设管理器全局唯一
3. **自定义渲染**：使用 QStyledItemDelegate 实现预览图显示
4. **VTK 集成**：通过传输函数生成颜色映射预览
5. **持久化**：使用 JSON + QSettings 保存用户配置

### 实现清单

- [x] vtkSMTransferFunctionPresets (预设管理)
- [x] pqPresetGroupsManager (分组管理)
- [x] pqPresetToPixmap (预览渲染)
- [x] pqColorMapDelegate (自定义绘制)
- [x] pqColorOpacityEditorWidget (UI 集成)
- [x] JSON 数据结构
- [x] CMake 构建配置

### 参考资源

- ParaView 源码：https://gitlab.kitware.com/paraview/paraview
- VTK 文档：https://vtk.org/doc/
- Qt 文档：https://doc.qt.io/

---

**文档完成！** 🎉

按照这个指南，你可以完整复刻 ParaView 的颜色映射预设选择器功能。
