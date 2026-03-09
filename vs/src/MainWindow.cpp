#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
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
#include <vtkInteractorStyleImage.h>
#include <QFileDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QToolBar>
#include <QMessageBox>

#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkScalarBarRepresentation.h>

using std::string; using std::vector;

// 前向声明
class MainWindow;

namespace
{
    class MouseInteractorStyle: public vtkInteractorStyleTrackballCamera
    {
    public:
        static MouseInteractorStyle* New() { return new MouseInteractorStyle(); }
        MouseInteractorStyle() 
        {
            initialize();
        }
        
        void SetMainWindow(MainWindow* window) { mainWindow = window; }                                                                     

        virtual void OnLeftButtonDoubleClick() override
        {
            vtkInteractorStyleTrackballCamera::OnLeftButtonDoubleClick();
            
            //* Check if double-click is on ScalarBar, trigger ColorBarDialog
            if (mainWindow != nullptr && mainWindow->IsClickOnScalarBar())
            {
                mainWindow->contourSettingButtonTriggered();
                return;
            }
            
            //* Check if double-click is on Cutplane ScalarBar, trigger CutplaneDialog
            if (mainWindow != nullptr && mainWindow->IsClickOnCutplaneScalarBar())
            {
                mainWindow->slicesSettingButtonTriggered();
                return;
            }
        }
        
        virtual void OnLeftButtonDown() override
        {
            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
            
            if (!isAddTextActor) return;
            
            int *pos = this->Interactor->GetEventPosition();

            cellPicker->Pick(pos[0], pos[1], 0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
            propPicker->Pick(pos[0], pos[1], 0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

            if (propPicker->GetActor() != nullptr && propPicker->GetActor() != pointActor)
            {
                auto boundaryData = propPicker->GetActor()->GetMapper()->GetInput();
                auto pos = boundaryData->GetPoint(cellPicker->GetPointId());
                
                // 构建坐标字符串（用于侧边栏）
                std::string coordStr = "x: " + std::to_string(pos[0]) + 
                                      "\ny: " + std::to_string(pos[1]) + 
                                      "\nz: " + std::to_string(pos[2]);
                
                // 收集所有物理量数据
                std::vector<std::pair<std::string, double>> probeData;
                for (int i = 0; i < parameterNames.size(); i++)
                {
                    std::string varName = boundaryData->GetPointData()->GetArray(parameterNames[i].c_str())->GetName();
                    double value = boundaryData->GetPointData()->
                        GetArray(parameterNames[i].c_str())->GetComponent(cellPicker->GetPointId(), 0);
                    probeData.push_back({varName, value});
                }
                
                // 更新侧边栏
                if (mainWindow != nullptr)
                {
                    mainWindow->UpdateProbePanel(coordStr, probeData);
                }
                vtkSmartPointer<vtkPointLocator> locate = vtkSmartPointer<vtkPointLocator>::New();
                locate->SetDataSet(boundaryData);
                vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
                locate->FindClosestNPoints(2, pos, idlist);
                
                vtkSmartPointer<vtkMath> math = vtkSmartPointer<vtkMath>::New();
                double point1[3];
                boundaryData->GetPoint(idlist[0].GetId(0), point1);
                double point2[3];
                boundaryData->GetPoint(idlist[0].GetId(1), point2);
                double dis = sqrt(math->Distance2BetweenPoints(point1, point2)) / 3;
            
                auto displayPoint =  vtkSmartPointer<vtkSphereSource>::New();
                displayPoint->SetCenter(pos);
                displayPoint->SetRadius(dis);

                auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

                mapper->SetInputConnection(displayPoint->GetOutputPort());
                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(pointActor);
                pointActor->SetMapper(mapper);
                this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(pointActor);
            }

            vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
        }
        
        void initialize()
        {
            textActor = vtkSmartPointer<vtkTextActor>::New();
            textActor->SetInput("please select a point");
            textActor->GetProperty()->SetColor(0.0,0,0);
            textActor->GetTextProperty()->BoldOn();
            textActor->GetTextProperty()->SetFontSize(18);

            pointActor = vtkSmartPointer<vtkActor>::New();
            pointActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
            pointActor->VisibilityOff();

        }

        vtkSmartPointer<vtkTextActor> textActor = nullptr;
        vtkSmartPointer<vtkActor> pointActor = nullptr;
        std::vector<std::string> parameterNames = {"rho", "u", "v", "w", "P", "tb1"};
        bool isAddTextActor = false;
        
        vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
        vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
        
        MainWindow* mainWindow = nullptr;  // 指向MainWindow的指针

        vtkSmartPointer<vtkActor> streamLineActor = vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
        vtkDataSet *meshDataSet = nullptr;
        bool hasPoint1Set = false;
        bool hasPoint2Set = false;
        double streamLinePoint1[3];
        double streamLinePoint2[3];
        bool readyToCreateStreamLine = false;
    };
}

static ::MouseInteractorStyle* style = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 加载Post专用蓝色主题
    QFile styleFile("post_blue_theme.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(styleSheet);
        styleFile.close();
        qDebug() << "Post蓝色主题已加载！";
    } else {
        qDebug() << "样式表加载失败，尝试从当前目录加载...";
    }
    
    // 设置菜单栏样式（蓝色渐变）
    if (menuBar()) {
        menuBar()->setStyleSheet(
            "QMenuBar {"
            "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "                                stop:0 #5a9fd4, stop:1 #4a8fc4);"
            "    border-bottom: 2px solid #3d87c2;"
            "    padding: 3px;"
            "    color: white;"
            "}"
            "QMenuBar::item {"
            "    padding: 5px 10px;"
            "    background-color: transparent;"
            "    color: white;"
            "    font-weight: bold;"
            "}"
            "QMenuBar::item:selected {"
            "    background-color: rgba(255, 255, 255, 0.2);"
            "    border-radius: 3px;"
            "}"
            "QMenuBar::item:pressed {"
            "    background-color: rgba(255, 255, 255, 0.3);"
            "}"
        );
        qDebug() << "菜单栏蓝色样式已设置！";
    }
    
    // 设置状态栏样式（蓝色渐变）
    if (statusBar()) {
        statusBar()->setStyleSheet(
            "QStatusBar {"
            "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
            "                                stop:0 #3d87c2, stop:1 #2e6ca6);"
            "    color: white;"
            "    border-top: 2px solid #2e6ca6;"
            "    font-weight: bold;"
            "}"
            "QStatusBar QLabel {"
            "    color: white;"
            "}"
        );
        qDebug() << "状态栏蓝色样式已设置！";
    }
    
    InitializeMainWindow();

}

MainWindow::~MainWindow()
{

    delete qtvtkWindow;
    delete ui;
}

void MainWindow::on_actionLoadMesh_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open File","./",tr("*.grid.hdf"));
    if (filename.length() == 0) 
    {
        cout << "Error choose aes file, please check and try it agian!" << endl;
        return;
    }
    else
    {
        cout << "reading aes grid file " << filename.toStdString() << endl << "please waiting for few seconds!" << endl;
        
        // 在加载新模型前清除之前的视图
        ResetViewsAndRenderers();
        
        on_actionNewWindow_triggered();

        qtvtkWindow->ReadAesFile(filename.toStdString());
        ui->vtkBox->renderWindow()->Render();
    }
    ResetScrollArea();
    cout << "End Load Mesh" << endl;
    double* bounds = qtvtkWindow->GetModelBounds();
    if (bounds != nullptr) {
        qDebug() << "Outline在X轴上的范围：" << "最小值 = " << bounds[0] << ", 最大值 = " << bounds[1];
    }
    
    //* Widget_copy填充 Zone 名称到 combo box
    ui->com_copy->clear();
    auto zoneNames = qtvtkWindow->GetZoneNames();
    for (const auto& name : zoneNames)
    {
        ui->com_copy->addItem(QString::fromStdString(name));
    }
    
    if (!zoneNames.empty())
    {
        qDebug() << "[Zone] Loaded" << zoneNames.size() << "zones";
        qDebug() << "Zone names:" << QString::fromStdString(zoneNames[0]);
        updatePeriodicCopyBoundaryList(0);
    }

    //* Debug
    auto boundaryDatasets = qtvtkWindow->GetBoundaryDatasets();
    for (int zi = 0; zi < (int)boundaryDatasets.size(); ++zi) {
        qDebug() << "Zone" << zi;
        for (int bi = 0; bi < (int)boundaryDatasets[zi].size(); ++bi) {
            qDebug() << "  "
                     << QString::fromLocal8Bit(boundaryDatasets[zi][bi].zoneName.c_str())
                     << "->"
                     << QString::fromLocal8Bit(boundaryDatasets[zi][bi].name.c_str())
                     << "type=" << boundaryDatasets[zi][bi].type;
        }
    }
}

void MainWindow::on_actionUpdateFlow_triggered()
{
    if (!qtvtkWindow->HasGrid())
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    QString filename = QFileDialog::getOpenFileName(this,"Open File","./",tr("*.hdf"));
    if (filename.length() == 0 || filename.toStdString().find("flow") == std::string::npos) 
    {
        cout << "Error choose flow file, please check and try it agian!" << endl;
        return;
    }
    qtvtkWindow->UpdateFlow(filename.toStdString());
    ui->vtkBox->renderWindow()->Render();
    cout << "End Update Flow" << endl;
}

void MainWindow::on_actionNewWindow_triggered()
{
    if (!qtvtkWindow->HasGrid()) 
    {
        cout << "the VTK Box is initialized." << endl;
        return;
    }
    cout << "Initialize the VTK Box" << endl;
    vtkDisplayWindow *newWindow = new vtkDisplayWindow();
    ui->vtkBox->setRenderWindow(newWindow->GetRenderWindow());
    delete qtvtkWindow;
    qtvtkWindow = newWindow;
    ui->vtkBox->renderWindow()->Render();
    if (selectBoundaryDialog != nullptr)
    {
        delete selectBoundaryDialog;
        selectBoundaryDialog = nullptr;
    }
    DisableScrollArea();
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open File","./",tr("*.hdf"));
    if (filename.length() == 0 || (filename.toStdString().find("flow") == std::string::npos &&
     filename.toStdString().find("grid") == std::string::npos))
    {
        cout << "Error choose flow file, please check and try it agian!" << endl;
        return;
    }
    if (filename.toStdString().find("flow") != std::string::npos)
    {
        if (!qtvtkWindow->HasGrid())
        {
            cout << "there is no grid file, please add a grid file first." << endl;
            return;
        }
        qtvtkWindow->UpdateFlow(filename.toStdString());

        ui->vtkBox->renderWindow()->Render();
        cout << "End Update Flow" << endl;
    }
    else if (filename.toStdString().find("grid") != std::string::npos)
    {
        cout << "reading aes grid file " << filename.toStdString() << endl << "please waiting for few seconds!" << endl;
        
        // 在加载新模型前清除之前的视图
        ResetViewsAndRenderers();
        
        on_actionNewWindow_triggered();
        qtvtkWindow->ReadAesFile(filename.toStdString());
        ui->vtkBox->renderWindow()->Render();
        ResetScrollArea();
        cout << "End Load Mesh" << endl;
    }
}

void MainWindow::on_actionAddScalarBarActor_triggered()
{
    if (!qtvtkWindow->HasGrid())
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (qtvtkWindow->IsScalarBarWidgetActive())
    {
        qtvtkWindow->InActivateScalarBarWidget();
    }
    else if (!qtvtkWindow->IsScalarBarWidgetActive())
    {
        qtvtkWindow->ActivateScalarBarWidget();
    }
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::on_actionAddAxisActor_triggered()
{
    if (!qtvtkWindow->HasGrid())
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (qtvtkWindow->IsAxiesWidgetActive())
    {
        qtvtkWindow->InActivateAxiesWidget();
    }
    else qtvtkWindow->ActivateAxiesWidget();
    ui->vtkBox->renderWindow()->Render();    
}

void MainWindow::on_actionAddOutlineActor_triggered()
{
    if (!qtvtkWindow->HasGrid())
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (qtvtkWindow->IsOutlineActorVisiable())
    {
        qtvtkWindow->InVisiableOutlineActor();
    }
    else qtvtkWindow->VisiableOutlineActor();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::on_actionAddPointInformation_triggered()
{
    if (!qtvtkWindow->HasGrid())
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    
    // MouseInteractorStyle已经在InitializeMainWindow中创建，这里只需要切换探测模式
    if (::style == nullptr)
    {
        cerr << "Error: MouseInteractorStyle not initialized!" << endl;
        return;
    }
    
    // 切换探测模式
    if (!::style->isAddTextActor)
    {
        // 启用探测模式
        ::style->isAddTextActor = true;
        ::style->pointActor->VisibilityOn();
        
        // 显示探测面板
        if (probePanel) {
            probePanel->show();
            probePanel->ClearData();
        }
        
        cout << "Probe mode enabled" << endl;
    }
    else
    {
        // 禁用探测模式
        ::style->isAddTextActor = false;
        ::style->pointActor->VisibilityOff();
        
        // 隐藏探测面板
        if (probePanel) {
            probePanel->hide();
        }
        
        cout << "Probe mode disabled" << endl;
    }
    
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::on_actionExport_Picture_triggered()
{
    ExportPicDialog dialog(this);
    
    // 传递三个视图的渲染窗口
    dialog.setRenderWindows(
        ui->vtkBox->renderWindow(),           // 3D 主视图
        MeridionalrenderWindow,                // Meridional 视图
        BladeToBladerenderWindow               // Blade-to-Blade 视图
    );
    
    dialog.exec();
}

void MainWindow::xoyViewTriggered()
{
    qtvtkWindow->xoyViewRender();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::xozViewTriggered()
{
    qtvtkWindow->xozViewRender();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::yozViewTriggered()
{
    qtvtkWindow->yozViewRender();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::xyzViewTriggered()
{
    qtvtkWindow->xyzViewRender();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::meshCheckBoxTriggered()
{
    if (!qtvtkWindow->HasGrid()) 
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (ui->meshCheckBox->isChecked())
    {
        qtvtkWindow->AddMeshActor();
    }
    else qtvtkWindow->RemoveMeshActor();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::contourCheckBoxTriggered()
{
    if (!qtvtkWindow->HasGrid()) 
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (ui->contourCheckBox->isChecked())
    {
        qtvtkWindow->AddContourActor();
        if (ui->shadeCheckBox->isChecked())
        {
            qtvtkWindow->RemoveShadeActor();
        }
    }
    else 
    {
        qtvtkWindow->RemoveContourActor();
        if (ui->shadeCheckBox->isChecked())
        {
            qtvtkWindow->AddShadeActor();
        }
    }
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::shadeCheckBoxTriggered()
{
    if (!qtvtkWindow->HasGrid()) 
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (ui->shadeCheckBox->isChecked())
    {
        if (!ui->contourCheckBox->isChecked())
        {
            qtvtkWindow->AddShadeActor();
        }
    }
    else qtvtkWindow->RemoveShadeActor();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::edgeCheckBoxTriggered()
{
    if (!qtvtkWindow->HasGrid()) 
    {
        cout << "there is no grid file, please add a grid file first." << endl;
        return;
    }
    if (ui->edgeCheckBox->isChecked())
    {
        qtvtkWindow->AddEdgeActor();
        if (qtvtkWindow->IsOutlineActorVisiable())
        {
            qtvtkWindow->InVisiableOutlineActor();
        }
    }
    else 
    {
        qtvtkWindow->RemoveEdgeActor();
        if (!qtvtkWindow->IsOutlineActorVisiable())
        {
            qtvtkWindow->VisiableOutlineActor();
        }
    }
    ui->vtkBox->renderWindow()->Render();
}


void MainWindow::contourSettingButtonTriggered()
{
    // 检查是否已加载网格
    if (!qtvtkWindow)
    {
        cout << "ERROR: qtvtkWindow is nullptr!" << endl;
        return;
    }
    
    if (!qtvtkWindow->HasGrid())
    {
        cout << "ERROR: No grid loaded. Please load a mesh file first." << endl;
        return;
    }
    
    cout << "Grid check passed" << endl;
    
    auto flows = qtvtkWindow->GetFlows();
    cout << "Number of flow variables: " << flows.size() << endl;
    if (flows.empty())
    {
        cout << "ERROR: No flow data available!" << endl;
        return;
    }
    
    // Only create dialog once, reuse it afterwards (preserves user settings)
    if (colorBarDialog == nullptr)
    {
        colorBarDialog = new ColorBarDialog(this);
        colorBarDialog->setColorBarDialog(flows, qtvtkWindow->GetCurFlowNumber());
        
        // 连接原有的参数设置信号
        connect(colorBarDialog, SIGNAL(finishSetParameters(double,double,int,int,double,double)),
                this, SLOT(setColorBar(double,double,int,int,double,double)));
        
        // ==================== 连接Legend控制信号 ====================
        
        // 1. 显示/隐藏ScalarBar
        connect(colorBarDialog, &ColorBarDialog::legendVisibilityChanged,
                [this](bool visible) {
                    qtvtkWindow->SetScalarBarVisibility(visible);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 2. 方向控制
        connect(colorBarDialog, &ColorBarDialog::legendOrientationChanged,
                [this](bool isVertical) {
                    qtvtkWindow->SetScalarBarOrientation(isVertical);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 3. 位置控制
        connect(colorBarDialog, &ColorBarDialog::legendPositionChanged,
                [this](double x, double y) {
                    qtvtkWindow->SetScalarBarPosition(x, y);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 4. 大小控制
        connect(colorBarDialog, &ColorBarDialog::legendSizeChanged,
                [this](double width, double height) {
                    qtvtkWindow->SetScalarBarSize(width, height);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 5. 标题可见性控制
        connect(colorBarDialog, &ColorBarDialog::legendTitleVisibilityChanged,
                [this](bool visible) {
                    if (!visible) {
                        qtvtkWindow->SetScalarBarTitle("");
                    } else {
                        auto flows = qtvtkWindow->GetFlows();
                        int curFlow = qtvtkWindow->GetCurFlowNumber();
                        if (curFlow >= 0 && curFlow < flows.size()) {
                            qtvtkWindow->SetScalarBarTitle(flows[curFlow].name);
                        }
                    }
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 6. 标题文本控制
        connect(colorBarDialog, &ColorBarDialog::legendTitleTextChanged,
                [this](const QString& title, bool useVariableName) {
                    if (useVariableName) {
                        auto flows = qtvtkWindow->GetFlows();
                        int curFlow = qtvtkWindow->GetCurFlowNumber();
                        if (curFlow >= 0 && curFlow < flows.size()) {
                            qtvtkWindow->SetScalarBarTitle(flows[curFlow].name);
                        }
                    } else {
                        qtvtkWindow->SetScalarBarTitle(title.toStdString());
                    }
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 7. 文字颜色控制
        connect(colorBarDialog, &ColorBarDialog::legendTextColorChanged,
                [this](double r, double g, double b) {
                    qtvtkWindow->SetScalarBarTextColor(r, g, b);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 8. 字体控制
        connect(colorBarDialog, &ColorBarDialog::legendFontChanged,
                [this](const QString& family, int size, bool bold, bool italic) {
                    qtvtkWindow->SetScalarBarFont(family.toStdString(), size, bold, italic);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        // 9. 颜色映射控制
        connect(colorBarDialog, &ColorBarDialog::colorMapChanged,
                [this](int flowNumber, int colorMapIndex, bool reverse) {
                    qtvtkWindow->SetBoundaryColorScheme(flowNumber, colorMapIndex, reverse);
                    ui->vtkBox->renderWindow()->Render();
                });
        
        colorBarDialog->setWindowModality(Qt::ApplicationModal);
        cout << "ColorBarDialog created successfully" << endl;
    }
    else
    {
        colorBarDialog->setColorBarDialog(flows, qtvtkWindow->GetCurFlowNumber());
    }
    
    colorBarDialog->show();
    colorBarDialog->raise();
    colorBarDialog->activateWindow();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::setColorBar(double m, double M, int number, int flowNumber, double width, double height)
{
    qtvtkWindow->SetScalarBar(m,M,number,flowNumber);
    qtvtkWindow->SetScalarBarSize(width, height);
    ui->vtkBox->renderWindow()->Render();
}

bool MainWindow::IsClickOnScalarBar()
{
    if (!qtvtkWindow || !qtvtkWindow->IsScalarBarWidgetActive())
        return false;
    
    // Get mouse position
    int* clickPos = ui->vtkBox->interactor()->GetEventPosition();
    int clickX = clickPos[0];
    int clickY = clickPos[1];
    
    // Get window size
    int* windowSize = ui->vtkBox->renderWindow()->GetSize();
    int windowWidth = windowSize[0];
    int windowHeight = windowSize[1];
    
    // Get ScalarBar widget
    auto scalarBarWidget = qtvtkWindow->GetScalarBarWidget();
    if (!scalarBarWidget) return false;
    
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        scalarBarWidget->GetRepresentation());
    if (!rep) return false;
    
    // Get position and size (normalized 0-1)
    double* pos = rep->GetPosition();      // Lower-left corner
    double* size = rep->GetPosition2();    // Width and height
    
    // Convert to pixel coordinates
    int barLeft = static_cast<int>(pos[0] * windowWidth);
    int barBottom = static_cast<int>(pos[1] * windowHeight);
    int barRight = static_cast<int>((pos[0] + size[0]) * windowWidth);
    int barTop = static_cast<int>((pos[1] + size[1]) * windowHeight);
    
    // Check if click is within ScalarBar bounds
    return (clickX >= barLeft && clickX <= barRight && 
            clickY >= barBottom && clickY <= barTop);
}

bool MainWindow::IsClickOnCutplaneScalarBar()
{
    if (!qtvtkWindow) return false;
    
    auto scalarBar = qtvtkWindow->GetCutplaneScalarBar();
    if (!scalarBar || !scalarBar->GetVisibility()) return false;
    
    // Get mouse position
    int* clickPos = ui->vtkBox->interactor()->GetEventPosition();
    int clickX = clickPos[0];
    int clickY = clickPos[1];
    
    // Get window size
    int* windowSize = ui->vtkBox->renderWindow()->GetSize();
    int windowWidth = windowSize[0];
    int windowHeight = windowSize[1];
    
    // Get position and size (normalized viewport 0-1)
    double* pos = scalarBar->GetPosition();   // Lower-left corner
    double barWidth = scalarBar->GetWidth();
    double barHeight = scalarBar->GetHeight();
    
    // Convert to pixel coordinates
    int barLeft = static_cast<int>(pos[0] * windowWidth);
    int barBottom = static_cast<int>(pos[1] * windowHeight);
    int barRight = static_cast<int>((pos[0] + barWidth) * windowWidth);
    int barTop = static_cast<int>((pos[1] + barHeight) * windowHeight);
    
    // Check if click is within ScalarBar bounds
    return (clickX >= barLeft && clickX <= barRight && 
            clickY >= barBottom && clickY <= barTop);
}


void MainWindow::selectBoundaryButtonTriggeded()
{
    //* Test Control Panel
    std::cout << "Control Test "<< std::endl;
    if (controlPanel == nullptr)
    {
        controlPanel = new ControlPanel(this);
        controlPanel->setWindowModality(Qt::NonModal);
        
        // 获取boundary数据并传递给ControlPanel
        auto boundaryData = qtvtkWindow->GetBoundaryDatasets();
        controlPanel->setupTable(boundaryData);
        
        // 连接ControlPanel的boundary控制信号到MainWindow的槽函数
        connect(controlPanel, &ControlPanel::setBoundarys, this, &MainWindow::showBoundaryActor);
        // 连接ControlPanel的cutplane控制信号到MainWindow的槽函数
        connect(controlPanel, &ControlPanel::setCutplaneVisiable, this, &MainWindow::showCutplane);
        // 连接ControlPanel的boundary透明度信号到vtkDisplayWindow
        connect(controlPanel, &ControlPanel::boundaryTransparencyChanged, qtvtkWindow, &vtkDisplayWindow::SetBoundaryTransparency);
        // 连接ControlPanel的slice透明度信号到vtkDisplayWindow
        connect(controlPanel, &ControlPanel::sliceTransparencyChanged, qtvtkWindow, &vtkDisplayWindow::SetSliceTransparency);
        // 连接ControlPanel的slice删除信号到MainWindow的删除函数
        connect(controlPanel, &ControlPanel::sliceDeleteRequested, this, &MainWindow::deleteSlice);
        // 连接ControlPanel的slice contour mode信号到vtkDisplayWindow
        connect(controlPanel, &ControlPanel::sliceContourModeChanged, qtvtkWindow, &vtkDisplayWindow::SetSliceContourMode);
        
        // 连接slice contour mode信号到CutplaneDialog的UI控件启用/禁用
        connect(controlPanel, &ControlPanel::sliceContourModeChanged, [this](const QString& mode) {
            if (cutPlaneDialog) {
                bool enableMapping = (mode == "isolated");
                cutPlaneDialog->setMappingControlEnabled(enableMapping);
            }
        });
        
        connect(controlPanel, &ControlPanel::sliceContourModeChanged, [this]() {
            ui->vtkBox->renderWindow()->Render();
        });
        
        // 设置透明度控件的初始状态，与transparancyCheckBox保持一致
        controlPanel->setTransparencyControlsEnabled(ui->transparancyCheckBox->isChecked());
    }
    
    controlPanel->show();
    
}

void MainWindow::showBoundaryActor(int meshNumber, int boundaryNumber, bool flag)
{
    if (flag) qtvtkWindow->VisiableBoundaryActor(meshNumber, boundaryNumber);
    else
    {
        qtvtkWindow->InVisiableBoundaryActor(meshNumber, boundaryNumber);
    }
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::isoSurfaceCheckBoxTriggered()
{
    if (!qtvtkWindow->HasIsoSurface())
    {
        qtvtkWindow->CreateIsoSurfaceActor();
        qtvtkWindow->AddIsoSurfaceActor();
        ui->vtkBox->renderWindow()->Render();
        return;
    }

    if (ui->isoSurfaceCheckBox->isChecked())
    {
        qtvtkWindow->AddIsoSurfaceActor();
    }
    else qtvtkWindow->RemoveIsoSurfaceActor();
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::isoSurfaceSettingButtonTriggered()
{
    if (!qtvtkWindow->HasIsoSurface()) 
    {
        cout << "please add isosurface actor first" << endl;
        return;
    }
    if (!ui->isoSurfaceCheckBox->isChecked())
    {
        cout << "please show isosurface first" << endl;
        return;
    }
    IsoSurfaceDialog* isoSurfaceDialog = new IsoSurfaceDialog(this);
    isoSurfaceDialog->setAttribute(Qt::WA_DeleteOnClose);
    isoSurfaceDialog->setWindowModality(Qt::ApplicationModal);
    isoSurfaceDialog->setIsoSurfaceDialog(qtvtkWindow->GetFlows(), qtvtkWindow->GetCurFlowNumber() , qtvtkWindow->GetIsoSurfaceFloodNumber(), 
    qtvtkWindow->GetDeriveds().contourFilter->GetValue(0));
    connect(isoSurfaceDialog, SIGNAL(changeFloodParameter(int)), this, SLOT(changeFloodNumber(int)));
    connect(isoSurfaceDialog, SIGNAL(changeFlowParameter(int)), this, SLOT(isoSurfaceChangeFlowNumber(int)));
    connect(isoSurfaceDialog, SIGNAL(changeIsoSurfaceValue(double)), this, SLOT(isoSurfaceValueChanged(double)));
    isoSurfaceDialog->show();

}

void MainWindow::changeFloodNumber(int number)
{
    qtvtkWindow->ChangeFloodNumber(number);
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::isoSurfaceChangeFlowNumber(int number)
{
    qtvtkWindow->IsoSurfaceChangeFlowNumber(number);
    changeFloodNumber(number);
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::isoSurfaceValueChanged(double value)
{
    qtvtkWindow->SetIsoSurfaceValue(value);
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::slicesCheckBoxTriggered()
{
    if (!qtvtkWindow->HasCutplane())
    {
        cout << "test" << endl;
        qtvtkWindow->AddNewCutplane();

        if (controlPanel != nullptr) {
            // 获取当前cutplane的数量作为索引
            int cutplaneIndex = qtvtkWindow->GetPlanes().size() - 1; // 新添加的cutplane索引
            double origin[3] = {0.0, 0.0, 0.0};
            double normal[3] = {1.0, 0.0, 0.0};
            controlPanel->addCutplaneToTable(cutplaneIndex, origin, normal);
        }
        // 第一次创建cutplane时也要显示ScalarBar
        if (ui->slicesCheckBox->isChecked()) {
            qtvtkWindow->ShowCutplaneScalarBar();
        }

        cout << "add a simple cutplane" << endl;
        ui->vtkBox->renderWindow()->Render();
        return;
    }
    if (ui->slicesCheckBox->isChecked())
    {
        qtvtkWindow->AddCutplaneActors();
        qtvtkWindow->ShowCutplaneScalarBar();
    }
    else 
    {
        qtvtkWindow->RemoveCutplaneActors();
        qtvtkWindow->HideCutplaneScalarBar();
    }
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::slicesSettingButtonTriggered()
{
    if (ui->slicesCheckBox->checkState() == Qt::Unchecked)
    {
        cout << "please add slices actor first" << endl;
        return;
    }
    CutplaneDialog *cutplaneDialog = new CutplaneDialog(this);
    cutplaneDialog->setCutplaneDialog(qtvtkWindow->GetPlanes());

    double* bounds = qtvtkWindow->GetModelBounds();
    if (bounds != nullptr) {
        cutplaneDialog->setModelBounds(bounds);
    }
    
    // 设置流场变量数据
    cutplaneDialog->setFlowVariables(qtvtkWindow->GetFlows(), qtvtkWindow->GetCurFlowNumber());
    
    cutplaneDialog->setAttribute(Qt::WA_DeleteOnClose);
    cutplaneDialog->setWindowModality(Qt::ApplicationModal);
    connect(cutplaneDialog, SIGNAL(finishSetParameters(double*,double*,int)),this, SLOT(changeCutplane(double*,double*,int)));
    connect(cutplaneDialog, &CutplaneDialog::createNewCutplane, this, &MainWindow::makeNewCutplane);
    connect(cutplaneDialog, &CutplaneDialog::colorMappingChanged, this, &MainWindow::updateCutplaneColorMapping);
    connect(cutplaneDialog, &CutplaneDialog::colorSchemeChanged, [this](int presetIndex, bool reverse){
        qtvtkWindow->SetCutplaneColorScheme(presetIndex, reverse);
        ui->vtkBox->renderWindow()->Render();
    });
    
    // 连接变量选择变化信号
    connect(cutplaneDialog, &CutplaneDialog::variableSelectionChanged, [this](int flowNumber){
        qtvtkWindow->SetCutplaneVariable(flowNumber);
        ui->vtkBox->renderWindow()->Render();
    });
    //* test
    connect(cutplaneDialog, &CutplaneDialog::sliceLocation,
            [this](double value,int axis) {
                if (qtvtkWindow) {
                    qtvtkWindow->CreatePlanePreview(value,axis);
                    ui->vtkBox->renderWindow()->Render();
                }
            });

    connect(cutplaneDialog, &QDialog::finished,
        [this]() {
            if (qtvtkWindow) {
                qtvtkWindow->HidePlanePreview();
            }
        });

    cutplaneDialog->show();

}

void MainWindow::changeCutplane(double* origin, double *normal, int cutplaneNumber)
{
    // cout << origin[0] << " " << origin[1] << " " << origin[2] << endl;
    // cout << normal[0] << " " << normal[1] << " " << normal[2] << endl;
    qtvtkWindow->SetCutplane(cutplaneNumber, origin,normal);
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::makeNewCutplane(double* origin, double* normal)
{
    qtvtkWindow->AddNewCutplane(origin, normal);

    if (controlPanel != nullptr) {
        int cutplaneIndex = qtvtkWindow->GetPlanes().size() - 1; // 新添加的cutplane索引
        controlPanel->addCutplaneToTable(cutplaneIndex, origin, normal);
    }
    
    cout << "add new cut plane with origin(" << origin[0] << ", " << origin[1] << ", " << origin[2] 
         << ") normal(" << normal[0] << ", " << normal[1] << ", " << normal[2] << ")" << endl;
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::transparancyCheckBoxTriggered()
{
    static bool firstTimeEnabled = true; // 跟踪是否是第一次启用透明度
    bool isChecked = ui->transparancyCheckBox->isChecked();
    
    if (isChecked) {
        // 只在第一次勾选时设置全局透明度为0.5
        if (firstTimeEnabled) {
            qtvtkWindow->SetActorTransparancy(0.5);
            firstTimeEnabled = false; // 标记已经执行过第一次设置
            
            // 如果ControlPanel存在，同步更新所有spinbox的值为0.5
            if (controlPanel) {
                std::vector<std::vector<double>> defaultTransparencies;
                controlPanel->getAllBoundaryTransparencies(defaultTransparencies);
                // 将所有值设置为0.5
                for (auto& meshVec : defaultTransparencies) {
                    for (auto& value : meshVec) {
                        value = 0.5;
                    }
                }
                controlPanel->setAllBoundaryTransparencies(defaultTransparencies);
            }
        } else {
            // 非首次勾选：恢复之前保存的透明度值
            if (controlPanel && !savedBoundaryTransparencies.empty()) {
                // 根据ControlPanel中的值来设置每个boundary的透明度
                for (int i = 0; i < savedBoundaryTransparencies.size(); i++) {
                    for (int j = 0; j < savedBoundaryTransparencies[i].size(); j++) {
                        double opacity = savedBoundaryTransparencies[i][j];
                        qtvtkWindow->SetBoundaryTransparency(i, j, opacity);
                    }
                }
            }
        }
        vtkObject::GlobalWarningDisplayOff();
    }
    else {
        // 取消勾选：保存当前透明度值，然后设置为完全不透明
        if (controlPanel) {
            controlPanel->getAllBoundaryTransparencies(savedBoundaryTransparencies);
        }
        qtvtkWindow->SetActorTransparancy(1.0);
        vtkObject::GlobalWarningDisplayOn();
    }
    
    // 控制ControlPanel中透明度控件的启用/禁用状态
    if (controlPanel) {
        controlPanel->setTransparencyControlsEnabled(isChecked);
    }
    
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::lightingCheckBoxTriggered()
{
    qtvtkWindow->SetActorLighting(ui->lightingCheckBox->isChecked());
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::showCutplane(int number, bool flag)
{
    if (flag) qtvtkWindow->AddCutplane(number);
    else
    {
        qtvtkWindow->RemoveCutplane(number);
    }
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::InitializeMainWindow()
{
    SetIcons();
    SetvtkBox();

    if (ui->CBtn_BackGround) {
        ui->CBtn_BackGround->setCurrentIndex(1);
    }
    
    // 初始化MouseInteractorStyle（用于双击ScalarBar等交互功能）
    // 注意：点击探测功能通过isAddTextActor标志控制，而不是通过是否创建style来控制
    if (::style == nullptr)
    {
        ::style = new MouseInteractorStyle();
        ::style->SetMainWindow(this);
        ::style->isAddTextActor = false;  // 初始时探测功能关闭
        ui->vtkBox->interactor()->SetInteractorStyle(::style);
        cout << "MouseInteractorStyle initialized (probe mode OFF)" << endl;
    }

    connect(ui->xoyView, SIGNAL(clicked()), this, SLOT(xoyViewTriggered()));
    connect(ui->xozView, SIGNAL(clicked()), this, SLOT(xozViewTriggered()));
    connect(ui->yozView, SIGNAL(clicked()), this, SLOT(yozViewTriggered()));
    connect(ui->xyzView, SIGNAL(clicked()), this, SLOT(xyzViewTriggered()));

    // 测试：添加lambda来验证按钮点击
    connect(ui->contourSettingButton, &QPushButton::clicked, [this]() {
        cout << "*** contourSettingButton clicked (lambda) ***" << endl;
    });
    
    connect(ui->contourSettingButton, &QPushButton::clicked, this, &MainWindow::contourSettingButtonTriggered);
    connect(ui->selectBoundaryButton, SIGNAL(clicked()), this, SLOT(selectBoundaryButtonTriggeded()));
    connect(ui->IsoSurfaceSettingButton, SIGNAL(clicked()), this, SLOT(isoSurfaceSettingButtonTriggered()));
    connect(ui->addSliceButton, &QPushButton::clicked, this, &MainWindow::slicesSettingButtonTriggered);

    connect(ui->ConstHeightpushButton, SIGNAL(clicked()),this,SLOT(ConstHeightButtonTriggered()));
//    connect(ui->ConstHeightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(ConstHeightCheckBoxTriggered()));
    

    // Span slider connection (0-100 maps to 0.0-1.0)
    ui->Sli_Span->setRange(0, 100);
    ui->Sli_Span->setValue(50);  // Default to 0.5
    connect(ui->Sli_Span, SIGNAL(valueChanged(int)), this, SLOT(onSpanSliderChanged(int)));
    
    // Periodic copy connections
    connect(ui->com_copy, SIGNAL(currentIndexChanged(int)), this, SLOT(onCopyZoneChanged(int)));
    connect(ui->Led_Copy, SIGNAL(returnPressed()), this, SLOT(onPeriodicCopyRequested()));
    connect(ui->Btn_ClearAllZoneCopy, &QPushButton::clicked, this, [this]() {
        if (qtvtkWindow && qtvtkWindow->HasGrid())
        {
            qtvtkWindow->ClearPeriodicCopies();
            ui->vtkBox->renderWindow()->Render();
            qDebug() << "[Periodic] Cleared all zone copies";
        }
    });

    //* Set View Control    // 创建主分割器
    mainSplitter = new QSplitter(Qt::Horizontal, this);

    // 创建左侧容器（用于3D视图）
    mainViewContainer = new QWidget(mainSplitter);
    QVBoxLayout *mainViewLayout = new QVBoxLayout(mainViewContainer);
    mainViewLayout->setContentsMargins(0, 0, 0, 0);
    mainViewLayout->addWidget(ui->vtkBox);
    mainSplitter->addWidget(mainViewContainer);

    // 创建右侧面板（用于Meridional和Blade-to-Blade视图）
    rightPanel = new QWidget(mainSplitter);
    rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    // 创建Meridional视图容器
    meridionalViewContainer = new QWidget(rightPanel);
    QVBoxLayout *meridionalLayout = new QVBoxLayout(meridionalViewContainer);
    meridionalLayout->setContentsMargins(0, 0, 0, 0);
    meridionalLayout->addWidget(vtkWidget);
    
    // 为Meridional视图设置2D交互样式（禁止旋转）
    MeridionalrenderWindow->AddRenderer(Meridionalrenderer);
    vtkWidget->setRenderWindow(MeridionalrenderWindow);
    vtkSmartPointer<vtkInteractorStyleImage> meridionalStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    if (MeridionalrenderWindow->GetInteractor())
    {
        MeridionalrenderWindow->GetInteractor()->SetInteractorStyle(meridionalStyle);
    }
    
    rightLayout->addWidget(meridionalViewContainer);

    // 创建Blade-to-Blade视图容器
    bladeToBladeViewContainer = new QWidget(rightPanel);
    QVBoxLayout *bladeLayout = new QVBoxLayout(bladeToBladeViewContainer);
    bladeLayout->setContentsMargins(0, 0, 0, 0);
    bladeLayout->addWidget(bladeToBladevtkWidget);
    rightLayout->addWidget(bladeToBladeViewContainer);

    mainSplitter->addWidget(rightPanel);

    // 设置分割器初始比例
    mainSplitter->setSizes(QList<int>() << 800 << 400);

    // 创建探测面板（放在最右侧）
    probePanel = new ProbePanel(this);
    probePanel->setMinimumWidth(300);
    probePanel->setMaximumWidth(500);
    probePanel->hide();  // 初始隐藏
    
    // 连接面板关闭信号
    connect(probePanel, &ProbePanel::panelClosed, this, &MainWindow::onProbePanelClosed);
    
    // 创建外层水平分割器，包含mainSplitter和probePanel
    QSplitter *outerSplitter = new QSplitter(Qt::Horizontal, this);
    outerSplitter->addWidget(mainSplitter);
    outerSplitter->addWidget(probePanel);
    outerSplitter->setStretchFactor(0, 1);  // mainSplitter可伸缩
    outerSplitter->setStretchFactor(1, 0);  // probePanel固定大小
    
    // 将外层分割器添加到主布局中
    ui->VTKLayout->addWidget(outerSplitter);

    // 初始化视图状态
    ui->Check_3Dview->setChecked(true);
    on_Check_3Dview_toggled(true);

    QButtonGroup *group_ViewControl = new QButtonGroup(this);
    group_ViewControl->setExclusive(true);
    group_ViewControl->addButton(ui->Check_3Dview);
    group_ViewControl->addButton(ui->Check_ThreeView);
    group_ViewControl->addButton(ui->Check_Meri);
    group_ViewControl->addButton(ui->Check_BladeToBlade);

}

void MainWindow::SetIcons()
{
    const string iconPath = ":/PostIcons/";
    
    //Orientation view buttons's icon
    QSize iconSize(32, 32); // Fixed icon size
    ui->xoyView->setIcon(QIcon((iconPath+"xoy.png").c_str()));
    ui->xozView->setIcon(QIcon((iconPath+"xoz.png").c_str()));
    ui->yozView->setIcon(QIcon((iconPath+"yoz.png").c_str()));
    ui->xyzView->setIcon(QIcon((iconPath+"xyz.png").c_str()));

    ui->xoyView->setIconSize(iconSize);
    ui->xozView->setIconSize(iconSize);
    ui->yozView->setIconSize(iconSize);
    ui->xyzView->setIconSize(iconSize);

    ui->contourSettingButton->setIcon(QIcon((iconPath+"contour.png").c_str()));
    ui->IsoSurfaceSettingButton->setIcon(QIcon((iconPath+"isoSurface.png").c_str()));
    ui->addSliceButton->setIcon(QIcon((iconPath+"cutplane.png").c_str()));
    ui->addStreamLineButton->setIcon(QIcon((iconPath+"streamline.png").c_str()));

    QSize buttonIconSize(48, 48);
    ui->contourSettingButton->setIconSize(buttonIconSize);
    ui->IsoSurfaceSettingButton->setIconSize(buttonIconSize);
    ui->addSliceButton->setIconSize(buttonIconSize);
    ui->addStreamLineButton->setIconSize(buttonIconSize);

    //toolBar's icons
    ui->toolBar->setIconSize(QSize(40,40));
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->actionOpenFile->setIcon(QIcon((iconPath+"open.png").c_str()));
    ui->actionAddAxisActor->setIcon(QIcon((iconPath+"orient_axes.png").c_str()));
    ui->actionAddScalarBarActor->setIcon(QIcon((iconPath+"colorbar.png").c_str()));
    ui->actionAddOutlineActor->setIcon(QIcon((iconPath+"boundbox.png").c_str()));
    ui->actionAddPointInformation->setIcon(QIcon((iconPath+"text.png").c_str()));
    ui->actionCalculatorFunction->setIcon(QIcon((iconPath+"calculator.png").c_str()));
    ui->actionNewWindow->setIcon(QIcon((iconPath+"newwindow.png").c_str()));
    QIcon checkedIcon((iconPath+"open.png").c_str());
}

void MainWindow::SetvtkBox()
{
    qtvtkWindow = new vtkDisplayWindow();
    ui->vtkBox->setRenderWindow(qtvtkWindow->GetRenderWindow());
    ui->vtkBox->renderWindow()->Render();
}

void MainWindow::ResetScrollArea()
{
    ui->edgeCheckBox->setCheckState(Qt::Unchecked);
    ui->meshCheckBox->setCheckState(Qt::Unchecked);
    ui->shadeCheckBox->setCheckState(Qt::Checked);
    ui->contourCheckBox->setCheckState(Qt::Checked);
    ui->slicesCheckBox->setCheckState(Qt::Unchecked);
    ui->transparancyCheckBox->setCheckState(Qt::Unchecked);
    ui->lightingCheckBox->setCheckState(Qt::Checked);

    connect(ui->meshCheckBox, SIGNAL(stateChanged(int)), this, SLOT(meshCheckBoxTriggered()));
    connect(ui->contourCheckBox, SIGNAL(stateChanged(int)), this, SLOT(contourCheckBoxTriggered()));
    connect(ui->shadeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(shadeCheckBoxTriggered()));
    connect(ui->edgeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(edgeCheckBoxTriggered()));

    ui->isoSurfaceCheckBox->setCheckState(Qt::Unchecked);

    connect(ui->isoSurfaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(isoSurfaceCheckBoxTriggered()));
    connect(ui->slicesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slicesCheckBoxTriggered()));

    connect(ui->transparancyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(transparancyCheckBoxTriggered()));
    connect(ui->lightingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(lightingCheckBoxTriggered()));

    ui->scrollArea->setEnabled(true);
}

void MainWindow::DisableScrollArea()
{
    disconnect(ui->meshCheckBox, SIGNAL(stateChanged(int)), this, SLOT(meshCheckBoxTriggered()));
    disconnect(ui->contourCheckBox, SIGNAL(stateChanged(int)), this, SLOT(contourCheckBoxTriggered()));
    disconnect(ui->shadeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(shadeCheckBoxTriggered()));
    disconnect(ui->edgeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(edgeCheckBoxTriggered()));
    
    disconnect(ui->isoSurfaceCheckBox, SIGNAL(stateChanged(int)), this, SLOT(isoSurfaceCheckBoxTriggered()));
    disconnect(ui->slicesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slicesCheckBoxTriggered()));

    disconnect(ui->transparancyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(transparancyCheckBoxTriggered()));
    disconnect(ui->lightingCheckBox, SIGNAL(stateChanged(int)), this, SLOT(lightingCheckBoxTriggered()));

    ui->scrollArea->setEnabled(false);
}


void MainWindow::on_ConstHeightCheckBox_toggled(bool trigger)
{
    if (trigger) {
        ConstHeightPlaneDialog *constheightdialog = new ConstHeightPlaneDialog(this);
        constheightdialog->show();
        connect(constheightdialog, SIGNAL(finishSetParameters(double)), this, SLOT(AddConstHeightPlane(double)));
    }
    else
    {
        qtvtkWindow->RemoveConstHeight();
        ui->vtkBox->renderWindow()->Render();
    }
}

void MainWindow::ConstHeightButtonTriggered()
{
    ConstSettingDialog * settingDialog = new ConstSettingDialog(this);
    settingDialog->setConstSettingDialog(qtvtkWindow->GetFlows(), qtvtkWindow->GetCurFlowNumber());
    settingDialog->show();
    connect(settingDialog,SIGNAL(finishSetHeight(double)),this, SLOT(AddConstHeightPlane(double)));
    connect(settingDialog,SIGNAL(finishSetFlow(int)),this, SLOT(ChangeConstHeightFlow(int)));
}
void MainWindow::AddConstHeightPlane(double height)
{
    qtvtkWindow->CreateConstHeight(height);
}

void MainWindow::ChangeConstHeightFlow(int flow)
{
    qtvtkWindow->ChangeConstHeightFlow(flow);
}


void MainWindow::AddBladeToBladePlane(double span)
{
    qDebug() << "[B2B] Creating blade-to-blade plane at span =" << span;
    
    // Remove old actors from renderer to avoid duplication
    BladeToBladerenderer->RemoveAllViewProps();
    
    // Create blade-to-blade plane and add to BladeToBladerenderer
    auto actors = qtvtkWindow->CreateBladeToBladePlane(span);
    
    qDebug() << "[B2B] Number of actors created:" << actors.size();
    
    if (actors.empty()) {
        qDebug() << "[B2B] ERROR: No actors created! Check console for details.";
        return;
    }
    
    for (int i = 0; i < actors.size(); i++)
    {
        BladeToBladerenderer->AddActor(actors[i]);
        qDebug() << "[B2B] Added actor" << i << "to BladeToBladerenderer";
    }
    
    // Setup 2D camera for flat view
    vtkCamera* camera = BladeToBladerenderer->GetActiveCamera();
    camera->SetPosition(0, 0, 10);
    camera->SetFocalPoint(0, 0, 0);
    camera->SetViewUp(0, 1, 0);
    camera->ParallelProjectionOn();
    
    // Reset camera to fit the 2D unwrapped surface
    BladeToBladerenderer->ResetCamera();
    
    qDebug() << "[B2B] Rendering BladeToBladerenderWindow...";
    BladeToBladerenderWindow->Render();
    qDebug() << "[B2B] Done!";
}

void MainWindow::ChangeBladeToBladePlaneFlow(int flow)
{
    // Remove old actors
    BladeToBladerenderer->RemoveAllViewProps();
    
    // Create new actors with updated flow variable
    auto actors = qtvtkWindow->ChangeBladeToBladePlaneFlow(flow);
    for (int i = 0; i < actors.size(); i++)
    {
        BladeToBladerenderer->AddActor(actors[i]);
    }
    BladeToBladerenderWindow->Render();
}

void MainWindow::onSpanSliderChanged(int value)
{
    double span = value / 100.0;
    qDebug() << "[B2B] Span slider changed to:" << span;
    
    AddBladeToBladePlane(span);
}

void MainWindow::onCopyZoneChanged(int index)
{
    if (!qtvtkWindow || !qtvtkWindow->HasGrid())
    {
        return;
    }
    updatePeriodicCopyBoundaryList(index);
}

void MainWindow::updatePeriodicCopyBoundaryList(int zoneIndex)
{
    if (!ui->periodicCopyGridLayout)
    {
        return;
    }

    for (auto *cb : periodicCopyBoundaryChecks)
    {
        ui->periodicCopyGridLayout->removeWidget(cb);
        cb->deleteLater();
    }
    periodicCopyBoundaryChecks.clear();

    if (!qtvtkWindow || !qtvtkWindow->HasGrid())
    {
        return;
    }

    auto boundaryDatasets = qtvtkWindow->GetBoundaryDatasets();
    if (zoneIndex < 0 || zoneIndex >= static_cast<int>(boundaryDatasets.size()))
    {
        return;
    }

    const auto &bnds = boundaryDatasets[zoneIndex];
    if (bnds.empty())
    {
        return;
    }

    int colCount = 1;
    for (int i = 0; i < static_cast<int>(bnds.size()); ++i)
    {
        QCheckBox *cb = new QCheckBox(ui->periodicCopyContainer);
        cb->setText(QString::fromLocal8Bit(bnds[i].name.c_str()));

        int row = i;
        int col = 0;

        ui->periodicCopyGridLayout->addWidget(cb, row, col);
        periodicCopyBoundaryChecks.push_back(cb);
    }
}

void MainWindow::onPeriodicCopyRequested()
{
    // 获取选中的 zone 索引
    int zoneIndex = ui->com_copy->currentIndex();
    
    // 获取复制数量
    QString numText = ui->Led_Copy->text();
    bool ok;
    int numCopies = numText.toInt(&ok);
    
    if (!ok || numCopies < 0)
    {
        qDebug() << "[Periodic] Invalid number of copies:" << numText;
        return;
    }
    
    std::vector<int> selectedBoundaries;
    for (int i = 0; i < static_cast<int>(periodicCopyBoundaryChecks.size()); ++i)
    {
        if (periodicCopyBoundaryChecks[i] && periodicCopyBoundaryChecks[i]->isChecked())
        {
            selectedBoundaries.push_back(i);
        }
    }

    if (selectedBoundaries.empty())
    {
        qDebug() << "[Periodic] No boundary selected for copy in zone" << zoneIndex;
        return;
    }

    qDebug() << "[Periodic] Creating" << numCopies << "copies of zone" << zoneIndex 
             << "(" << ui->com_copy->currentText() << ") with" << selectedBoundaries.size() << "boundaries";
    
    qtvtkWindow->CreatePeriodicCopies(zoneIndex, numCopies, selectedBoundaries);
    
    ui->vtkBox->renderWindow()->Render();
}


void MainWindow::updateCutplaneColorMapping(double minValue, double maxValue, int numberOfColors,bool isBaned)
{
    qtvtkWindow->SetCutplaneColorMapping(minValue, maxValue, numberOfColors,isBaned);
    ui->vtkBox->renderWindow()->Render();
}

//* View Control Functions
void MainWindow::on_Check_3Dview_toggled(bool checked)
{
    if (checked)
    {
        mainViewContainer->show();
        rightPanel->hide();

        // 重新计算分割器大小
        mainSplitter->setSizes(QList<int>() << this->width() << 0);

        UpdateViewLabels();
        qDebug() << "Switched to 3D View mode";
    }
}

void MainWindow::on_Check_ThreeView_toggled(bool checked)
{
    if (checked)
    {
        // Setup views before showing them
        SetupMeridionalView();
        SetupBladeToBladeView();

        mainViewContainer->show();
        rightPanel->show();

        // Ensure sub-views are visible
        meridionalViewContainer->show();
        bladeToBladeViewContainer->show();

        // Adjust splitter sizes
        mainSplitter->setSizes(QList<int>() << (this->width() * 0.6) << (this->width() * 0.4));

        // Render all windows
        ui->vtkBox->renderWindow()->Render();
        MeridionalrenderWindow->Render();
        BladeToBladerenderWindow->Render();

        UpdateViewLabels();
        qDebug() << "Switched to Three View mode";
    }
}

void MainWindow::on_Check_Meri_toggled(bool checked)
{
    if (checked)
    {
        SetupMeridionalView();

        mainViewContainer->hide();
        rightPanel->show();

        // Only show the Meridional view
        meridionalViewContainer->show();
        bladeToBladeViewContainer->hide();

        // Adjust splitter sizes
        mainSplitter->setSizes(QList<int>() << 0 << this->width());

        MeridionalrenderWindow->Render();
        UpdateViewLabels();
        qDebug() << "Switched to Meridional View mode";
    }
}

void MainWindow::on_Check_BladeToBlade_toggled(bool checked)
{
    if (checked)
    {
        SetupBladeToBladeView();

        mainViewContainer->hide();
        rightPanel->show();

        // Only show the Blade-to-Blade view
        meridionalViewContainer->hide();
        bladeToBladeViewContainer->show();

        // Adjust splitter sizes
        mainSplitter->setSizes(QList<int>() << 0 << this->width());

        BladeToBladerenderWindow->Render();
        UpdateViewLabels();
        qDebug() << "Switched to Blade-to-Blade View mode";
    }
}

void MainWindow::deleteSlice(int cutplaneIndex)
{
    // 调用vtkDisplayWindow删除cutplane
    qtvtkWindow->DeleteCutplane(cutplaneIndex);
    
    // 重新渲染
    ui->vtkBox->renderWindow()->Render();
}

//* Background Control Functions
void MainWindow::on_CBtn_BackGround_currentTextChanged(const QString &text)
{
    if (!qtvtkWindow->HasGrid())
    {
        return;
    }
    
    // 设置主VTK窗口的背景
    qtvtkWindow->SetBackgroundStyle(text);
    
    // 设置Meridional视图背景
    SetViewBackground(Meridionalrenderer, text);
    MeridionalrenderWindow->Render();
    
    // 设置Blade-to-Blade视图背景
    SetViewBackground(BladeToBladerenderer, text);
    BladeToBladerenderWindow->Render();
}

// 视图管理辅助方法实现
void MainWindow::SetupMeridionalView()
{
    if (qtvtkWindow->MeridionalPlaneActor.empty())
    {
        // 设置背景
        SetViewBackground(Meridionalrenderer, ui->CBtn_BackGround->currentText());
        MeridionalrenderWindow->AddRenderer(Meridionalrenderer);
        vtkWidget->setRenderWindow(MeridionalrenderWindow);
        
        // 设置2D交互样式，禁止旋转，只允许平移和缩放
        vtkSmartPointer<vtkInteractorStyleImage> imageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
        if (MeridionalrenderWindow->GetInteractor())
        {
            MeridionalrenderWindow->GetInteractor()->SetInteractorStyle(imageStyle);
        }
        
        // 创建Meridional平面
        auto plane = qtvtkWindow->CreateMeridionalPlane(0, 10);
        for (int i = 0; i < plane.size(); i++)
        {
            Meridionalrenderer->AddActor(plane[i]);
        }
        MeridionalrenderWindow->Render();
    }
}

void MainWindow::SetupBladeToBladeView()
{
    // 设置背景
    SetViewBackground(BladeToBladerenderer, ui->CBtn_BackGround->currentText());
    BladeToBladerenderWindow->AddRenderer(BladeToBladerenderer);
    bladeToBladevtkWidget->setRenderWindow(BladeToBladerenderWindow);
    
    qDebug() << "[B2B Setup] Setting up Blade-to-Blade view";
    
    // Setup 2D camera (top view, looking down Z-axis)
    vtkCamera* camera = BladeToBladerenderer->GetActiveCamera();
    camera->SetPosition(0, 0, 10);       // Position camera above
    camera->SetFocalPoint(0, 0, 0);      // Look at Z=0 plane
    camera->SetViewUp(0, 1, 0);          // Y-axis points up
    camera->ParallelProjectionOn();      // Parallel projection for 2D
    
    // Restore any existing B2B planes if they exist
    if (!qtvtkWindow->BladeToBladePlaneActor.empty()) {
        qDebug() << "[B2B Setup] Restoring" << qtvtkWindow->BladeToBladePlaneActor.size() << "existing B2B planes";
        for (int i = 0; i < qtvtkWindow->BladeToBladePlaneActor.size(); i++) {
            BladeToBladerenderer->AddActor(qtvtkWindow->BladeToBladePlaneActor[i]);
        }
        BladeToBladerenderer->ResetCamera();
    }
    
    BladeToBladerenderWindow->Render();
}

void MainWindow::SetViewBackground(vtkSmartPointer<vtkRenderer> renderer, const QString &style)
{
    if (style == "Sky Blue") {
        renderer->SetBackground(1.0, 1.0, 1.0);
        renderer->SetBackground2(0.529, 0.8078, 0.92157);
        renderer->SetGradientBackground(true);
    } else if (style == "White") {
        renderer->SetBackground(1.0, 1.0, 1.0);
        renderer->SetGradientBackground(false);
    }
}

void MainWindow::HideAllViews()
{
    ui->vtkBox->hide();
    vtkWidget->hide();
    bladeToBladevtkWidget->hide();
}

void MainWindow::ShowMainView()
{
    ui->vtkBox->show();
}

void MainWindow::ShowMeridionalView()
{
    SetupMeridionalView();
    vtkWidget->show();
}

void MainWindow::ShowBladeToBladeView()
{
    SetupBladeToBladeView();
    bladeToBladevtkWidget->show();
}

void MainWindow::CreateViewLabels()
{
    // 创建主视图标签
    if (!mainViewLabel) {
        mainViewLabel = new QLabel("3D View", this);
        mainViewLabel->setAlignment(Qt::AlignCenter);
        mainViewLabel->setStyleSheet("QLabel { background-color: rgba(70, 130, 180, 0.8); color: white; font-weight: bold; padding: 4px; border-radius: 4px; }");
        mainViewLabel->hide();
    }
    
    // 创建Meridional视图标签
    if (!meridionalViewLabel) {
        meridionalViewLabel = new QLabel("Meridional View", this);
        meridionalViewLabel->setAlignment(Qt::AlignCenter);
        meridionalViewLabel->setStyleSheet("QLabel { background-color: rgba(70, 130, 180, 0.8); color: white; font-weight: bold; padding: 4px; border-radius: 4px; }");
        meridionalViewLabel->hide();
    }
    
    // 创建Blade-to-Blade视图标签
    if (!bladeToBladeViewLabel) {
        bladeToBladeViewLabel = new QLabel("Blade-to-Blade View", this);
        bladeToBladeViewLabel->setAlignment(Qt::AlignCenter);
        bladeToBladeViewLabel->setStyleSheet("QLabel { background-color: rgba(70, 130, 180, 0.8); color: white; font-weight: bold; padding: 4px; border-radius: 4px; }");
        bladeToBladeViewLabel->hide();
    }
}

void MainWindow::UpdateViewLabels()
{
    CreateViewLabels();

    // --- Main View Label ---
    mainViewLabel->setParent(mainViewContainer);
    mainViewLabel->setGeometry(10, 10, 100, 25);
    mainViewLabel->setVisible(mainViewContainer->isVisible());
    if(mainViewContainer->isVisible()) mainViewLabel->raise();

    // --- Meridional View Label ---
    meridionalViewLabel->setParent(meridionalViewContainer);
    meridionalViewLabel->setGeometry(10, 10, 120, 25);
    meridionalViewLabel->setVisible(meridionalViewContainer->isVisible());
    if(meridionalViewContainer->isVisible()) meridionalViewLabel->raise();

    // --- Blade-to-Blade View Label ---
    bladeToBladeViewLabel->setParent(bladeToBladeViewContainer);
    bladeToBladeViewLabel->setGeometry(10, 10, 140, 25);
    bladeToBladeViewLabel->setVisible(bladeToBladeViewContainer->isVisible());
    if(bladeToBladeViewContainer->isVisible()) bladeToBladeViewLabel->raise();
}

//* Probe Panel Functions
void MainWindow::UpdateProbePanel(const std::string& coordinates, 
                                   const std::vector<std::pair<std::string, double>>& data)
{
    if (probePanel) {
        probePanel->UpdateProbeData(coordinates, data);
    }
}

void MainWindow::onProbePanelClosed()
{
    // 当用户点击探测面板的关闭按钮时，同步关闭探测功能
    if (::style && ::style->isAddTextActor) {
        ::style->isAddTextActor = false;  // 关闭探测模式
        ::style->pointActor->VisibilityOff();
        ui->vtkBox->renderWindow()->Render();
        cout << "Probe panel closed, probe mode disabled" << endl;
    }
}

void MainWindow::ResetViewsAndRenderers()
{
    cout << "Resetting all views and renderers to clear previous model..." << endl;
    
    // 清除Meridional View中的所有actors
    if (Meridionalrenderer) {
        Meridionalrenderer->RemoveAllViewProps();
        if (MeridionalrenderWindow) {
            MeridionalrenderWindow->Render();
        }
    }
    
    // 清除Blade-to-Blade View中的所有actors
    if (BladeToBladerenderer) {
        BladeToBladerenderer->RemoveAllViewProps();
        if (BladeToBladerenderWindow) {
            BladeToBladerenderWindow->Render();
        }
    }
    
    cout << "Views and renderers reset completed." << endl;
}

//* Turbo Initialize Button
void MainWindow::on_Btn_TurboInitial_clicked()
{
    qDebug() << "[Turbo Init] Checking node_radius availability...";
    

    if (!qtvtkWindow->HasNodeRadius())
    {
        QMessageBox::warning(this, "Failed",
                           "Please Set Model as Rotation in PreProcessing !\n"
                           "Current mesh does not contain node_radius data!\n"
                           "Turbomachinery features are not available.");
        qDebug() << "[Turbo Init] node_radius field not found!";
        ui->Wi_TurboSet->setEnabled(false);
        return;
    }
    
    // node_radius存在，启用Wi_TurboSet
    ui->Wi_TurboSet->setEnabled(true);

    qDebug() << "[Turbo Init] Successfully enabled Wi_TurboSet!";
}
