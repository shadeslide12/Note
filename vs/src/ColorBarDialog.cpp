#include "ColorBarDialog.h"
#include "ui_ColorBarDialog.h"
#include "ColorMapPreset.h"

#include <QString>
#include <QColorDialog>
#include <QCoreApplication>
#include <QDir>
#include <stdlib.h>
using std::string;
using std::vector;
using std::to_string;
using std::stod; using std::stoi;
#include <iostream>
using std::cout; using std::endl;

#include <vtkLookupTable.h>
#include <exception>

ColorBarDialog::ColorBarDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ColorBarDialog)
    , currentTextColor(Qt::black)
    , currentColorMapIndex(0)
{
    ui->setupUi(this);
    
    // 连接原有信号
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(setParameters()));
    connect(ui->nameComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFlowNumber(int)));
    
    // 初始化并连接Legend控件
    initializeLegendControls();
    connectLegendSignals();
    
    // 初始化颜色映射预设
    initializeColorMapPresets();
    
    // 连接颜色映射ComboBox
    ui->comboColorMapPreset->blockSignals(true);
    ui->checkReverseColorMap->blockSignals(true);
    
    connect(ui->comboColorMapPreset, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorBarDialog::onColorMapPresetChanged);
    connect(ui->checkReverseColorMap, &QCheckBox::toggled, 
            this, &ColorBarDialog::onReverseColorMapToggled);
    
    ui->comboColorMapPreset->blockSignals(false);
    ui->checkReverseColorMap->blockSignals(false);
    
    cout << "create colorbar dialog" << endl;
}

ColorBarDialog::~ColorBarDialog()
{
    cout << "destroy colorbar dialog" << endl;
    delete ui;
}

void ColorBarDialog::initializeLegendControls()
{
    // 设置默认文字颜色为黑色
    currentTextColor = Qt::black;
    ui->btnTextColor->setStyleSheet("QPushButton { background-color: black; color: white; }");
    ui->btnTextColor->setText("Black");
    
    // 设置默认位置（右上角）
    ui->textLegendXPosition->setText("90");
    ui->textLegendYPosition->setText("10");
    
    // 设置默认方向为垂直
    ui->radioVertical->setChecked(true);
    
    // 设置默认显示标题
    ui->checkShowTitle->setChecked(true);
    
    // 设置默认标题模式为使用变量名
    ui->comboTitleMode->setCurrentIndex(0);
    ui->textTitleText->setEnabled(false);
}

void ColorBarDialog::connectLegendSignals()
{
    // 1. Show Legend控制
    connect(ui->checkShowLegend, &QCheckBox::toggled, 
            this, &ColorBarDialog::onShowLegendToggled);
    
    // 2. 方向控制
    connect(ui->radioVertical, &QRadioButton::toggled, 
            this, &ColorBarDialog::onOrientationChanged);
    connect(ui->radioHorizontal, &QRadioButton::toggled, 
            this, &ColorBarDialog::onOrientationChanged);
    
    // 3. 标题控制
    connect(ui->checkShowTitle, &QCheckBox::toggled, 
            this, &ColorBarDialog::onShowTitleToggled);
    connect(ui->comboTitleMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ColorBarDialog::onTitleModeChanged);
    
    // 3.1 自定义标题文本输入（当用户编辑完成时）
    connect(ui->textTitleText, &QLineEdit::editingFinished,
            [this]() {
                if (ui->comboTitleMode->currentIndex() == 1) {  // Use Custom Text mode
                    QString customTitle = ui->textTitleText->text();
                    if (!customTitle.isEmpty()) {
                        emit legendTitleTextChanged(customTitle, false);
                    }
                }
            });
    
    // 4. 文字颜色控制
    connect(ui->btnTextColor, &QPushButton::clicked, 
            this, &ColorBarDialog::onTextColorClicked);
    
    // 5. 位置和大小变化（当用户编辑完成时）
    connect(ui->textLegendXPosition, &QLineEdit::editingFinished, 
            this, &ColorBarDialog::onPositionOrSizeChanged);
    connect(ui->textLegendYPosition, &QLineEdit::editingFinished, 
            this, &ColorBarDialog::onPositionOrSizeChanged);
    connect(ui->textScalarBarWidth, &QLineEdit::editingFinished, 
            this, &ColorBarDialog::onPositionOrSizeChanged);
    connect(ui->textScalarBarHeight, &QLineEdit::editingFinished, 
            this, &ColorBarDialog::onPositionOrSizeChanged);
    
    // 6. 字体变化
    connect(ui->comboFontFamily, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this]() {
                emit legendFontChanged(
                    ui->comboFontFamily->currentText(),
                    ui->spinFontSize->value(),
                    ui->checkBold->isChecked(),
                    ui->checkItalic->isChecked()
                );
            });
    
    connect(ui->spinFontSize, QOverload<int>::of(&QSpinBox::valueChanged),
            [this]() {
                emit legendFontChanged(
                    ui->comboFontFamily->currentText(),
                    ui->spinFontSize->value(),
                    ui->checkBold->isChecked(),
                    ui->checkItalic->isChecked()
                );
            });
    
    connect(ui->checkBold, &QCheckBox::toggled,
            [this]() {
                emit legendFontChanged(
                    ui->comboFontFamily->currentText(),
                    ui->spinFontSize->value(),
                    ui->checkBold->isChecked(),
                    ui->checkItalic->isChecked()
                );
            });
    
    connect(ui->checkItalic, &QCheckBox::toggled,
            [this]() {
                emit legendFontChanged(
                    ui->comboFontFamily->currentText(),
                    ui->spinFontSize->value(),
                    ui->checkBold->isChecked(),
                    ui->checkItalic->isChecked()
                );
            });
}

// ==================== Legend控制槽函数实现 ====================

void ColorBarDialog::onShowLegendToggled(bool checked)
{
    cout << "Legend visibility changed: " << (checked ? "visible" : "hidden") << endl;
    emit legendVisibilityChanged(checked);
}

void ColorBarDialog::onOrientationChanged()
{
    bool isVertical = ui->radioVertical->isChecked();
    cout << "Legend orientation changed: " << (isVertical ? "Vertical" : "Horizontal") << endl;
    emit legendOrientationChanged(isVertical);
}

void ColorBarDialog::onShowTitleToggled(bool checked)
{
    cout << "Legend title visibility changed: " << (checked ? "visible" : "hidden") << endl;
    
    // 控制标题相关控件的启用状态
    ui->comboTitleMode->setEnabled(checked);
    ui->labelTitleMode->setEnabled(checked);
    
    if (checked && ui->comboTitleMode->currentIndex() == 1) {
        ui->textTitleText->setEnabled(true);
        ui->labelTitleText->setEnabled(true);
    } else {
        ui->textTitleText->setEnabled(false);
        ui->labelTitleText->setEnabled(false);
    }
    
    emit legendTitleVisibilityChanged(checked);
}

void ColorBarDialog::onTitleModeChanged(int index)
{
    // index 0: Use Variable Name
    // index 1: Use Custom Text
    bool useCustomText = (index == 1);
    
    cout << "Title mode changed: " << (useCustomText ? "Use Custom Text" : "Use Variable Name") << endl;
    
    // 控制自定义文本输入框的启用状态
    ui->textTitleText->setEnabled(useCustomText && ui->checkShowTitle->isChecked());
    ui->labelTitleText->setEnabled(useCustomText && ui->checkShowTitle->isChecked());
    
    if (useCustomText) {
        // 使用自定义文本
        QString customTitle = ui->textTitleText->text();
        if (customTitle.isEmpty()) {
            customTitle = "Custom Title";
            ui->textTitleText->setText(customTitle);  // 将默认值写回到文本框
        }
        emit legendTitleTextChanged(customTitle, false);
    } else {
        // 使用变量名
        QString varName = ui->nameComboBox->currentText();
        emit legendTitleTextChanged(varName, true);
    }
}

void ColorBarDialog::onTextColorClicked()
{
    // 打开颜色选择对话框
    QColor color = QColorDialog::getColor(currentTextColor, this, "Select Text Color");
    
    if (color.isValid()) {
        currentTextColor = color;
        
        // 更新按钮样式
        QString colorName = color.name();
        QString textColor = (color.lightness() > 128) ? "black" : "white";
        ui->btnTextColor->setStyleSheet(
            QString("QPushButton { background-color: %1; color: %2; }")
            .arg(colorName).arg(textColor)
        );
        ui->btnTextColor->setText(colorName);
        
        // 发送信号（转换为0-1范围的RGB值）
        double r = color.redF();
        double g = color.greenF();
        double b = color.blueF();
        
        cout << "Text color changed: RGB(" << r << ", " << g << ", " << b << ")" << endl;
        emit legendTextColorChanged(r, g, b);
    }
}

void ColorBarDialog::onPositionOrSizeChanged()
{
    try {
        // 读取位置（百分比，需要转换为0-1范围）
        double xPos = stod(ui->textLegendXPosition->text().toStdString()) / 100.0;
        double yPos = stod(ui->textLegendYPosition->text().toStdString()) / 100.0;
        
        // 读取大小（已经是0-1范围）
        double w = stod(ui->textScalarBarWidth->text().toStdString());
        double h = stod(ui->textScalarBarHeight->text().toStdString());
        
        // 验证范围
        if (xPos < 0.0 || xPos > 1.0 || yPos < 0.0 || yPos > 1.0) {
            cout << "Warning: Position values should be between 0 and 100" << endl;
            return;
        }
        
        if (w < 0.01 || w > 1.0 || h < 0.01 || h > 1.0) {
            cout << "Warning: Size values should be between 0.01 and 1.0" << endl;
            return;
        }
        
        cout << "Legend position changed: (" << xPos << ", " << yPos << ")" << endl;
        cout << "Legend size changed: (" << w << ", " << h << ")" << endl;
        
        emit legendPositionChanged(xPos, yPos);
        emit legendSizeChanged(w, h);
        
    } catch (const std::exception& e) {
        cout << "Error parsing position/size values: " << e.what() << endl;
    }
}

// ==================== 原有函数 ====================

void ColorBarDialog::setColorBarDialog(vector<vtkAesReader::FlowData> flows, int flowNumber)
{
    auto flow = flows[flowNumber];
    flowInfo = flows;
    rangem = flow.range[0];
    rangeM = flow.range[1];
    m = flow.scalarBar->GetLookupTable()->GetRange()[0];
    M = flow.scalarBar->GetLookupTable()->GetRange()[1];
    
    // 设置默认ScalarBar尺寸（相对于窗口的比例，范围0-1）
    width = 0.1;
    height = 0.8;
    
    // 清空并重新填充变量列表
    // CRITICAL: Block signals to prevent currentIndexChanged from firing during update
    ui->nameComboBox->blockSignals(true);
    ui->nameComboBox->clear();
    for (auto &x : flows)
    {
        ui->nameComboBox->addItem(x.name.c_str());
    }
    ui->nameComboBox->setCurrentIndex(flowNumber);
    ui->nameComboBox->blockSignals(false);
    
    string rangeText = "(" + to_string(rangem) + ", " + to_string(rangeM) + ")";
    ui->textParameterRange->setText(rangeText.c_str());
    ui->textColorBarmin->setText(to_string(m).c_str());
    ui->textColorBarMax->setText(to_string(M).c_str());
    ui->textColorNumber->setText(to_string(flow.scalarBar->GetNumberOfLabels()).c_str());
    ui->textScalarBarWidth->setText(QString::number(width, 'f', 2));
    ui->textScalarBarHeight->setText(QString::number(height, 'f', 2));
}

void ColorBarDialog::changeFlowNumber(int number)
{
    auto flow = flowInfo[number];
    rangem = flow.range[0];
    rangeM = flow.range[1];
    m = flow.range[0];
    M = flow.range[1];

    string rangeText = "(" + to_string(rangem) + ", " + to_string(rangeM) + ")";
    ui->textParameterRange->setText(rangeText.c_str());
    ui->textColorBarmin->setText(to_string(m).c_str());
    ui->textColorBarMax->setText(to_string(M).c_str());
    ui->textColorNumber->setText(to_string(flow.scalarBar->GetNumberOfLabels()).c_str());
    
    // 如果标题模式是使用变量名，更新标题
    if (ui->comboTitleMode->currentIndex() == 0) {
        emit legendTitleTextChanged(ui->nameComboBox->currentText(), true);
    }
    
    emit(finishSetParameters(m, M, flow.scalarBar->GetNumberOfLabels(), ui->nameComboBox->currentIndex(), width, height));
}

void ColorBarDialog::setParameters()
{
    try
    {
        m = stod(ui->textColorBarmin->text().toStdString());
        M = stod(ui->textColorBarMax->text().toStdString());
        colorNumber = stoi(ui->textColorNumber->text().toStdString());
        width = stod(ui->textScalarBarWidth->text().toStdString());
        height = stod(ui->textScalarBarHeight->text().toStdString());
    }
    catch (const std::exception& e)
    {
        cout << "catch exception while setting parameters, please check." << endl << 
        "exception is: " << e.what() << endl;
        this->close();
        return;
    }

    if ((colorNumber < 2 || colorNumber > 256) || (m > M))
    {
        cout << "error set colorbar parameters, please check and set again!" << endl;
        this->close();
        return;
    }
    
    // 验证尺寸参数（相对尺寸，范围应该在0.01到1.0之间）
    if (width < 0.01 || width > 1.0 || height < 0.01 || height > 1.0)
    {
        cout << "error: ScalarBar size must be between 0.01 and 1.0 (relative to window size)" << endl;
        this->close();
        return;
    }
    
    emit(finishSetParameters(m, M, colorNumber, ui->nameComboBox->currentIndex(), width, height));
}


// ==================== 颜色映射控制槽函数实现 ====================

void ColorBarDialog::initializeColorMapPresets()
{
    ColorMapPreset& preset = ColorMapPreset::instance();
    
    // 尝试从多个可能的路径加载 JSON 文件
    QString appDir = QCoreApplication::applicationDirPath();
    QString currentDir = QDir::currentPath();
    
    QStringList searchPaths = {
        // 相对于可执行文件
        appDir + "/ColorMaps.json",
        appDir + "/../ColorMaps.json",
        appDir + "/../src/ColorMaps.json",
        // 相对于当前工作目录
        currentDir + "/ColorMaps.json",
        currentDir + "/src/ColorMaps.json",
    };
    
    cout << "[ColorBarDialog] Searching for ColorMaps.json..." << endl;
    cout << "[ColorBarDialog] App dir: " << appDir.toStdString() << endl;
    cout << "[ColorBarDialog] Current dir: " << currentDir.toStdString() << endl;
    
    bool loaded = false;
    for (const QString& path : searchPaths) {
        if (QFile::exists(path)) {
            loaded = preset.loadFromJson(path);
            if (loaded) {
                cout << "[ColorBarDialog] Loaded color maps from: " << path.toStdString() << endl;
                break;
            }
        }
    }
    
    if (!loaded) {
        cout << "[ColorBarDialog] Warning: Could not load ColorMaps.json, using default presets" << endl;
    }
    
    // 填充 ComboBox
    ui->comboColorMapPreset->clear();
    
    if (preset.isLoaded()) {
        // 只显示默认的颜色映射
        auto defaultIndices = preset.getDefaultMapIndices();
        
        for (int idx : defaultIndices) {
            QString name = preset.getName(idx);
            QPixmap preview = preset.generatePreview(idx, 200, 20, false);
            ui->comboColorMapPreset->addItem(QIcon(preview), name, idx);
        }
        
        cout << "[ColorBarDialog] Added " << defaultIndices.size() << " default color maps to combo box" << endl;
    } else {
        // 如果加载失败，添加基本预设
        ui->comboColorMapPreset->addItem("Blue to Red (default)", -1);
        ui->comboColorMapPreset->addItem("Viridis", -2);
        ui->comboColorMapPreset->addItem("Rainbow", -3);
        ui->comboColorMapPreset->addItem("Cool-Warm", -4);
    }
    
    // 默认选择第一个
    if (ui->comboColorMapPreset->count() > 0) {
        ui->comboColorMapPreset->setCurrentIndex(0);
        currentColorMapIndex = ui->comboColorMapPreset->itemData(0).toInt();
    }
}

void ColorBarDialog::updateColorMapPreviewIcons()
{
    ColorMapPreset& preset = ColorMapPreset::instance();
    if (!preset.isLoaded()) return;
    
    bool reverse = ui->checkReverseColorMap->isChecked();
    
    for (int i = 0; i < ui->comboColorMapPreset->count(); ++i) {
        int presetIndex = ui->comboColorMapPreset->itemData(i).toInt();
        if (presetIndex >= 0) {
            QPixmap preview = preset.generatePreview(presetIndex, 200, 20, reverse);
            ui->comboColorMapPreset->setItemIcon(i, QIcon(preview));
        }
    }
}

void ColorBarDialog::onColorMapPresetChanged(int comboIndex)
{
    if (comboIndex < 0) return;
    
    // 获取实际的预设索引（存储在 item data 中）
    currentColorMapIndex = ui->comboColorMapPreset->itemData(comboIndex).toInt();
    
    bool reverse = ui->checkReverseColorMap->isChecked();
    int currentFlow = ui->nameComboBox->currentIndex();
    
    QString name = ui->comboColorMapPreset->currentText();
    cout << "[ColorBarDialog] Color map preset changed: " << name.toStdString()
         << ", presetIndex=" << currentColorMapIndex 
         << ", reverse=" << (reverse ? "true" : "false") << endl;
    
    emit colorMapChanged(currentFlow, currentColorMapIndex, reverse);
}

void ColorBarDialog::onReverseColorMapToggled(bool checked)
{
    cout << "[ColorBarDialog] Reverse color map toggled: " << (checked ? "true" : "false") << endl;
    
    // 更新所有预览图标
    updateColorMapPreviewIcons();
    
    // 触发颜色映射更新
    int currentFlow = ui->nameComboBox->currentIndex();
    emit colorMapChanged(currentFlow, currentColorMapIndex, checked);
}
