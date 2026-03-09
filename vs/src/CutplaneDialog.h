#pragma once

#include <QDialog>
#include <QColor>

#include <vector>
#include <vtkSmartPointer.h>
#include <vtkPlane.h>
#include "vtkAesReader.h"

namespace Ui {
class CutplaneDialog;
}

struct FlowData
{
    std::vector<double> datas;
    std::string name;
    double range[2];
    vtkSmartPointer<vtkScalarBarActor> scalarBar;
    double boundaryRange[2];
};

class CutplaneDialog: public QDialog
{
    Q_OBJECT
    
public:
    CutplaneDialog(QWidget *parent = nullptr);
    ~CutplaneDialog();

    void setCutplaneDialog(std::vector<vtkSmartPointer<vtkPlane>> planes);
    void setModelBounds(double* bounds);
    void setFlowVariables(const std::vector<vtkAesReader::FlowData>& flows, int currentFlowNumber);
    void updateRangeFromCurrentVariable();
    void setMappingControlEnabled(bool enabled);

private slots:
    void setParameters();
    void createSignal();
    void changeCutplaneNumber(int number);

    void onSliceLocationChanged(int index); // 处理平面类型变化
    void onSliderValueChanged(int value);   // 处理滑块值变化
    void onExtractSliceClicked();           // 处理提取切片按钮点击
    void onColorMapPresetChanged(int index);   // 处理颜色映射预设变化
    void onReverseColorMapToggled(bool checked); // 处理反转颜色映射
    void onVariableSelectionChanged(int index); // 处理变量选择变化
    void on_Btn_SetPosition_clicked();
    void on_Btn_AddNew_clicked();

    void on_Check_Banded_toggled(bool checked);
    void on_Check_Continuous_toggled(bool checked);


signals:
    void finishSetParameters(double*, double*, int);
    void createNewCutplane(double*, double*);
    void sliceLocation(double,int);
    void colorMappingChanged(double minValue, double maxValue, int numberOfColors,bool isBanded);
    void colorSchemeChanged(int presetIndex, bool reverse);  // 颜色方案变化信号
    void variableSelectionChanged(int flowNumber); // 变量选择变化信号

private:
    Ui::CutplaneDialog *ui;

    std::vector<double*> origin;
    std::vector<double*> normal;
    double curOrigin[3];
    double curNormal[3];
    bool createFlag = false;

    double modelBounds[6]; // [xmin, xmax, ymin, ymax, zmin, zmax]
    int currentAxis;       // 当前选择的轴：0=X轴, 1=Y轴, 2=Z轴
    
    // 流场变量相关
    std::vector<vtkAesReader::FlowData> flowVariables;
    int currentFlowNumber; // 当前选择的流场变量编号
    
    // 颜色映射相关
    int currentColorMapIndex;
    void initializeColorMapPresets();
    void updateColorMapPreviewIcons();
    
    // 设置滑块范围和当前值
    void updateSliderRange();
    // 根据滑块值更新标签显示
    void updateValueLabel(double value);
    // 根据滑块值设置切面位置
    void setPlanePosition(double value);
    void onColorMappingChanged();
    bool isBaned = 1;
};