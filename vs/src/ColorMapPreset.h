#pragma once

#include <QString>
#include <QPixmap>
#include <QColor>
#include <vector>
#include <string>

#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>

/**
 * @brief 单个颜色映射预设的数据结构
 */
struct ColorMapData
{
    QString name;                   // 颜色映射名称
    QString colorSpace;             // 颜色空间 (RGB, Lab, Diverging, HSV, etc.)
    std::vector<double> rgbPoints;  // RGBPoints: [pos, r, g, b, pos, r, g, b, ...]
    QColor nanColor;                // NaN 值的颜色
    bool isDefault;                 // 是否为默认映射
    
    ColorMapData() : isDefault(false), nanColor(Qt::yellow) {}
};

/**
 * @brief 颜色映射预设管理类
 * 
 * 负责从 JSON 文件加载颜色映射预设，生成预览图，
 * 并提供接口将颜色映射应用到 VTK LookupTable
 */
class ColorMapPreset
{
public:
    static ColorMapPreset& instance();
    
    /**
     * @brief 从 JSON 文件加载颜色映射预设
     * @param jsonFilePath JSON 文件路径
     * @return 是否加载成功
     */
    bool loadFromJson(const QString& jsonFilePath);
    
    /**
     * @brief 获取所有已加载的颜色映射数量
     */
    int count() const { return static_cast<int>(m_colorMaps.size()); }
    
    /**
     * @brief 获取指定索引的颜色映射名称
     */
    QString getName(int index) const;
    
    /**
     * @brief 获取指定索引的颜色映射数据
     */
    const ColorMapData* getColorMap(int index) const;
    
    /**
     * @brief 根据名称获取颜色映射索引
     * @return 索引，未找到返回 -1
     */
    int getIndexByName(const QString& name) const;
    
    /**
     * @brief 生成颜色映射的预览图
     * @param index 颜色映射索引
     * @param width 预览图宽度
     * @param height 预览图高度
     * @param reverse 是否反转颜色
     * @return 预览图 pixmap
     */
    QPixmap generatePreview(int index, int width = 200, int height = 20, bool reverse = false) const;
    
    /**
     * @brief 将颜色映射应用到 VTK LookupTable
     * @param index 颜色映射索引
     * @param lut 目标 LookupTable
     * @param reverse 是否反转颜色
     * @param numColors 颜色数量 (默认256)
     */
    void applyToLookupTable(int index, vtkLookupTable* lut, bool reverse = false, int numColors = 256) const;
    
    /**
     * @brief 获取默认颜色映射的索引列表
     */
    std::vector<int> getDefaultMapIndices() const;
    
    /**
     * @brief 检查是否已加载
     */
    bool isLoaded() const { return !m_colorMaps.empty(); }

private:
    ColorMapPreset() = default;
    ~ColorMapPreset() = default;
    ColorMapPreset(const ColorMapPreset&) = delete;
    ColorMapPreset& operator=(const ColorMapPreset&) = delete;
    
    /**
     * @brief 根据位置插值获取颜色
     * @param colorMap 颜色映射数据
     * @param t 归一化位置 [0, 1]
     * @param reverse 是否反转
     * @return RGB 颜色
     */
    QColor interpolateColor(const ColorMapData& colorMap, double t, bool reverse = false) const;
    
    /**
     * @brief 解析 RGBPoints，提取控制点
     */
    struct ControlPoint {
        double position;
        double r, g, b;
    };
    std::vector<ControlPoint> parseControlPoints(const ColorMapData& colorMap) const;
    
    std::vector<ColorMapData> m_colorMaps;
};
