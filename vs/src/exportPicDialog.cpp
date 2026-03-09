#include "exportPicDialog.h"
#include "ui_exportPicDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <algorithm>
#include <cmath>

#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageData.h>
#include <vtkImageResize.h>

ExportPicDialog::ExportPicDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExportPicDialog)
    , mainRenderWindow(nullptr)
    , meridionalRenderWindow(nullptr)
    , bladeToBladeRenderWindow(nullptr){

    ui->setupUi(this);
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    ui->pathLineEdit->setText(defaultPath);

    connect(ui->browseButton, &QPushButton::clicked, this, &ExportPicDialog::onBrowseClicked);
    connect(ui->resolutionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ExportPicDialog::onResolutionChanged);
    connect(ui->exportButton, &QPushButton::clicked, this, &ExportPicDialog::onExportClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &ExportPicDialog::onCancelClicked);
}

ExportPicDialog::~ExportPicDialog()
{
    delete ui;
}

void ExportPicDialog::setRenderWindows(vtkRenderWindow* mainWindow,
                                        vtkRenderWindow* meridionalWindow,
                                        vtkRenderWindow* bladeToBladeWindow)
{
    mainRenderWindow = mainWindow;
    meridionalRenderWindow = meridionalWindow;
    bladeToBladeRenderWindow = bladeToBladeWindow;
    
    ui->mainViewRadio->setEnabled(mainRenderWindow != nullptr);
    ui->meridionalViewRadio->setEnabled(meridionalRenderWindow != nullptr);
    ui->bladeToBladeViewRadio->setEnabled(bladeToBladeRenderWindow != nullptr);
    
    if (!mainRenderWindow && meridionalRenderWindow) {
        ui->meridionalViewRadio->setChecked(true);
    } else if (!mainRenderWindow && !meridionalRenderWindow && bladeToBladeRenderWindow) {
        ui->bladeToBladeViewRadio->setChecked(true);
    }
}

void ExportPicDialog::onBrowseClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Save Directory",
                                                     ui->pathLineEdit->text(),
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->pathLineEdit->setText(dir);
    }
}


void ExportPicDialog::onResolutionChanged(int index)
{
    bool isCustom = (index == 1);
    ui->widthLabel->setEnabled(isCustom);
    ui->widthSpinBox->setEnabled(isCustom);
    ui->heightLabel->setEnabled(isCustom);
    ui->heightSpinBox->setEnabled(isCustom);
}


void ExportPicDialog::onExportClicked()
{
    vtkRenderWindow* currentWindow = getCurrentRenderWindow();
    if (!currentWindow) {
        QMessageBox::warning(this, "Error", "No render window available for the selected view!");
        return;
    }
    
    QString filePath = getFullFilePath();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please specify a file name!");
        return;
    }
    
    if (performExport()) {
        QMessageBox::information(this, "Inform",
            QString("Image Saved to:\n%1").arg(filePath));
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to export image!");
    }
}

void ExportPicDialog::onCancelClicked()
{
    reject();
}

QString ExportPicDialog::getFullFilePath() const
{
    QString path = ui->pathLineEdit->text();
    QString fileName = ui->fileNameLineEdit->text();
    
    if (fileName.isEmpty()) {
        fileName = "screenshot";
    }
    
    QString format = ui->formatComboBox->currentText().toLower();
    QString fullPath = path + "/" + fileName + "." + format;
    
    return fullPath;
}

void ExportPicDialog::getCurrentResolution(int& width, int& height) const
{
    int index = ui->resolutionComboBox->currentIndex();
    
    if (index == 0) {
        vtkRenderWindow* currentWindow = getCurrentRenderWindow();
        if (currentWindow) {
            int* size = currentWindow->GetSize();
            width = size[0];
            height = size[1];
        } else {
            width = 1920;
            height = 1080;
        }
    } else {
        width = ui->widthSpinBox->value();
        height = ui->heightSpinBox->value();
    }
}

vtkRenderWindow* ExportPicDialog::getCurrentRenderWindow() const
{
    if (ui->mainViewRadio->isChecked()) {
        return mainRenderWindow;
    } else if (ui->meridionalViewRadio->isChecked()) {
        return meridionalRenderWindow;
    } else if (ui->bladeToBladeViewRadio->isChecked()) {
        return bladeToBladeRenderWindow;
    }
    return nullptr;
}

bool ExportPicDialog::performExport()
{
    try {
        // 获取当前选择的渲染窗口
        vtkRenderWindow* currentWindow = getCurrentRenderWindow();
        if (!currentWindow) {
            qCritical() << "No render window selected!";
            return false;
        }
        
        // 获取目标分辨率
        int targetWidth, targetHeight;
        getCurrentResolution(targetWidth, targetHeight);
        
        // 获取当前窗口大小
        int* currentSize = currentWindow->GetSize();
        int currentWidth = currentSize[0];
        int currentHeight = currentSize[1];
        
        // 获取视图名称用于调试
        QString viewName = "Unknown";
        if (ui->mainViewRadio->isChecked()) viewName = "3D View";
        else if (ui->meridionalViewRadio->isChecked()) viewName = "Meridional View";
        else if (ui->bladeToBladeViewRadio->isChecked()) viewName = "Blade-to-Blade View";
        
        qDebug() << "Exporting" << viewName;
        qDebug() << "Current window size:" << currentWidth << "x" << currentHeight;
        qDebug() << "Target image size:" << targetWidth << "x" << targetHeight;
        
        // 创建 vtkWindowToImageFilter
        vtkSmartPointer<vtkWindowToImageFilter> windowToImage = 
            vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImage->SetInput(currentWindow);
        windowToImage->SetInputBufferTypeToRGB();
        // 捕获图像（使用 scale=1，不放大）
        windowToImage->Update();
        
        // 准备最终输出的数据
        vtkAlgorithmOutput* finalOutput = windowToImage->GetOutputPort();
        vtkSmartPointer<vtkImageResize> resizer;
        
        // 如果需要调整到指定尺寸
        int resolutionIndex = ui->resolutionComboBox->currentIndex();
        if (resolutionIndex != 0) {
            // 检查是否需要缩放
            if (targetWidth != currentWidth || targetHeight != currentHeight) {
                qDebug() << "Resizing from" << currentWidth << "x" << currentHeight 
                         << "to" << targetWidth << "x" << targetHeight;
                
                resizer = vtkSmartPointer<vtkImageResize>::New();
                resizer->SetInputConnection(windowToImage->GetOutputPort());
                resizer->SetOutputDimensions(targetWidth, targetHeight, 1);
                resizer->SetResizeMethodToOutputDimensions();
                resizer->InterpolateOn();
                resizer->Update();
                
                finalOutput = resizer->GetOutputPort();
                
                qDebug() << "Image resized to exactly" << targetWidth << "x" << targetHeight;
            } else {
                qDebug() << "Target size matches current window size, no resize needed";
            }
        }
        
        // 根据格式选择写入器
        QString format = ui->formatComboBox->currentText();
        QString filePath = getFullFilePath();
        
        vtkSmartPointer<vtkImageWriter> writer;
        
        if (format == "PNG") {
            auto pngWriter = vtkSmartPointer<vtkPNGWriter>::New();
            writer = pngWriter;
        } else if (format == "JPEG") {
            auto jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();
            writer = jpegWriter;
        } else {
            qWarning() << "Unsupported format:" << format;
            return false;
        }
        
        writer->SetFileName(filePath.toStdString().c_str());
        writer->SetInputConnection(finalOutput);
        writer->Write();
        
        qDebug() << "Image exported to:" << filePath;
        return true;
        
    } catch (const std::exception& e) {
        qCritical() << "Exception during export:" << e.what();
        return false;
    } catch (...) {
        qCritical() << "Unknown exception during export";
        return false;
    }
}
