#pragma once

#include <QDialog>
#include <QColor>

#include <string>
#include <vector>

#include "vtkAesReader.h"

namespace Ui {
class ColorBarDialog;
}

class ColorBarDialog: public QDialog
{
    Q_OBJECT
    
public:
    ColorBarDialog(QWidget *parent = nullptr);
    ~ColorBarDialog();

    void setColorBarDialog(std::vector<vtkAesReader::FlowData> flows, int flowNumber);

private slots:
    void setParameters();
    void changeFlowNumber(int);
    
    // Legend控制槽函数
    void onShowLegendToggled(bool checked);
    void onOrientationChanged();
    void onShowTitleToggled(bool checked);
    void onTitleModeChanged(int index);
    void onTextColorClicked();
    void onPositionOrSizeChanged();
    
    // 颜色映射控制槽函数
    void onColorMapPresetChanged(int index);
    void onReverseColorMapToggled(bool checked);

signals:
    void finishSetParameters(double, double, int, int, double, double);
    
    // 新增信号用于Legend控制
    void legendVisibilityChanged(bool visible);
    void legendOrientationChanged(bool isVertical);
    void legendPositionChanged(double x, double y);
    void legendSizeChanged(double width, double height);
    void legendTitleVisibilityChanged(bool visible);
    void legendTitleTextChanged(const QString& title, bool useVariableName);
    void legendTextColorChanged(double r, double g, double b);
    void legendFontChanged(const QString& family, int size, bool bold, bool italic);
    
    // 颜色映射变化信号 - 现在传递预设索引而不是硬编码的 schemeIndex
    void colorMapChanged(int flowNumber, int presetIndex, bool reverse);

public:
    double m;
    double M;
    double width;
    double height;

private:
    Ui::ColorBarDialog *ui;
    std::vector<vtkAesReader::FlowData> flowInfo;
    double rangeM;
    double rangem;
    int colorNumber;
    
    // Legend相关状态
    QColor currentTextColor;
    
    // 颜色映射相关
    int currentColorMapIndex;
    
    // 初始化函数
    void initializeLegendControls();
    void connectLegendSignals();
    void initializeColorMapPresets();
    void updateColorMapPreviewIcons();
};
