#pragma once

#include <QDialog>
#include <QString>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>

namespace Ui {
class ExportPicDialog;
}

class ExportPicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportPicDialog(QWidget *parent = nullptr);
    ~ExportPicDialog();

    // 设置要导出的 VTK 渲染窗口（多个视图）
    void setRenderWindows(vtkRenderWindow* mainWindow,
                          vtkRenderWindow* meridionalWindow,
                          vtkRenderWindow* bladeToBladeWindow);

private slots:
    void onBrowseClicked();
    void onResolutionChanged(int index);
    void onExportClicked();
    void onCancelClicked();

private:
    Ui::ExportPicDialog *ui;
    
    vtkRenderWindow* mainRenderWindow;
    vtkRenderWindow* meridionalRenderWindow;
    vtkRenderWindow* bladeToBladeRenderWindow;
    
    QString getFullFilePath() const;
    
    void getCurrentResolution(int& width, int& height) const;
    
    vtkRenderWindow* getCurrentRenderWindow() const;
    
    bool performExport();
};
