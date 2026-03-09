#include "ColorMapPreset.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>

#include <algorithm>
#include <cmath>

ColorMapPreset& ColorMapPreset::instance()
{
    static ColorMapPreset instance;
    return instance;
}

bool ColorMapPreset::loadFromJson(const QString& jsonFilePath)
{
    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[ColorMapPreset] Failed to open file:" << jsonFilePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "[ColorMapPreset] JSON parse error:" << parseError.errorString();
        return false;
    }
    
    if (!doc.isArray()) {
        qWarning() << "[ColorMapPreset] JSON root is not an array";
        return false;
    }
    
    m_colorMaps.clear();
    QJsonArray array = doc.array();
    
    for (const QJsonValue& value : array) {
        if (!value.isObject()) continue;
        
        QJsonObject obj = value.toObject();
        
        // 只加载有 RGBPoints 的颜色映射
        if (!obj.contains("RGBPoints")) continue;
        
        ColorMapData colorMap;
        colorMap.name = obj["Name"].toString();
        colorMap.colorSpace = obj["ColorSpace"].toString("RGB");
        colorMap.isDefault = obj["DefaultMap"].toBool(false);
        
        // 解析 NanColor
        if (obj.contains("NanColor")) {
            QJsonArray nanArray = obj["NanColor"].toArray();
            if (nanArray.size() >= 3) {
                colorMap.nanColor = QColor::fromRgbF(
                    nanArray[0].toDouble(),
                    nanArray[1].toDouble(),
                    nanArray[2].toDouble()
                );
            }
        }
        
        // 解析 RGBPoints
        QJsonArray rgbArray = obj["RGBPoints"].toArray();
        colorMap.rgbPoints.reserve(rgbArray.size());
        for (const QJsonValue& v : rgbArray) {
            colorMap.rgbPoints.push_back(v.toDouble());
        }
        
        // 至少需要 8 个值 (2个控制点，每个 4 个值: pos, r, g, b)
        if (colorMap.rgbPoints.size() >= 8) {
            m_colorMaps.push_back(std::move(colorMap));
        }
    }
    
    qDebug() << "[ColorMapPreset] Loaded" << m_colorMaps.size() << "color maps from" << jsonFilePath;
    return !m_colorMaps.empty();
}

QString ColorMapPreset::getName(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_colorMaps.size())) {
        return QString();
    }
    return m_colorMaps[index].name;
}

const ColorMapData* ColorMapPreset::getColorMap(int index) const
{
    if (index < 0 || index >= static_cast<int>(m_colorMaps.size())) {
        return nullptr;
    }
    return &m_colorMaps[index];
}

int ColorMapPreset::getIndexByName(const QString& name) const
{
    for (int i = 0; i < static_cast<int>(m_colorMaps.size()); ++i) {
        if (m_colorMaps[i].name == name) {
            return i;
        }
    }
    return -1;
}

std::vector<ColorMapPreset::ControlPoint> ColorMapPreset::parseControlPoints(const ColorMapData& colorMap) const
{
    std::vector<ControlPoint> points;
    
    // RGBPoints 格式: [pos, r, g, b, pos, r, g, b, ...]
    for (size_t i = 0; i + 3 < colorMap.rgbPoints.size(); i += 4) {
        ControlPoint cp;
        cp.position = colorMap.rgbPoints[i];
        cp.r = colorMap.rgbPoints[i + 1];
        cp.g = colorMap.rgbPoints[i + 2];
        cp.b = colorMap.rgbPoints[i + 3];
        points.push_back(cp);
    }
    
    // 按位置排序
    std::sort(points.begin(), points.end(), 
        [](const ControlPoint& a, const ControlPoint& b) {
            return a.position < b.position;
        });
    
    return points;
}

QColor ColorMapPreset::interpolateColor(const ColorMapData& colorMap, double t, bool reverse) const
{
    if (reverse) {
        t = 1.0 - t;
    }
    
    auto points = parseControlPoints(colorMap);
    if (points.empty()) {
        return Qt::black;
    }
    
    if (points.size() == 1) {
        return QColor::fromRgbF(
            qBound(0.0, points[0].r, 1.0),
            qBound(0.0, points[0].g, 1.0),
            qBound(0.0, points[0].b, 1.0)
        );
    }
    
    // 归一化位置到 [0, 1]
    double minPos = points.front().position;
    double maxPos = points.back().position;
    double normalizedT = minPos + t * (maxPos - minPos);
    
    // 找到插值区间
    size_t i = 0;
    for (; i < points.size() - 1; ++i) {
        if (normalizedT <= points[i + 1].position) {
            break;
        }
    }
    
    if (i >= points.size() - 1) {
        i = points.size() - 2;
    }
    
    const ControlPoint& p1 = points[i];
    const ControlPoint& p2 = points[i + 1];
    
    // 计算局部插值参数
    double localT = 0.0;
    if (std::abs(p2.position - p1.position) > 1e-10) {
        localT = (normalizedT - p1.position) / (p2.position - p1.position);
    }
    localT = qBound(0.0, localT, 1.0);
    
    // 线性插值
    double r = p1.r + localT * (p2.r - p1.r);
    double g = p1.g + localT * (p2.g - p1.g);
    double b = p1.b + localT * (p2.b - p1.b);
    
    return QColor::fromRgbF(
        qBound(0.0, r, 1.0),
        qBound(0.0, g, 1.0),
        qBound(0.0, b, 1.0)
    );
}

QPixmap ColorMapPreset::generatePreview(int index, int width, int height, bool reverse) const
{
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::white);
    
    const ColorMapData* colorMap = getColorMap(index);
    if (!colorMap) {
        return pixmap;
    }
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    // 逐列绘制颜色
    for (int x = 0; x < width; ++x) {
        double t = static_cast<double>(x) / (width - 1);
        QColor color = interpolateColor(*colorMap, t, reverse);
        painter.setPen(color);
        painter.drawLine(x, 0, x, height);
    }
    
    painter.end();
    return pixmap;
}

void ColorMapPreset::applyToLookupTable(int index, vtkLookupTable* lut, bool reverse, int numColors) const
{
    if (!lut) {
        qWarning() << "[ColorMapPreset] LookupTable is null";
        return;
    }
    
    const ColorMapData* colorMap = getColorMap(index);
    if (!colorMap) {
        qWarning() << "[ColorMapPreset] Invalid color map index:" << index;
        return;
    }
    
    // 保存当前范围
    double range[2];
    lut->GetTableRange(range);
    
    // 设置颜色数量
    lut->SetNumberOfTableValues(numColors);
    
    // 填充颜色表
    for (int i = 0; i < numColors; ++i) {
        double t = static_cast<double>(i) / (numColors - 1);
        QColor color = interpolateColor(*colorMap, t, reverse);
        
        lut->SetTableValue(i, 
            color.redF(), 
            color.greenF(), 
            color.blueF(), 
            1.0);
    }
    
    // 恢复范围
    lut->SetTableRange(range);
    
    // 设置 NaN 颜色
    lut->SetNanColor(
        colorMap->nanColor.redF(),
        colorMap->nanColor.greenF(),
        colorMap->nanColor.blueF(),
        1.0
    );
    
    lut->Build();
    
    qDebug() << "[ColorMapPreset] Applied color map:" << colorMap->name 
             << "reverse:" << reverse << "numColors:" << numColors;
}

std::vector<int> ColorMapPreset::getDefaultMapIndices() const
{
    std::vector<int> indices;
    for (int i = 0; i < static_cast<int>(m_colorMaps.size()); ++i) {
        if (m_colorMaps[i].isDefault) {
            indices.push_back(i);
        }
    }
    return indices;
}
