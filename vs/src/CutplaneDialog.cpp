#include "CutplaneDialog.h"
#include "ui_CutplaneDialog.h"
#include "ColorMapPreset.h"
#include <string>
using std::stod;
using std::string;
#include <iostream>
#include <QButtonGroup>
#include <QCoreApplication>
#include <QDir>
using std::cout;
using std::endl;

using std::vector;
CutplaneDialog::CutplaneDialog(QWidget *parent): QDialog(parent),ui(new Ui::CutplaneDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(setParameters()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCutplaneNumber(int)));
    // connect(ui->Btn_AddNew, SIGNAL(clicked()), this, SLOT(createSignal()));

    // 连接新添加的控件信号
    connect(ui->Combo_SLiceLocation, SIGNAL(currentIndexChanged(int)), this, SLOT(onSliceLocationChanged(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(ui->Combo_SekectMapVariable, SIGNAL(currentIndexChanged(int)), this, SLOT(onVariableSelectionChanged(int)));
    
    // 初始化颜色映射预设（从 JSON 加载）
    currentColorMapIndex = 0;
    initializeColorMapPresets();
    
    // 连接颜色映射 ComboBox 和 Reverse CheckBox
    ui->Combo_ContourType->blockSignals(true);
    ui->checkReverseColorMap->blockSignals(true);
    
    connect(ui->Combo_ContourType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CutplaneDialog::onColorMapPresetChanged);
    connect(ui->checkReverseColorMap, &QCheckBox::toggled,
            this, &CutplaneDialog::onReverseColorMapToggled);
    
    ui->Combo_ContourType->blockSignals(false);
    ui->checkReverseColorMap->blockSignals(false);

    // 初始化
    currentAxis = 0; // 默认为X轴
    currentFlowNumber = 0; // 默认选择第一个流场变量
    // 初始化模型边界值为0
    for (int i = 0; i < 6; i++) {
        modelBounds[i] = 0.0;
    }
    
    // 设置滑块初始范围
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setValue(50);

    updateValueLabel(0.0);

    //* Set Group Of Color Method
    QButtonGroup *group_ColorMethod = new QButtonGroup(this);
    group_ColorMethod->setExclusive(true);
    group_ColorMethod->addButton(ui->Check_Banded);
    group_ColorMethod->addButton(ui->Check_Continuous);
}

CutplaneDialog::~CutplaneDialog()
{
    delete ui;
}

void CutplaneDialog::createSignal()
{
    createFlag = true;
    // emit(createNewCutplane());
}

void CutplaneDialog::setCutplaneDialog(vector<vtkSmartPointer<vtkPlane>> planes)
{
    int n = planes.size();
    
    // 清空现有数据
    ui->comboBox->clear();
    origin.clear();
    normal.clear();
    
    for (int i = 1; i <= n; i++)
    {
        string name = "Slice";
        name += std::to_string(i);
        ui->comboBox->addItem(name.c_str());

        origin.push_back(planes[i-1]->GetOrigin());
        normal.push_back(planes[i-1]->GetNormal());
    }

    // 如果有cutplane数据，初始化第一个
    if (n > 0) {
        ui->comboBox->setCurrentIndex(0);
    }

    updateSliderRange();
}

void CutplaneDialog::changeCutplaneNumber(int number)
{
    if (origin.size() == 0) return;
    curOrigin[0] = origin[0][0];
    curOrigin[1] = origin[0][1];
    curOrigin[2] = origin[0][2];
    curNormal[0] = normal[0][0];
    curNormal[1] = normal[0][1];
    curNormal[2] = normal[0][2];
}

void CutplaneDialog::setParameters()
{
    std::cout<< "[Debug] CurrentIndex is :"<< ui->comboBox->currentIndex() << std::endl;
    onColorMappingChanged();
}

// 新增：设置模型边界值
void CutplaneDialog::setModelBounds(double* bounds)
{
    if (bounds != nullptr) {
        for (int i = 0; i < 6; i++) {
            modelBounds[i] = bounds[i];
        }
        updateSliderRange();
    }
}

// 处理平面类型变化
void CutplaneDialog::onSliceLocationChanged(int index)
{
    currentAxis = index;
    updateSliderRange();
}

// 处理滑块值变化
void CutplaneDialog::onSliderValueChanged(int value)
{
    // 将滑块值（0-100）映射到当前轴的范围内
    double min = 0.0, max = 1.0;
    switch (currentAxis) {
        case 0: // X轴
            min = modelBounds[0];
            max = modelBounds[1];
            break;
        case 1: // Y轴
            min = modelBounds[2];
            max = modelBounds[3];
            break;
        case 2: // Z轴
            min = modelBounds[4];
            max = modelBounds[5];
            break;
    }
    
    double range = max - min;
    double actualValue = min + (range * value / 100.0);
    
    updateValueLabel(actualValue);
    setPlanePosition(actualValue);
    emit sliceLocation(actualValue,currentAxis);
}

// 处理提取切片按钮点击
void CutplaneDialog::onExtractSliceClicked()
{
    // 根据当前设置创建新切面
    createFlag = true;
    emit(createNewCutplane(curOrigin, curNormal));
}

// 更新滑块范围
void CutplaneDialog::updateSliderRange()
{
    // 滑块范围固定为0-100，实际值通过映射计算
    ui->horizontalSlider->setValue(50); // 默认在中间位置
    
    // 更新当前值显示
    onSliderValueChanged(50);
}

// 更新值标签显示
void CutplaneDialog::updateValueLabel(double value)
{
    QString axisName;
    switch (currentAxis) {
        case 0: axisName = "X"; break;
        case 1: axisName = "Y"; break;
        case 2: axisName = "Z"; break;
    }
    
    ui->label_ValueLocation->setText(QString("%1 : %2").arg(axisName).arg(value, 0, 'f', 4));
}

// 根据滑块值设置切面位置
void CutplaneDialog::setPlanePosition(double value)
{
    // 根据当前轴和值设置切面的原点和法向量
    switch (currentAxis) {
        case 0: // X轴 - YZ平面
            curOrigin[0] = value;
            curOrigin[1] = 0.0;
            curOrigin[2] = 0.0;
            curNormal[0] = 1.0;
            curNormal[1] = 0.0;
            curNormal[2] = 0.0;
            break;
        case 1: // Y轴 - XZ平面
            curOrigin[0] = 0.0;
            curOrigin[1] = value;
            curOrigin[2] = 0.0;
            curNormal[0] = 0.0;
            curNormal[1] = 1.0;
            curNormal[2] = 0.0;
            break;
        case 2: // Z轴 - XY平面
            curOrigin[0] = 0.0;
            curOrigin[1] = 0.0;
            curOrigin[2] = value;
            curNormal[0] = 0.0;
            curNormal[1] = 0.0;
            curNormal[2] = 1.0;
            break;
    }
    
}


void CutplaneDialog::onColorMappingChanged()
{
    
    bool ok;
    double minValue = ui->LnEdit_Min->text().toDouble(&ok);
    if (!ok) {
        std::cout << "[Warning] Invalid min value: " << ui->LnEdit_Min->text().toStdString() << std::endl;
        return;
    }
    
    double maxValue = ui->LnEdit_Max->text().toDouble(&ok);
    if (!ok) {
        std::cout << "[Warning] Invalid max value: " << ui->LnEdit_Max->text().toStdString() << std::endl;
        return;
    }
    
    int numberOfColors = ui->LnEdit_Numbers->text().toInt(&ok);
    if (!ok || numberOfColors <= 0) {
        std::cout << "[Warning] Invalid number of colors: " << ui->LnEdit_Numbers->text().toStdString() << std::endl;
        return;
    }
    
    if (minValue >= maxValue) {
        std::cout << "[Warning] Min value must be less than max value" << std::endl;
        return;
    }
    
    // 发送颜色映射变化信号（共享给所有cutplane）
    emit colorMappingChanged(minValue, maxValue, numberOfColors,isBaned);
    
    std::cout << "[Debug] Shared cutplane color mapping changed: range[" 
              << minValue << ", " << maxValue 
              << "], colors=" << numberOfColors << std::endl;
}


void CutplaneDialog::onColorMapPresetChanged(int comboIndex)
{
    if (comboIndex < 0) return;
    
    // 获取实际的预设索引（存储在 item data 中）
    currentColorMapIndex = ui->Combo_ContourType->itemData(comboIndex).toInt();
    
    bool reverse = ui->checkReverseColorMap->isChecked();
    
    QString name = ui->Combo_ContourType->currentText();
    std::cout << "[CutplaneDialog] Color map preset changed: " << name.toStdString()
              << ", presetIndex=" << currentColorMapIndex
              << ", reverse=" << (reverse ? "true" : "false") << std::endl;
    
    emit colorSchemeChanged(currentColorMapIndex, reverse);
}

void CutplaneDialog::onReverseColorMapToggled(bool checked)
{
    std::cout << "[CutplaneDialog] Reverse color map toggled: " << (checked ? "true" : "false") << std::endl;
    
    // 更新所有预览图标
    updateColorMapPreviewIcons();
    
    // 触发颜色映射更新
    emit colorSchemeChanged(currentColorMapIndex, checked);
}

void CutplaneDialog::initializeColorMapPresets()
{
    ColorMapPreset& preset = ColorMapPreset::instance();
    
    // 尝试从多个可能的路径加载 JSON 文件
    QString appDir = QCoreApplication::applicationDirPath();
    QString currentDir = QDir::currentPath();
    
    QStringList searchPaths = {
        appDir + "/ColorMaps.json",
        appDir + "/../ColorMaps.json",
        appDir + "/../src/ColorMaps.json",
        currentDir + "/ColorMaps.json",
        currentDir + "/src/ColorMaps.json",
    };
    
    // ColorMapPreset 是单例，可能已由 ColorBarDialog 加载
    if (!preset.isLoaded()) {
        for (const QString& path : searchPaths) {
            if (QFile::exists(path)) {
                if (preset.loadFromJson(path)) {
                    std::cout << "[CutplaneDialog] Loaded color maps from: " << path.toStdString() << std::endl;
                    break;
                }
            }
        }
    }
    
    // 填充 ComboBox
    ui->Combo_ContourType->clear();
    
    if (preset.isLoaded()) {
        auto defaultIndices = preset.getDefaultMapIndices();
        
        for (int idx : defaultIndices) {
            QString name = preset.getName(idx);
            QPixmap preview = preset.generatePreview(idx, 200, 20, false);
            ui->Combo_ContourType->addItem(QIcon(preview), name, idx);
        }
        
        std::cout << "[CutplaneDialog] Added " << defaultIndices.size() << " default color maps to combo box" << std::endl;
    } else {
        // 如果加载失败，添加基本预设
        ui->Combo_ContourType->addItem("Rainbow (default)", -1);
        ui->Combo_ContourType->addItem("Viridis", -2);
        ui->Combo_ContourType->addItem("Gray", -3);
    }
    
    // 默认选择第一个
    if (ui->Combo_ContourType->count() > 0) {
        ui->Combo_ContourType->setCurrentIndex(0);
        currentColorMapIndex = ui->Combo_ContourType->itemData(0).toInt();
    }
}

void CutplaneDialog::updateColorMapPreviewIcons()
{
    ColorMapPreset& preset = ColorMapPreset::instance();
    if (!preset.isLoaded()) return;
    
    bool reverse = ui->checkReverseColorMap->isChecked();
    
    for (int i = 0; i < ui->Combo_ContourType->count(); ++i) {
        int presetIndex = ui->Combo_ContourType->itemData(i).toInt();
        if (presetIndex >= 0) {
            QPixmap preview = preset.generatePreview(presetIndex, 200, 20, reverse);
            ui->Combo_ContourType->setItemIcon(i, QIcon(preview));
        }
    }
}

void CutplaneDialog::on_Btn_SetPosition_clicked() {
    std::cout << "[Debug] Updating existing cutplane at index: " << ui->comboBox->currentIndex() << std::endl;
    emit(finishSetParameters(curOrigin, curNormal, ui->comboBox->currentIndex()));
}

void CutplaneDialog::on_Btn_AddNew_clicked() {
    std::cout << "[Debug] Creating new cutplane with origin: ("
          << curOrigin[0] << ", " << curOrigin[1] << ", " << curOrigin[2]
          << ") normal: (" << curNormal[0] << ", " << curNormal[1] << ", " << curNormal[2] << ")" << std::endl;
    emit(createNewCutplane(curOrigin, curNormal));
}

void CutplaneDialog::on_Check_Banded_toggled(bool checked){
    if (checked)
        isBaned =1 ;
}

void CutplaneDialog::on_Check_Continuous_toggled(bool checked){
    if (checked)
        isBaned =0;
}

// 设置流场变量数据
void CutplaneDialog::setFlowVariables(const std::vector<vtkAesReader::FlowData>& flows, int currentFlowNumber)
{
    flowVariables = flows;
    this->currentFlowNumber = currentFlowNumber;
    
    // 清空并重新填充变量选择ComboBox
    ui->Combo_SekectMapVariable->clear();
    
    for (size_t i = 0; i < flowVariables.size(); i++) {
        ui->Combo_SekectMapVariable->addItem(QString::fromStdString(flowVariables[i].name));
    }
    
    // 设置当前选择的变量
    if (currentFlowNumber >= 0 && currentFlowNumber < static_cast<int>(flowVariables.size())) {
        ui->Combo_SekectMapVariable->setCurrentIndex(currentFlowNumber);
        
        // 根据当前变量更新范围值
        updateRangeFromCurrentVariable();
    }
    
    std::cout << "[Debug] Flow variables set: " << flowVariables.size() 
              << " variables, current: " << currentFlowNumber << std::endl;
}

// 处理变量选择变化
void CutplaneDialog::onVariableSelectionChanged(int index)
{
    if (index >= 0 && index < static_cast<int>(flowVariables.size())) {
        currentFlowNumber = index;
        
        // 更新范围值显示
        updateRangeFromCurrentVariable();
        
        // 发送变量选择变化信号
        emit variableSelectionChanged(currentFlowNumber);
        
        std::cout << "[Debug] Variable selection changed to: " << index 
                  << " (" << flowVariables[index].name << ")" << std::endl;
    }
}

// 根据当前选择的变量更新范围值
void CutplaneDialog::updateRangeFromCurrentVariable()
{
    if (currentFlowNumber >= 0 && currentFlowNumber < static_cast<int>(flowVariables.size())) {
        const vtkAesReader::FlowData& currentFlow = flowVariables[currentFlowNumber];
        
        // 更新最小值和最大值输入框
        ui->LnEdit_Min->setText(QString::number(currentFlow.range[0], 'f', 6));
        ui->LnEdit_Max->setText(QString::number(currentFlow.range[1], 'f', 6));
        
        std::cout << "[Debug] Updated range for " << currentFlow.name 
                  << ": [" << currentFlow.range[0] << ", " << currentFlow.range[1] << "]" << std::endl;
    }
}

void CutplaneDialog::setMappingControlEnabled(bool enabled)
{
    // 启用/禁用MappingControl widget及其所有子控件
    std::cout << "[Debug] Setting mapping control enabled to " << enabled << std::endl;
    switch (enabled) {
        case true:
            ui->MappingControl->setEnabled(1);
        case false:
            ui->MappingControl->setDisabled(1);
    }

    
    std::cout << "[Debug] MappingControl enabled: " << (enabled ? "true" : "false") << std::endl;
}