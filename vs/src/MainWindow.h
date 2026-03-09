#pragma once

#include <QMainWindow>

#include <string>
#include <vector>

#include "vtkDisplayWindow.h"
#include "SelectBoundaryDialog.h"
#include "ColorBarDialog.h"
#include "IsoSurfaceDialog.h"
#include "CutplaneDialog.h"
#include "ControlPanel.h"
#include "ConstHeightPlaneDialog.h"
#include "ConstSettingDialog.h"
#include "ProbePanel.h"
#include "exportPicDialog.h"
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkPropPicker.h>
#include <vtkRendererCollection.h>
#include <vtkDataSetMapper.h>
#include <vtkPointLocator.h>
#include <vtkIdList.h>
#include <vtkPointData.h>
#include <vtkTextActor.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkArrayCalculator.h>
#include <vtkGlyph2D.h>
#include <vtkGlyphSource2D.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QButtonGroup>
#include <QSplitter>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct WindowState
    {

    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    // 更新探测面板数据
    void UpdateProbePanel(const std::string& coordinates, 
                          const std::vector<std::pair<std::string, double>>& data);
    
    // 重置所有视图和渲染器，清除之前模型的残留
    void ResetViewsAndRenderers();
    void BladeToBladePlaneButtonTriggered();

    // Check if mouse click is on ScalarBar (for double-click to open ColorBarDialog)
    bool IsClickOnScalarBar();
    bool IsClickOnCutplaneScalarBar();

    void contourSettingButtonTriggered();
    void slicesSettingButtonTriggered();

private slots:
    void on_actionLoadMesh_triggered();
    void on_actionUpdateFlow_triggered();
    void on_actionNewWindow_triggered();
    void on_actionOpenFile_triggered();
    void on_actionAddScalarBarActor_triggered();
    void on_actionAddAxisActor_triggered();
    void on_actionAddOutlineActor_triggered();
    void on_actionAddPointInformation_triggered();
    void on_actionExport_Picture_triggered();
    void on_ConstHeightCheckBox_toggled(bool trigger);

    void xoyViewTriggered();
    void xozViewTriggered();
    void yozViewTriggered();
    void xyzViewTriggered();

    void meshCheckBoxTriggered();
    void contourCheckBoxTriggered();
    void shadeCheckBoxTriggered();
    void edgeCheckBoxTriggered();
    void transparancyCheckBoxTriggered();
    void lightingCheckBoxTriggered();

    void setColorBar(double,double,int,int,double,double);

    void selectBoundaryButtonTriggeded();
    void showBoundaryActor(int,int,bool);
    void deleteSlice(int cutplaneIndex);

    void isoSurfaceCheckBoxTriggered();
    void isoSurfaceSettingButtonTriggered();
    void isoSurfaceChangeFlowNumber(int);
    void isoSurfaceValueChanged(double);
    void changeFloodNumber(int);
    
    void slicesCheckBoxTriggered();
    void changeCutplane(double*,double*,int);
    void makeNewCutplane(double* origin, double* normal);
    void updateCutplaneColorMapping(double minValue, double maxValue, int numberOfColors, bool isBanded);
    void showCutplane(int, bool);

    void ConstHeightButtonTriggered();
    void AddConstHeightPlane(double height);
    void ChangeConstHeightFlow(int flow);
    
    // Blade-to-blade slots
    void AddBladeToBladePlane(double span);
    void ChangeBladeToBladePlaneFlow(int flow);
    void onSpanSliderChanged(int value);
    
    // Periodic copy slots
    void onCopyZoneChanged(int index);
    void onPeriodicCopyRequested();
    
    void onProbePanelClosed();

    //* View Control 
    void on_Check_3Dview_toggled(bool checked);
    void on_Check_ThreeView_toggled(bool checked);
    void on_Check_Meri_toggled(bool checked);
    void on_Check_BladeToBlade_toggled(bool checked);
    
    //* Background Control
    void on_CBtn_BackGround_currentTextChanged(const QString &text);
    
    //* Turbo Initialize
    void on_Btn_TurboInitial_clicked();

private:
    Ui::MainWindow *ui;
    vtkDisplayWindow *qtvtkWindow;
    SelectBoundaryDialog *selectBoundaryDialog = nullptr;
    ColorBarDialog *colorBarDialog = nullptr;
    IsoSurfaceDialog *isoSurfaceDialog = nullptr;
    CutplaneDialog *cutPlaneDialog = nullptr;

    // Meridional视图相关
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> MeridionalrenderWindow= vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> Meridionalrenderer= vtkSmartPointer<vtkRenderer>::New();
    QVTKOpenGLNativeWidget *vtkWidget= new QVTKOpenGLNativeWidget(this);
    
    // Blade-to-Blade视图相关
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> BladeToBladerenderWindow= vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> BladeToBladerenderer= vtkSmartPointer<vtkRenderer>::New();
    QVTKOpenGLNativeWidget *bladeToBladevtkWidget= new QVTKOpenGLNativeWidget(this);
    
    // 视图标题标签
    QLabel *mainViewLabel = nullptr;
    QLabel *meridionalViewLabel = nullptr;
    QLabel *bladeToBladeViewLabel = nullptr;

    // 视图布局管理
    QSplitter *mainSplitter = nullptr;
    QWidget *rightPanel = nullptr;
    QVBoxLayout *rightLayout = nullptr;

    // 视图容器
    QWidget *mainViewContainer = nullptr;
    QWidget *meridionalViewContainer = nullptr;
    QWidget *bladeToBladeViewContainer = nullptr;
    
    // 探测面板
    ProbePanel *probePanel = nullptr;

    void InitializeMainWindow();
    void SetIcons();
    void SetvtkBox();
    void ResetScrollArea();
    void DisableScrollArea();
    
    // 视图管理辅助方法
    void SetupMeridionalView();
    void SetupBladeToBladeView();
    void SetViewBackground(vtkSmartPointer<vtkRenderer> renderer, const QString &style);
    void HideAllViews();
    void ShowMainView();
    void ShowMeridionalView();
    void ShowBladeToBladeView();
    void CreateViewLabels();
    void UpdateViewLabels();

    std::vector<QCheckBox*> periodicCopyBoundaryChecks;
    void updatePeriodicCopyBoundaryList(int zoneIndex);

    //* test
    void CreateCutPreview();
    ControlPanel* controlPanel = nullptr;
    
    // 保存boundary透明度状态
    std::vector<std::vector<double>> savedBoundaryTransparencies;
};