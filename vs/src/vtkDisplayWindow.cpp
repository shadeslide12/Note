#include "vtkDisplayWindow.h"

// Manual VTK module imports to avoid static initialization issues
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)
#include "ColorMapPreset.h"

using std::string; using std::vector;
#include "time.h"
#include <cmath>
using std::string; using std::vector; using std::set;

#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkCamera.h>
#include <vtkMaskPoints.h>
#include <vtkGlyph3D.h>
#include <vtkGlyph2D.h>
#include <vtkGlyphSource2D.h>
#include <vtkArrowSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkLookupTable.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkNamedColors.h>
#include <vtkArrayCalculator.h>
#include <vtkSphereSource.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkAesReader.h"
#include <vtkAppendFilter.h>
#include <vtkScalarBarRepresentation.h>

#include <vtkOpenGLState.h>
#include <vtkObject.h>

//=======
#include <vtkPointInterpolator.h>
#include <vtkGaussianKernel.h>
#include <vtkLinearKernel.h>
#include <vtkVoronoiKernel.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkRotationalExtrusionFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkIntersectionPolyDataFilter.h>
#include <vtkImplicitSelectionLoop.h>
#include <vtkClipPolyData.h>
#include <vtkLine.h>
#include <vtkProbeFilter.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkClipDataSet.h>
#include <vtkUnstructuredGridGeometryFilter.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkPolyLine.h>
#include <vtkCleanPolyData.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkAppendPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <limits>
#include <algorithm>

vtkDisplayWindow::vtkDisplayWindow(QObject *parent):QObject(parent)
{
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    SetBackground();
}

vtkDisplayWindow::~vtkDisplayWindow(){}

void vtkDisplayWindow::ReadAesFile(string aesFileName)
{
    aesReader.ReadGrid(aesFileName);
    aesReader.UpdateFlow(aesFileName.erase(aesFileName.length()-8)+"flow.hdf");
    auto boundaryDatasets = aesReader.GetBoundarys();
    CreateBasicObjects();
    CreateAuxiliaryObject();
    renderer->ResetCamera();
    hasGrid = true;
}

void vtkDisplayWindow::UpdateFlow(string flowFileName)
{
    aesReader.UpdateFlow(flowFileName);
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            y.contourActor->GetMapper()->SetScalarRange(aesReader.GetFlows()[0].range);
            y.contourActor->GetMapper()->SetLookupTable(aesReader.GetFlows()[0].scalarBar->GetLookupTable());
        }
    }
    auxiliarys.scalarBarWidget->SetScalarBarActor(aesReader.GetFlows()[0].scalarBar);
    if (hasVector)
    {
        hasVector = false;
        RemoveVelocityActor();
    }
    curFlow = 0;
}

void vtkDisplayWindow::CreateBasicObjects()
{
    auto boundaryDatasets = aesReader.GetBoundarys();
    for (auto &x : boundaryDatasets)
    {
        vector<BasicObject> zoneBoundarys;
        for (auto &y : x)
        {
            vtkSmartPointer<vtkDataSetMapper> meshMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            vtkSmartPointer<vtkDataSetMapper> shadeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            vtkSmartPointer<vtkDataSetMapper> contourMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            vtkSmartPointer<vtkDataSetMapper> edgeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            meshMapper->SetInputData(y.dataset);
            meshMapper->ScalarVisibilityOff();
            shadeMapper->SetInputData(y.dataset);
            shadeMapper->ScalarVisibilityOff();
            contourMapper->SetInputData(y.dataset);
            contourMapper->ScalarVisibilityOn();
            contourMapper->SetInterpolateScalarsBeforeMapping(true);
            auto flow = aesReader.GetFlows().front();
            contourMapper->SetScalarRange(flow.range);
            contourMapper->GetInput()->GetPointData()->SetActiveScalars(flow.name.c_str());
            contourMapper->SetLookupTable(flow.scalarBar->GetLookupTable());
            vtkSmartPointer<vtkDataSetSurfaceFilter> dsSurfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
            dsSurfaceFilter->SetInputData(y.dataset);
            vtkSmartPointer<vtkFeatureEdges> edgeFilter = vtkSmartPointer<vtkFeatureEdges>::New();
            edgeFilter->SetInputConnection(dsSurfaceFilter->GetOutputPort());
            edgeFilter->BoundaryEdgesOn();
            edgeFilter->NonManifoldEdgesOff();
            edgeFilter->ManifoldEdgesOff();
            edgeFilter->SetFeatureAngle(100);
            edgeMapper->SetInputConnection(edgeFilter->GetOutputPort());
            edgeMapper->ScalarVisibilityOff();
            BasicObject boundary;
            boundary.meshActor = vtkSmartPointer<vtkActor>::New();
            boundary.shadeActor = vtkSmartPointer<vtkActor>::New();
            boundary.contourActor = vtkSmartPointer<vtkActor>::New();
            boundary.edgeActor = vtkSmartPointer<vtkActor>::New();
            boundary.velocityActor = vtkSmartPointer<vtkActor>::New();
            boundary.meshActor->SetMapper(meshMapper);
            boundary.meshActor->GetProperty()->SetRepresentationToWireframe();
            boundary.meshActor->GetProperty()->SetColor(0,0,0);
            boundary.meshActor->SetVisibility(true);
            boundary.shadeActor->SetMapper(shadeMapper);
            boundary.shadeActor->SetVisibility(true);
            boundary.shadeActor->GetProperty()->SetColor(0.83,0.83,0.83);
            boundary.contourActor->SetMapper(contourMapper);
            boundary.contourActor->SetVisibility(true);
            boundary.edgeActor->SetMapper(edgeMapper);
            boundary.edgeActor->SetVisibility(true);
            boundary.edgeActor->GetProperty()->SetColor(0,0,0); 
            boundary.edgeActor->GetProperty()->SetLineWidth(5.0);
            zoneBoundarys.emplace_back(boundary);
            renderer->AddActor(boundary.contourActor);
        }
        boundarys.emplace_back(zoneBoundarys);
    }
}

void vtkDisplayWindow::CreateAuxiliaryObject()
{
    CreateAxisWidget();
    CreateScalarBarWidget();
    CreateOutlineActor();
}

void vtkDisplayWindow::CreateAxisWidget()
{
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->SetPosition(0,0,0);
    axes->SetTotalLength(3,3,3);
    axes->SetShaftType(0);
    axes->SetCylinderRadius(0.05);

    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOff();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->BoldOff();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 0);
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(16);

    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOff();
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->BoldOff();
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 0);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(16);

    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOff();
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->BoldOff();
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 0);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(16);

    auxiliarys.axisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    vtkSmartPointer<vtkNamedColors> color = vtkSmartPointer<vtkNamedColors>::New();
    double rgb[3];
    color->GetColorRGB("Carrot", rgb);
    auxiliarys.axisWidget->SetOutlineColor(rgb[0],rgb[1],rgb[2]);
    auxiliarys.axisWidget->SetOrientationMarker(axes);
    auxiliarys.axisWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
    auxiliarys.axisWidget->SetInteractor(renderWindow->GetInteractor());
    auxiliarys.axisWidget->SetEnabled(1);
    auxiliarys.axisWidget->InteractiveOff();
}

void vtkDisplayWindow::CreateScalarBarWidget()
{
    auxiliarys.scalarBarWidget = vtkSmartPointer<vtkScalarBarWidget>::New();
    auxiliarys.scalarBarWidget->SetScalarBarActor(aesReader.GetFlows()[0].scalarBar);
    auxiliarys.scalarBarWidget->SetInteractor(renderWindow->GetInteractor());
    auxiliarys.scalarBarWidget->SetEnabled(1);
}

void vtkDisplayWindow::CreateOutlineActor()
{
    auto zoneGrids = aesReader.GetZoneGrids();
    for (auto &x : zoneGrids)
    {
        vtkSmartPointer<vtkOutlineFilter> filter = vtkSmartPointer<vtkOutlineFilter>::New();
        filter->SetInputData(x.dataset);
        filter->Update();
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(filter->GetOutput());
        vtkSmartPointer<vtkActor> outline = vtkSmartPointer<vtkActor>::New();
        outline->SetMapper(mapper);
        outline->GetProperty()->SetColor(0,0,0);
        auxiliarys.outlineActors.emplace_back(outline);
        outline->SetVisibility(true);
        renderer->AddActor(outline);
    }
}

void vtkDisplayWindow::VisiableBoundaryActor(int zNumber, int bNumber)
{
    boundarys[zNumber][bNumber].meshActor->SetVisibility(true);
    boundarys[zNumber][bNumber].shadeActor->SetVisibility(true);
    boundarys[zNumber][bNumber].contourActor->SetVisibility(true);
    boundarys[zNumber][bNumber].edgeActor->SetVisibility(true);
    boundarys[zNumber][bNumber].velocityActor->SetVisibility(true);
}

void vtkDisplayWindow::InVisiableBoundaryActor(int zNumber, int bNumber)
{
    boundarys[zNumber][bNumber].meshActor->SetVisibility(false);
    boundarys[zNumber][bNumber].shadeActor->SetVisibility(false);
    boundarys[zNumber][bNumber].contourActor->SetVisibility(false);
    boundarys[zNumber][bNumber].edgeActor->SetVisibility(false);
    boundarys[zNumber][bNumber].velocityActor->SetVisibility(false);
}

void vtkDisplayWindow::AddMeshActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->AddActor(y.meshActor);
        }
    }
}

void vtkDisplayWindow::RemoveMeshActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->RemoveActor(y.meshActor);
        }
    }
}

void vtkDisplayWindow::AddContourActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->AddActor(y.contourActor);
        }
    }
    ActivateScalarBarWidget();

    //* Add Contour copies
    for (auto& [zoneIdx, actors] : periodicCopyContourActorsByZone)
    {
        for (auto& actor : actors)
        {
            renderer->AddActor(actor);
        }
    }
}

void vtkDisplayWindow::RemoveContourActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->RemoveActor(y.contourActor);
        }
    }
    InActivateScalarBarWidget();

    //* Remove Contour copies
    for (auto& [zoneIdx, actors] : periodicCopyContourActorsByZone)
    {
        for (auto& actor : actors)
        {
            renderer->RemoveActor(actor);
        }
    }
}

void vtkDisplayWindow::AddShadeActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->AddActor(y.shadeActor);
        }
    }

    //* Add periodic copies
    for (auto& [zoneIdx, actors] : periodicCopyShadeActorsByZone)
    {
        for (auto& actor : actors)
        {
            renderer->AddActor(actor);
        }
    }
}

void vtkDisplayWindow::RemoveMeridianActor()
{
    for(int i = 0;i < MeridionalPlaneActor.size();i++)
    {
        renderer->RemoveActor(MeridionalPlaneActor[i]);
    }
}

void vtkDisplayWindow::RemoveConstHeight()
{
    for(int i = 0;i < ConstHeightPlaneActor.size();i++)
    {
        renderer->RemoveActor(ConstHeightPlaneActor[i]);
    }
}

void vtkDisplayWindow::RemoveShadeActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->RemoveActor(y.shadeActor);
        }
    }

    //* Remove periodic copies
    for (auto& [zoneIdx, actors] : periodicCopyShadeActorsByZone)
    {
        for (auto& actor : actors)
        {
            renderer->RemoveActor(actor);
        }
    }
}

void vtkDisplayWindow::AddEdgeActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->AddActor(y.edgeActor);
        }
    }
}

void vtkDisplayWindow::RemoveEdgeActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->RemoveActor(y.edgeActor);
        }
    }
}

void vtkDisplayWindow::CreateVelocityActor(int vx, int vy, int vz)
{
    aesReader.CreateVector(vx,vy,vz);
    cout << "begin to create velocity actor." << endl;
    for (int i = 0; i < boundarys.size(); i++)
    {
        for (int j = 0; j < boundarys[i].size(); j++)
        {
            vtkNew<vtkMaskPoints> mask;
            mask->SetOnRatio(1);
            mask->SetRandomMode(true);
            mask->SetInputData(aesReader.GetBoundarys()[i][j].dataset);
            mask->Update();

            vtkSmartPointer<vtkDataSetMapper> velocityMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            auto &glyph3D = boundarys[i][j].glyph3D;
            if (glyph3D == nullptr) 
            {
                glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
                vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
                arrow->Update();
                glyph3D->SetSourceData(arrow->GetOutput());
                
                glyph3D->SetScaleModeToScaleByVector();
                glyph3D->SetVectorModeToUseVector();
                glyph3D->SetColorMode(1);
            }
                glyph3D->SetInputData(mask->GetOutput());
                glyph3D->SetScaleFactor(aesReader.GetScaleFactor());

            glyph3D->Update();
            velocityMapper->SetInputData(glyph3D->GetOutput());
            velocityMapper->ScalarVisibilityOff();
            velocityMapper->Update();
            boundarys[i][j].velocityActor->SetMapper(velocityMapper);
            boundarys[i][j].velocityActor->GetProperty()->SetColor(0,0,0);
        }
    }
    cout << "end create velocity actor." << endl;
    hasVector = true;
}

void vtkDisplayWindow::AddVelocityActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->AddActor(y.velocityActor);
        }
    }
}

void vtkDisplayWindow::RemoveVelocityActor()
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            renderer->RemoveActor(y.velocityActor);
        }
    }
}

void vtkDisplayWindow::ActivateAxiesWidget()
{
    auxiliarys.axisWidget->SetEnabled(1);
}

void vtkDisplayWindow::InActivateAxiesWidget()
{
    auxiliarys.axisWidget->SetEnabled(0);
}

void vtkDisplayWindow::ActivateScalarBarWidget()
{
    auxiliarys.scalarBarWidget->SetEnabled(1);
}

void vtkDisplayWindow::InActivateScalarBarWidget()
{
    auxiliarys.scalarBarWidget->SetEnabled(0);
}

void vtkDisplayWindow::SetScalarBar(double m, double M, int number, int flowNumber)
{
    aesReader.ChangeScalarBar(m,M,number,flowNumber);
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            y.contourActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(aesReader.GetFlows()[flowNumber].name.c_str());
            y.contourActor->GetMapper()->SetScalarRange(m,M);
            y.contourActor->GetMapper()->SetLookupTable(aesReader.GetFlows()[flowNumber].scalarBar->GetLookupTable());
            y.contourActor->GetMapper()->Update();
        }
    }
    
    auxiliarys.scalarBarWidget->SetScalarBarActor(aesReader.GetFlows()[flowNumber].scalarBar);
    curFlow = flowNumber;
}

void vtkDisplayWindow::SetScalarBarSize(double width, double height)
{
    // 获取ScalarBarWidget的表示对象
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        auxiliarys.scalarBarWidget->GetRepresentation());
    
    if (rep)
    {
        // 设置ScalarBar的尺寸（相对于窗口的比例，范围0-1）
        // Position2是相对尺寸，表示ScalarBar占据窗口的宽度和高度比例
        rep->SetPosition2(width, height);
        
        // 如果需要同时调整位置以保持在视图内，可以获取当前位置并调整
        double* currentPos = rep->GetPosition();
        // 确保ScalarBar不会超出窗口边界
        if (currentPos[0] + width > 1.0)
        {
            rep->SetPosition(1.0 - width, currentPos[1]);
        }
        if (currentPos[1] + height > 1.0)
        {
            rep->SetPosition(currentPos[0], 1.0 - height);
        }
        
        // 更新表示
        rep->Modified();
    }
}

void vtkDisplayWindow::SetScalarBarVisibility(bool visible)
{
    if (visible)
    {
        ActivateScalarBarWidget();
    }
    else
    {
        InActivateScalarBarWidget();
    }
    cout << "ScalarBar visibility set to: " << (visible ? "visible" : "hidden") << endl;
}

void vtkDisplayWindow::SetScalarBarOrientation(bool isVertical)
{
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        auxiliarys.scalarBarWidget->GetRepresentation());
    
    if (rep)
    {
        vtkScalarBarActor* scalarBar = rep->GetScalarBarActor();
        if (scalarBar)
        {
            if (isVertical)
            {
                scalarBar->SetOrientationToVertical();
                // 垂直时的默认位置和大小
                rep->SetPosition(0.9, 0.1);
                rep->SetPosition2(0.1, 0.8);
            }
            else
            {
                scalarBar->SetOrientationToHorizontal();
                // 水平时的默认位置和大小
                rep->SetPosition(0.1, 0.05);
                rep->SetPosition2(0.8, 0.1);
            }
            rep->Modified();
            cout << "ScalarBar orientation set to: " << (isVertical ? "Vertical" : "Horizontal") << endl;
        }
    }
}

void vtkDisplayWindow::SetScalarBarPosition(double x, double y)
{
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        auxiliarys.scalarBarWidget->GetRepresentation());
    
    if (rep)
    {
        // 设置ScalarBar的位置（左下角坐标，范围0-1）
        rep->SetPosition(x, y);
        rep->Modified();
        cout << "ScalarBar position set to: (" << x << ", " << y << ")" << endl;
    }
}

void vtkDisplayWindow::SetScalarBarTitle(const std::string& title)
{
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        auxiliarys.scalarBarWidget->GetRepresentation());
    
    if (rep)
    {
        vtkScalarBarActor* scalarBar = rep->GetScalarBarActor();
        if (scalarBar)
        {
            scalarBar->SetTitle(title.c_str());
            scalarBar->DrawAnnotationsOn();
            rep->Modified();
            cout << "ScalarBar title set to: " << title << endl;
        }
    }
}

void vtkDisplayWindow::SetScalarBarTextColor(double r, double g, double b)
{
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        auxiliarys.scalarBarWidget->GetRepresentation());
    
    if (rep)
    {
        vtkScalarBarActor* scalarBar = rep->GetScalarBarActor();
        if (scalarBar)
        {
            // 设置标题文字颜色
            scalarBar->GetTitleTextProperty()->SetColor(r, g, b);
            // 设置标签文字颜色
            scalarBar->GetLabelTextProperty()->SetColor(r, g, b);
            // 设置注释文字颜色
            scalarBar->GetAnnotationTextProperty()->SetColor(r, g, b);
            rep->Modified();
            cout << "ScalarBar text color set to RGB(" << r << ", " << g << ", " << b << ")" << endl;
        }
    }
}

void vtkDisplayWindow::SetScalarBarFont(const std::string& family, int size, bool bold, bool italic)
{
    vtkScalarBarRepresentation* rep = vtkScalarBarRepresentation::SafeDownCast(
        auxiliarys.scalarBarWidget->GetRepresentation());
    
    if (rep)
    {
        vtkScalarBarActor* scalarBar = rep->GetScalarBarActor();
        if (scalarBar)
        {
            // 设置标题字体
            vtkTextProperty* titleProp = scalarBar->GetTitleTextProperty();
            titleProp->SetFontSize(size + 4);  // 标题稍大一些
            titleProp->SetBold(bold);
            titleProp->SetItalic(italic);
            
            // 设置字体系列
            if (family == "Arial")
                titleProp->SetFontFamilyToArial();
            else if (family == "Times New Roman" || family == "Times")
                titleProp->SetFontFamilyToTimes();
            else if (family == "Courier New" || family == "Courier")
                titleProp->SetFontFamilyToCourier();
            else
                titleProp->SetFontFamilyToArial();  // 默认
            
            // 设置标签字体
            vtkTextProperty* labelProp = scalarBar->GetLabelTextProperty();
            labelProp->SetFontSize(size);
            labelProp->SetBold(bold);
            labelProp->SetItalic(italic);
            
            if (family == "Arial")
                labelProp->SetFontFamilyToArial();
            else if (family == "Times New Roman" || family == "Times")
                labelProp->SetFontFamilyToTimes();
            else if (family == "Courier New" || family == "Courier")
                labelProp->SetFontFamilyToCourier();
            else
                labelProp->SetFontFamilyToArial();
            
            rep->Modified();
            cout << "ScalarBar font set to: " << family << ", size=" << size 
                 << ", bold=" << bold << ", italic=" << italic << endl;
        }
    }
}


void vtkDisplayWindow::VisiableOutlineActor()
{
    for (auto &x : auxiliarys.outlineActors)
    {
        x->SetVisibility(true);
    }
}

void vtkDisplayWindow::InVisiableOutlineActor()
{
    for (auto &x : auxiliarys.outlineActors)
    {
        x->SetVisibility(false);
    }
}

void vtkDisplayWindow::SetVectorScaleFactor(double scale)
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            y.glyph3D->SetScaleFactor(scale);
            y.glyph3D->Update();
        }
    }
}

void vtkDisplayWindow::xoyViewRender()
{
    auto camera = renderer->GetActiveCamera();
    camera->SetFocalPoint(0,0,0);
    camera->SetPosition(0,0,1);
    camera->SetViewUp(0,1,0);
    renderer->ResetCamera();
}

void vtkDisplayWindow::xozViewRender()
{
    auto camera = renderer->GetActiveCamera();
    camera->SetFocalPoint(0,0,0);
    camera->SetPosition(0,-1,0);
    camera->SetViewUp(0,0,1);
    renderer->ResetCamera();
}

void vtkDisplayWindow::yozViewRender()
{
    auto camera = renderer->GetActiveCamera();
    camera->SetFocalPoint(0,0,0);
    camera->SetPosition(1,0,0);
    camera->SetViewUp(0,0,1);
    renderer->ResetCamera();    
}

void vtkDisplayWindow::xyzViewRender()
{
    auto camera = renderer->GetActiveCamera();
    camera->SetFocalPoint(0,0,0);
    camera->SetPosition(1,1,1);
    camera->SetViewUp(0,0,1);
    renderer->ResetCamera();
}

void vtkDisplayWindow::CreateIsoSurfaceActor()
{
    deriveds.contourFilter = vtkSmartPointer<vtkContourFilter>::New();
    deriveds.contourFilter->SetInputData(aesReader.GetTotalGrid());
    auto range = aesReader.GetFlows()[0].range;
    deriveds.contourFilter->SetValue(0, range[0] + (range[1] - range[0]) * 0.5);
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(deriveds.contourFilter->GetOutputPort());
    mapper->SetLookupTable(aesReader.GetFlows()[0].scalarBar->GetLookupTable());
    mapper->SetScalarRange(aesReader.GetFlows()[0].scalarBar->GetLookupTable()->GetRange());
    deriveds.isoSurfaceActor = vtkSmartPointer<vtkActor>::New();
    deriveds.isoSurfaceActor->SetMapper(mapper);
    renderer->AddActor(deriveds.isoSurfaceActor);
    deriveds.isoSurfaceActor->VisibilityOff();

    hasIsoSurface = true;
}

void vtkDisplayWindow::AddIsoSurfaceActor()
{
    deriveds.isoSurfaceActor->VisibilityOn();
}

void vtkDisplayWindow::RemoveIsoSurfaceActor()
{
    deriveds.isoSurfaceActor->VisibilityOff();
}

void vtkDisplayWindow::ChangeFloodNumber(int floodNumber)
{
    deriveds.isoSurfaceActor->GetMapper()->SetLookupTable(aesReader.GetFlows()[floodNumber].scalarBar->GetLookupTable());
    deriveds.isoSurfaceActor->GetMapper()->SetScalarRange(aesReader.GetFlows()[floodNumber].scalarBar->GetLookupTable()->GetRange());
    deriveds.isoSurfaceActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(aesReader.GetFlows()[floodNumber].name.c_str());
    isoSurfaceFloodNumber = floodNumber;
}

void vtkDisplayWindow::IsoSurfaceChangeFlowNumber(int flowNumber)
{
    aesReader.GetTotalGrid()->GetPointData()->SetActiveScalars(aesReader.GetFlows()[flowNumber].name.c_str());
    auto range = aesReader.GetFlows()[flowNumber].range;
    deriveds.contourFilter->SetValue(0, range[0] + (range[1]-range[0]) * 0.5);
    deriveds.contourFilter->Update();
    isoSurfaceFlowNumber = flowNumber;
}

void vtkDisplayWindow::SetIsoSurfaceValue(double value)
{
    deriveds.contourFilter->SetValue(0, value);
    deriveds.contourFilter->Update();
}

void vtkDisplayWindow::AddNewCutplane() {
    AddNewCutplane(nullptr,nullptr);
}

void vtkDisplayWindow::AddNewCutplane(double* origin, double* normal)
{
    auto totalMesh = aesReader.GetTotalGrid();
    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
    
    // 如果提供了参数，使用提供的参数；否则使用默认值
    if (origin && normal) {
        plane->SetOrigin(origin);
        plane->SetNormal(normal);
        std::cout << "[Debug] Creating cutplane with custom parameters: origin(" 
                  << origin[0] << ", " << origin[1] << ", " << origin[2] 
                  << ") normal(" << normal[0] << ", " << normal[1] << ", " << normal[2] << ")" << std::endl;
    } else {
        plane->SetOrigin(0,0,0);
        plane->SetNormal(1,0,0);
        std::cout << "[Debug] Creating cutplane with default parameters" << std::endl;
    }
    
    cutter->SetInputData(totalMesh);
    cutter->SetCutFunction(plane);
    
    // 如果是第一个cutplane，创建共享的LookupTable
    if (deriveds.cutplanes.empty()) {
        // 添加边界检查，防止程序卡住
        auto flows = aesReader.GetFlows();
        if (flows.empty() || curFlow < 0 || curFlow >= flows.size()) {
            std::cerr << "[Error] Invalid flow data or curFlow index: " << curFlow 
                      << ", flows size: " << flows.size() << std::endl;
            return;
        }
        
        deriveds.cutplaneLookupTable = vtkSmartPointer<vtkLookupTable>::New();
        deriveds.cutplaneLookupTable->SetNumberOfTableValues(10);
        deriveds.cutplaneLookupTable->SetRange(flows[curFlow].range);
        deriveds.cutplaneLookupTable->SetHueRange(0.6667, 0.0);  // 蓝到红渐变，与主模型一致
        deriveds.cutplaneLookupTable->Build();
        
        // 初始化颜色映射参数
        deriveds.cutplaneColorMapping.minValue = flows[curFlow].range[0];
        deriveds.cutplaneColorMapping.maxValue = flows[curFlow].range[1];
        deriveds.cutplaneColorMapping.numberOfColors = 256;
        deriveds.cutplaneColorMapping.useCustomRange = false;
        
        std::cout << "[Debug] Created shared cutplane LookupTable with range: [" 
                  << deriveds.cutplaneColorMapping.minValue << ", " 
                  << deriveds.cutplaneColorMapping.maxValue << "]" << std::endl;
    }
    
    // 创建mapper并使用共享的LookupTable
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cutter->GetOutputPort());
    mapper->SetLookupTable(deriveds.cutplaneLookupTable);
    mapper->SetScalarRange(deriveds.cutplaneLookupTable->GetRange());
    
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    
    // 添加到容器
    deriveds.cutplanes.emplace_back(plane);
    deriveds.cutters.emplace_back(cutter);
    deriveds.cutplaneActors.emplace_back(actor);
    
    renderer->AddActor(deriveds.cutplaneActors.back());
    
    std::cout << "[Debug] Added cutplane " << (deriveds.cutplanes.size() - 1) 
              << ", total cutplanes: " << deriveds.cutplanes.size() << std::endl;
}

void vtkDisplayWindow::SetCutplane(int cutplaneNumber, double *origin, double *normal)
{
    deriveds.cutplanes[cutplaneNumber]->SetOrigin(origin);
    deriveds.cutplanes[cutplaneNumber]->SetNormal(normal);
    deriveds.cutters[cutplaneNumber]->Update();
}

void vtkDisplayWindow::AddCutplaneActors()
{
    for (auto &x : deriveds.cutplaneActors)
    {
        renderer->AddActor(x);
    }
}

void vtkDisplayWindow::RemoveCutplaneActors()
{
    for (auto &x : deriveds.cutplaneActors)
    {
        renderer->RemoveActor(x);
    }
}

void vtkDisplayWindow::AddCutplane(int number)
{
    deriveds.cutplaneActors[number]->VisibilityOn();
}

void vtkDisplayWindow::RemoveCutplane(int number)
{
    deriveds.cutplaneActors[number]->VisibilityOff();
}

void vtkDisplayWindow::SetActorTransparancy(double opacity)
{
    for (auto &x : boundarys)
    {
        for (auto &y : x)
        {
            y.contourActor->GetProperty()->SetOpacity(opacity);
            y.shadeActor->GetProperty()->SetOpacity(opacity);
        }
    }
    std::cout<< "[Debug] SetActorTransparancy "<<std::endl;
}

void vtkDisplayWindow::SetBoundaryTransparency(int meshNumber, int boundaryNumber, double opacity)
{
    // 检查索引有效性
    if (meshNumber < 0 || meshNumber >= boundarys.size()) {
        std::cout << "[Error] Invalid meshNumber: " << meshNumber << std::endl;
        return;
    }
    
    if (boundaryNumber < 0 || boundaryNumber >= boundarys[meshNumber].size()) {
        std::cout << "[Error] Invalid boundaryNumber: " << boundaryNumber << std::endl;
        return;
    }
    
    // 设置指定boundary的透明度
    BasicObject& boundary = boundarys[meshNumber][boundaryNumber];
    if (boundary.contourActor) {
        boundary.contourActor->GetProperty()->SetOpacity(opacity);
    }
    if (boundary.shadeActor) {
        boundary.shadeActor->GetProperty()->SetOpacity(opacity);
    }
    if (boundary.meshActor) {
        boundary.meshActor->GetProperty()->SetOpacity(opacity);
    }
    if (boundary.edgeActor) {
        boundary.edgeActor->GetProperty()->SetOpacity(opacity);
    }
    if (boundary.velocityActor) {
        boundary.velocityActor->GetProperty()->SetOpacity(opacity);
    }
    
    // 立即刷新渲染窗口以显示透明度变化
    renderWindow->Render();
    
    std::cout << "[Debug] SetBoundaryTransparency: mesh=" << meshNumber 
              << ", boundary=" << boundaryNumber << ", opacity=" << opacity << std::endl;
}

void vtkDisplayWindow::SetSliceTransparency(int sliceNumber, double opacity)
{
    // 检查索引有效性
    if (sliceNumber < 0 || sliceNumber >= deriveds.cutplaneActors.size()) {
        std::cout << "[Error] Invalid sliceNumber: " << sliceNumber << std::endl;
        return;
    }
    
    // 设置指定slice的透明度
    if (deriveds.cutplaneActors[sliceNumber]) {
        deriveds.cutplaneActors[sliceNumber]->GetProperty()->SetOpacity(opacity);
    }
    
    // 立即刷新渲染窗口以显示透明度变化
    renderWindow->Render();
    
    std::cout << "[Debug] SetSliceTransparency: slice=" << sliceNumber 
              << ", opacity=" << opacity << std::endl;
}

void vtkDisplayWindow::SetActorLighting(bool flag)
{
    if (flag)
    {
        for (auto &x : boundarys) {
            for (auto &y: x) {
                y.contourActor->GetProperty()->SetAmbient(0);
                y.shadeActor->GetProperty()->SetAmbient(0);
            }
        }
    }
    else
    {
        for (auto &x : boundarys)
        {
            for (auto &y : x)
            {
                y.contourActor->GetProperty()->SetAmbient(1);  
                y.shadeActor->GetProperty()->SetAmbient(1);
            }
        }
    }
}


void vtkDisplayWindow::SetBackground()
{
    renderer->SetUseDepthPeeling(1);
    renderer->SetOcclusionRatio(0.1);
    renderer->SetMaximumNumberOfPeels(100);

    // renderWindow->SetMultiSamples(1);
    // renderWindow->SetAlphaBitPlanes(1);


    renderer->SetBackground(1.0, 1.0, 1.0);
    renderer->SetGradientBackground(false);

}

void vtkDisplayWindow::SetBackgroundStyle(const QString &style)
{
    if (style == "Sky Blue") {
        // 天蓝色渐变背景 (默认)
        renderer->SetBackground(1.0, 1.0, 1.0);           // 白色
        renderer->SetBackground2(0.529, 0.8078, 0.92157); // 天蓝色
        renderer->SetGradientBackground(true);
    } else if (style == "White") {
        // 纯白色背景
        renderer->SetBackground(1.0, 1.0, 1.0);           // 白色
        renderer->SetGradientBackground(false);
    }
    
    // 刷新渲染窗口
    renderWindow->Render();
}

std::vector<vtkSmartPointer<vtkActor>> vtkDisplayWindow::CreateMeridionalPlane(double minRange, double maxRange)
{

  if(MeridionalPlaneActor.empty())
  {
    auto BndGrid = aesReader.GetBoundarys();
    auto Flow = aesReader.GetFlows();
    vtkSmartPointer<vtkUnstructuredGrid> md = vtkSmartPointer<vtkUnstructuredGrid>::New();

    for(int zone = 0; zone < BndGrid.size(); zone++) {
      for (int i = 0; i < BndGrid[zone].size(); i++) {
        if (BndGrid[zone][i].type == 9) {
          vtkSmartPointer<vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
          appendFilter->AddInputData(md);
          appendFilter->AddInputData(BndGrid[zone][i].dataset);
          appendFilter->Update();

          md->DeepCopy(appendFilter->GetOutput());
          break;
        }
      }
    }
    vtkSmartPointer<vtkPoints> planepts = vtkSmartPointer<vtkPoints>::New();
    for (int m = 0; m < md->GetNumberOfPoints(); m++) {
      double y = md->GetPoint(m)[1];
      double z = md->GetPoint(m)[2];
      double r = sqrt(y * y + z * z);
      planepts->InsertNextPoint(md->GetPoint(m)[0], r * cos(0), r * sin(0));
    }

  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

  // 设置点
  polyData->SetPoints(planepts);

  polyData->SetPolys(md->GetCells());

      MeridionalPlane.emplace_back(polyData);

      ChangeMeridionalFlow(Flow[0].range[0], Flow[0].range[1], curFlow);
      return MeridionalPlaneActor;
    }
}
std::vector<vtkSmartPointer<vtkActor>> vtkDisplayWindow::ChangeMeridionalFlow(double minRange, double maxRange, int flowNumber)
{
    RemoveMeridianActor();
    MeridionalPlaneActor.clear();
    
    if (MeridionalPlane.empty()) {
        std::cout << "[DEBUG] MeridionalPlane is empty " << std::endl;
        return {};
    }

    auto Flow = aesReader.GetFlows();
    std::string currentFlowName = Flow[flowNumber].name;
    vtkPolyData* targetPD = MeridionalPlane[0];

    // Check if the target flow variable already exists in the cached plane
    if (!targetPD->GetPointData()->HasArray(currentFlowName.c_str()))
    {
        std::cout << "[B2B] Caching all meridional flow variables (First Run)..." << std::endl;
        
        auto TotalMesh = aesReader.GetTotalGrid();
        
        // 1. Build Source Geometry (Projected Points) - Only once
        vtkSmartPointer<vtkPolyData> ply = vtkSmartPointer<vtkPolyData>::New();
        vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
        pts->SetNumberOfPoints(TotalMesh->GetNumberOfPoints());
        
        for(int i = 0; i < TotalMesh->GetNumberOfPoints(); i++){
            double* p = TotalMesh->GetPoint(i);
            double nr = sqrt(p[1] * p[1] + p[2] * p[2]);
            pts->SetPoint(i, p[0], nr, 0.0); // Projected to (x, r, 0)
        }
        ply->SetPoints(pts);

        // 2. Add ALL flow variables to Source
        for (size_t f = 0; f < Flow.size(); f++) {
            vtkSmartPointer<vtkDoubleArray> scalar = vtkSmartPointer<vtkDoubleArray>::New();
            scalar->SetName(Flow[f].name.c_str());
            scalar->SetNumberOfValues(TotalMesh->GetNumberOfPoints());
            for(int i = 0; i < TotalMesh->GetNumberOfPoints(); i++) {
                scalar->SetValue(i, Flow[f].datas[i]);
            }
            ply->GetPointData()->AddArray(scalar);
        }

        // 3. Interpolate ALL variables at once
        vtkSmartPointer<vtkPointInterpolator> inter = vtkSmartPointer<vtkPointInterpolator>::New();
        inter->SetInputData(targetPD);
        inter->SetSourceData(ply);
        
        vtkSmartPointer<vtkVoronoiKernel> kernel = vtkSmartPointer<vtkVoronoiKernel>::New();
        inter->SetKernel(kernel);
        inter->Update();

        // 4. Update Cache (ShallowCopy keeps the geometry and interpolated arrays)
        targetPD->ShallowCopy(inter->GetOutput());
    }

    // 5. Switch Active Scalar (Instant)
    targetPD->GetPointData()->SetActiveScalars(currentFlowName.c_str());

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(targetPD); // Use cached data directly
    mapper->SetLookupTable(Flow[flowNumber].scalarBar->GetLookupTable());
    mapper->SetScalarRange(minRange, maxRange);
    mapper->Update();

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    
    MeridionalPlaneActor.emplace_back(actor);

    return MeridionalPlaneActor;
}
void vtkDisplayWindow::VisualizeMeridonalPlane()
{
    if(MeridionalPlaneActor[0]->GetVisibility() == true)
    {
        for(int i = 0; i < MeridionalPlaneActor.size(); i++)
        {
            MeridionalPlaneActor[i]->VisibilityOff();
        }
    }
    else{
        for(int i = 0; i < MeridionalPlaneActor.size(); i++)
        {
            MeridionalPlaneActor[i]->VisibilityOn();
        }
    }

}

void vtkDisplayWindow::CreateConstHeight(double height)
{
//    if(aesReader.node_radius.empty())
//    {
//        std::cout << "can not use this function" << std::endl;
//        return;
//    }
    auto Mesh = aesReader.GetZoneGrids();
    auto Flow = aesReader.GetFlows();
    auto BndGrid = aesReader.GetBoundarys();
    auto TotalMeshes = aesReader.GetTotalGrid();
    vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputData(TotalMeshes);
    TotalMeshes->GetPointData()->SetActiveScalars("radius");
    contour->SetValue(0,height);
    contour->Update();
//    contour->GetOutput()->GetPointData()->GetArray(curFlow);
    contour->GetOutput()->GetPointData()->SetActiveScalars(Flow[curFlow].name.c_str());
    ConstHeightPlane.emplace_back(contour);
    vtkSmartPointer<vtkPolyData> polydata;
//    contour->SetArrayComponent(curFlow);
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(contour->GetOutputPort());
    mapper->SetScalarRange(Flow[curFlow].range);
    mapper->SetLookupTable(Flow[curFlow].scalarBar->GetLookupTable());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    ConstHeightPlaneActor.emplace_back(actor);
    renderer->AddActor(actor);
}

void vtkDisplayWindow::ChangeConstHeightFlow(int flowNumber)
{
    auto Flow = aesReader.GetFlows();
    for(int i = 0;i < ConstHeightPlaneActor.size();i++)
    {
        renderer->RemoveActor(ConstHeightPlaneActor[i]);
    }
    ConstHeightPlaneActor.clear();
    for(int i = 0; i < ConstHeightPlane.size(); i++)
    {
        ConstHeightPlane[i]->GetOutput()->GetPointData()->SetActiveScalars(Flow[flowNumber].name.c_str());
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(ConstHeightPlane[i]->GetOutputPort());
        mapper->SetScalarRange(Flow[flowNumber].range);
        mapper->SetLookupTable(Flow[flowNumber].scalarBar->GetLookupTable());
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        ConstHeightPlaneActor.emplace_back(actor);
        renderer->AddActor(actor);
    }
}

std::vector<vtkSmartPointer<vtkActor>> vtkDisplayWindow::CreateBladeToBladePlane(double span)
{
    std::vector<vtkSmartPointer<vtkActor>> actors;
    
    std::cout << "[B2B] Creating blade-to-blade plane at span = " << span << std::endl;
    
    // Clear previous B2B data to avoid duplication
    BladeToBladePlane.clear();
    BladeToBladePlaneActor.clear();
    
    // Check if node_radius exists
    if(aesReader.node_radius.empty())
    {
        std::cout << "[B2B ERROR] node_radius field not found!" << std::endl;
        return actors;
    }
    
    auto Flow = aesReader.GetFlows();
    auto TotalMeshes = aesReader.GetTotalGrid();
    
    const double PI = 3.14159265358979323846;
    
    if (!TotalMeshes->GetPointData()->HasArray("radius"))
    {
        std::cout << "[B2B ERROR] VTK grid does not have 'radius' array!" << std::endl;
        return actors;
    }
    
    // Extract contour at given span from TotalMeshes
    vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
    contour->SetInputData(TotalMeshes);
    TotalMeshes->GetPointData()->SetActiveScalars("radius");
    contour->SetValue(0, span);
    contour->Update();
    
    vtkPolyData* contourOutput = contour->GetOutput();
    int numPoints = contourOutput->GetNumberOfPoints();
    
    std::cout << "[B2B] Contour extracted: " << numPoints << " points" << std::endl;
    
    if (numPoints == 0)
    {
        std::cout << "[B2B ERROR] Empty contour! Check span value." << std::endl;
        auto radiusArray = TotalMeshes->GetPointData()->GetArray("radius");
        if (radiusArray) {
            double range[2];
            radiusArray->GetRange(range);
            std::cout << "[B2B] Valid radius range: [" << range[0] << ", " << range[1] << "]" << std::endl;
        }
        return actors;
    }
    
    // Calculate coordinates for all points
    std::vector<double> origX(numPoints), origTheta(numPoints), origRadius(numPoints);
    double sumSin = 0.0, sumCos = 0.0;
    double xMin = std::numeric_limits<double>::max();
    double rAtXMin = 0.0;
    
    for (int i = 0; i < numPoints; i++)
    {
        double p[3];
        contourOutput->GetPoint(i, p);
        origX[i] = p[0];
        double y = p[1];
        double z = p[2];
        origRadius[i] = std::sqrt(y*y + z*z);
        origTheta[i] = std::atan2(z, y);
        
        sumSin += std::sin(origTheta[i]);
        sumCos += std::cos(origTheta[i]);
        
        if (origX[i] < xMin)
        {
            xMin = origX[i];
            rAtXMin = origRadius[i];
        }
    }
    
    // Calculate circular mean reference theta
    double refTheta = std::atan2(sumSin, sumCos);
    std::cout << "[B2B] Reference theta: " << refTheta << " rad (" 
              << (refTheta * 180.0 / PI) << " deg)" << std::endl;
    std::cout << "[B2B] X min: " << xMin << ", R at X min: " << rAtXMin << std::endl;
    
    // Adjust theta values to be continuous around refTheta
    std::vector<double> adjustedTheta(numPoints);
    for (int i = 0; i < numPoints; i++)
    {
        double delta = origTheta[i] - refTheta;
        while (delta > PI) delta -= 2.0 * PI;
        while (delta < -PI) delta += 2.0 * PI;
        adjustedTheta[i] = refTheta + delta;
    }
    
    // For constant-radius surface, use x directly as horizontal coordinate
    double xMax = *std::max_element(origX.begin(), origX.end());
    std::cout << "[B2B] X range: [" << xMin << ", " << xMax << "]" << std::endl;
    
    // Calculate average radius (should be approximately constant on this surface)
    double avgRadius = 0.0;
    for (int i = 0; i < numPoints; i++)
    {
        avgRadius += origRadius[i];
    }
    avgRadius /= numPoints;
    std::cout << "[B2B] Average radius: " << avgRadius << std::endl;
    
    // Create unwrapped points
    // X = axial coordinate
    // Y = avgRadius * theta (using average radius for consistency)
    vtkSmartPointer<vtkPoints> unwrappedPoints = vtkSmartPointer<vtkPoints>::New();
    unwrappedPoints->SetNumberOfPoints(numPoints);
    
    for (int i = 0; i < numPoints; i++)
    {
        double x = origX[i];
        double theta = adjustedTheta[i];
        unwrappedPoints->SetPoint(i, x, avgRadius * theta, 0.0);
    }
    
    // Create unwrapped polydata
    vtkSmartPointer<vtkPolyData> unwrappedData = vtkSmartPointer<vtkPolyData>::New();
    unwrappedData->SetPoints(unwrappedPoints);
    unwrappedData->SetPolys(contourOutput->GetPolys());
    unwrappedData->GetPointData()->ShallowCopy(contourOutput->GetPointData());
    
    std::cout << "[B2B] Unwrapped to 2D (M'=meridional, Y=r*theta)" << std::endl;
    
    // Store for later use
    vtkSmartPointer<vtkContourFilter> dummyFilter = vtkSmartPointer<vtkContourFilter>::New();
    dummyFilter->SetInputData(unwrappedData);
    dummyFilter->Update();
    BladeToBladePlane.emplace_back(dummyFilter);
    
    // Set active scalars and create mapper
    unwrappedData->GetPointData()->SetActiveScalars(Flow[curFlow].name.c_str());
    
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(unwrappedData);
    mapper->SetScalarRange(Flow[curFlow].range);
    mapper->SetLookupTable(Flow[curFlow].scalarBar->GetLookupTable());
    
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    
    // Disable lighting for 2D plane to avoid shading artifacts
    actor->GetProperty()->SetAmbient(1.0);   // Full ambient lighting
    actor->GetProperty()->SetDiffuse(0.0);   // No diffuse lighting
    actor->GetProperty()->SetSpecular(0.0);  // No specular lighting
    
    BladeToBladePlaneActor.emplace_back(actor);
    actors.emplace_back(actor);
    
    std::cout << "[B2B SUCCESS] Created unwrapped 2D blade-to-blade plane" << std::endl;
    return actors;
}

std::vector<vtkSmartPointer<vtkActor>> vtkDisplayWindow::ChangeBladeToBladePlaneFlow(int flowNumber)
{
    std::vector<vtkSmartPointer<vtkActor>> actors;
    auto Flow = aesReader.GetFlows();
    
    BladeToBladePlaneActor.clear();
    
    for(int i = 0; i < BladeToBladePlane.size(); i++)
    {
        BladeToBladePlane[i]->GetOutput()->GetPointData()->SetActiveScalars(Flow[flowNumber].name.c_str());
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(BladeToBladePlane[i]->GetOutputPort());
        mapper->SetScalarRange(Flow[flowNumber].range);
        mapper->SetLookupTable(Flow[flowNumber].scalarBar->GetLookupTable());
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        
        // Disable lighting for 2D plane to avoid shading artifacts
        actor->GetProperty()->SetAmbient(1.0);   // Full ambient lighting
        actor->GetProperty()->SetDiffuse(0.0);   // No diffuse lighting
        actor->GetProperty()->SetSpecular(0.0);  // No specular lighting
        
        BladeToBladePlaneActor.emplace_back(actor);
        actors.emplace_back(actor);
    }
    
    return actors;
}

vtkSmartPointer<vtkPolyData> ConvertUnstructuredGridToPolyData(vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid)
{
  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

  // 设置点
  polyData->SetPoints(unstructuredGrid->GetPoints());

  // 用于存储单元
  vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();

  for (vtkIdType i = 0; i < unstructuredGrid->GetNumberOfCells(); ++i)
  {
    vtkSmartPointer<vtkCell> cell = unstructuredGrid->GetCell(i);

    if (cell->GetCellType() == VTK_TRIANGLE || cell->GetCellType() == VTK_QUAD)
    {
      polys->InsertNextCell(cell);
    }
  }

  polyData->SetPolys(polys);

  return polyData;
}

double* vtkDisplayWindow::GetModelBounds()
{
    if (!hasGrid)
    {
        std::cout << "没有加载网格数据，无法获取边界值" << std::endl;
        return nullptr;
    }
    
    // 获取总网格数据的边界值
    return aesReader.GetTotalGrid()->GetBounds();
    
    // 返回的数组包含6个值：[xmin, xmax, ymin, ymax, zmin, zmax]
}

std::vector<std::string> vtkDisplayWindow::GetZoneNames()
{
    std::vector<std::string> names;
    auto boundarys = aesReader.GetBoundarys();
    
    for (int i = 0; i < boundarys.size(); i++)
    {
        if (boundarys[i].size() > 0)
        {
            std::string zoneName = boundarys[i][0].zoneName;
            bool exists = false;
            for (const auto& name : names)
            {
                if (name == zoneName) { exists = true; break; }
            }
            if (!exists)
            {
                names.push_back(zoneName);
            }
        }
    }
    return names;
}

void vtkDisplayWindow::ClearPeriodicCopies()
{
    // 从渲染器移除所有 Zone 的周期性复制 shade actors
    for (auto& [zoneIdx, actors] : periodicCopyShadeActorsByZone)
    {
        for (auto& actor : actors)
        {
            renderer->RemoveActor(actor);
        }
    }
    periodicCopyShadeActorsByZone.clear();
    
    // 从渲染器移除所有 Zone 的周期性复制 contour actors
    for (auto& [zoneIdx, actors] : periodicCopyContourActorsByZone)
    {
        for (auto& actor : actors)
        {
            renderer->RemoveActor(actor);
        }
    }
    periodicCopyContourActorsByZone.clear();
    
    std::cout << "[Periodic] Cleared all periodic copies from all zones" << std::endl;
}

void vtkDisplayWindow::ClearPeriodicCopiesForZone(int zoneIndex)
{
    // 清除指定 Zone 的周期性复制 shade actors
    if (periodicCopyShadeActorsByZone.find(zoneIndex) != periodicCopyShadeActorsByZone.end())
    {
        for (auto& actor : periodicCopyShadeActorsByZone[zoneIndex])
        {
            renderer->RemoveActor(actor);
        }
        periodicCopyShadeActorsByZone[zoneIndex].clear();
        periodicCopyShadeActorsByZone.erase(zoneIndex);
    }
    
    // 清除指定 Zone 的周期性复制 contour actors
    if (periodicCopyContourActorsByZone.find(zoneIndex) != periodicCopyContourActorsByZone.end())
    {
        for (auto& actor : periodicCopyContourActorsByZone[zoneIndex])
        {
            renderer->RemoveActor(actor);
        }
        periodicCopyContourActorsByZone[zoneIndex].clear();
        periodicCopyContourActorsByZone.erase(zoneIndex);
    }
    
    std::cout << "[Periodic] Cleared periodic copies for zone " << zoneIndex << std::endl;
}

void vtkDisplayWindow::CreatePeriodicCopies(int zoneIndex, int numCopies)
{
    std::vector<int> allBoundaries;
    if (zoneIndex >= 0 && zoneIndex < static_cast<int>(boundarys.size()))
    {
        for (int i = 0; i < static_cast<int>(boundarys[zoneIndex].size()); ++i)
        {
            allBoundaries.push_back(i);
        }
    }
    CreatePeriodicCopies(zoneIndex, numCopies, allBoundaries);
}

void vtkDisplayWindow::CreatePeriodicCopies(int zoneIndex, int numCopies, const std::vector<int> &boundaryIndices)
{
    //* Only clear Current Zone
    ClearPeriodicCopiesForZone(zoneIndex);

    if (numCopies <= 0)
    {
        std::cout << "[Periodic] No copies to create (numCopies=" << numCopies << ")" << std::endl;
        return;
    }

    auto angles = aesReader.angles;

    if (zoneIndex < 0 || zoneIndex >= static_cast<int>(boundarys.size()))
    {
        std::cout << "[Periodic ERROR] Invalid zone index: " << zoneIndex << std::endl;
        return;
    }

    if (zoneIndex >= static_cast<int>(angles.size()))
    {
        std::cout << "[Periodic ERROR] No angle data for zone " << zoneIndex << std::endl;
        return;
    }

    if (boundaryIndices.empty())
    {
        std::cout << "[Periodic] No boundary indices provided for zone " << zoneIndex << std::endl;
        return;
    }

    double angleStep = std::abs(angles[zoneIndex]);
    std::cout << "[Periodic] Zone " << zoneIndex << " angle step: " << angleStep << " degrees" << std::endl;
    std::cout << "[Periodic] Zone has " << boundarys[zoneIndex].size() << " boundaries, selected "
              << boundaryIndices.size() << std::endl;

    for (int copyNum = 1; copyNum <= numCopies; copyNum++)
    {
        double rotationAngle = angleStep * copyNum;

        //* 创建旋转变换（绕 X 轴旋转）
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->RotateX(rotationAngle);

        // 只遍历选中的 boundary
        for (int bndIdx : boundaryIndices)
        {
            if (bndIdx < 0 || bndIdx >= static_cast<int>(boundarys[zoneIndex].size()))
            {
                continue;
            }

            auto& bndObj = boundarys[zoneIndex][bndIdx];

            //* Copy shadeActor
            if (bndObj.shadeActor && bndObj.shadeActor->GetMapper())
            {
                vtkMapper* originalMapper = bndObj.shadeActor->GetMapper();
                vtkDataSet* inputData = originalMapper->GetInput();

                if (inputData && inputData->GetNumberOfPoints() > 0)
                {
                    vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
                    transformFilter->SetInputData(inputData);
                    transformFilter->SetTransform(transform);
                    transformFilter->Update();

                    vtkSmartPointer<vtkDataSetMapper> shadeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
                    shadeMapper->SetInputConnection(transformFilter->GetOutputPort());

                    shadeMapper->SetScalarVisibility(originalMapper->GetScalarVisibility());
                    shadeMapper->SetScalarRange(originalMapper->GetScalarRange());
                    if (originalMapper->GetLookupTable())
                    {
                        shadeMapper->SetLookupTable(originalMapper->GetLookupTable());
                    }

                    vtkSmartPointer<vtkActor> shadeActor = vtkSmartPointer<vtkActor>::New();
                    shadeActor->SetMapper(shadeMapper);

                    shadeActor->GetProperty()->DeepCopy(bndObj.shadeActor->GetProperty());

                    renderer->AddActor(shadeActor);
                    periodicCopyShadeActorsByZone[zoneIndex].push_back(shadeActor);
                }
            }

            //* Copy contourActor
            if (bndObj.contourActor && bndObj.contourActor->GetMapper())
            {
                vtkMapper* originalContourMapper = bndObj.contourActor->GetMapper();
                vtkDataSet* contourInputData = originalContourMapper->GetInput();

                if (contourInputData && contourInputData->GetNumberOfPoints() > 0)
                {
                    // 应用变换
                    vtkSmartPointer<vtkTransformFilter> contourTransformFilter = vtkSmartPointer<vtkTransformFilter>::New();
                    contourTransformFilter->SetInputData(contourInputData);
                    contourTransformFilter->SetTransform(transform);
                    contourTransformFilter->Update();

                    // 创建新的 contour mapper 和 actor
                    vtkSmartPointer<vtkDataSetMapper> contourMapper = vtkSmartPointer<vtkDataSetMapper>::New();
                    contourMapper->SetInputConnection(contourTransformFilter->GetOutputPort());

                    // 复制原始 contour mapper 的标量设置
                    contourMapper->SetScalarVisibility(originalContourMapper->GetScalarVisibility());
                    contourMapper->SetScalarRange(originalContourMapper->GetScalarRange());
                    if (originalContourMapper->GetLookupTable())
                    {
                        contourMapper->SetLookupTable(originalContourMapper->GetLookupTable());
                    }

                    vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
                    contourActor->SetMapper(contourMapper);

                    // 复制原始 contour actor 的属性（包括可见性）
                    contourActor->GetProperty()->DeepCopy(bndObj.contourActor->GetProperty());
                    contourActor->SetVisibility(bndObj.contourActor->GetVisibility());

                    // 添加到渲染器和对应 Zone 的 contour actors 列表
                    renderer->AddActor(contourActor);
                    periodicCopyContourActorsByZone[zoneIndex].push_back(contourActor);
                }
            }
        }

        std::cout << "[Periodic] Created copy " << copyNum << " at rotation " << rotationAngle << " degrees" << std::endl;
    }

    int totalActors = periodicCopyShadeActorsByZone[zoneIndex].size() + 
                      periodicCopyContourActorsByZone[zoneIndex].size();
    std::cout << "[Periodic] Created " << numCopies << " periodic copies for zone " << zoneIndex 
              << " (" << totalActors << " actors: " 
              << periodicCopyShadeActorsByZone[zoneIndex].size() << " shade + "
              << periodicCopyContourActorsByZone[zoneIndex].size() << " contour)" << std::endl;
}

void vtkDisplayWindow::CreatePlanePreview(double value,int currenAxis)
{
    // 获取模型的边界框
    std::cout << "[Debug] Preview Location actual value: " << value << std::endl;

    double bounds[6];
    if (aesReader.GetTotalGrid())
    {
        aesReader.GetTotalGrid()->GetBounds(bounds);
        std::cout << "边界框: [" << bounds[0] << ", " << bounds[1] << ", " 
                  << bounds[2] << ", " << bounds[3] << ", " 
                  << bounds[4] << ", " << bounds[5] << "]" << std::endl;
    }
    else
    {
        std::cout << "未找到模型网格！" << std::endl;
        return;  // 没有模型就不创建平面
    }
    
    // 创建一个平面 - 直接使用边界框中间的x位置
    double position = value;
    double xRange = bounds[1] - bounds[0];
    double yRange = bounds[3] - bounds[2];
    double zRange = bounds[5] - bounds[4];

    if (!xPlaneSource) {
        xPlaneSource = vtkSmartPointer<vtkPlaneSource>::New();
    }

    switch ( currenAxis) {
        case 0: // X轴 - YZ平面
            xPlaneSource->SetCenter(position, 0.0, 0.0);
            xPlaneSource->SetNormal(1.0, 0.0, 0.0);
            xPlaneSource->SetOrigin(position, bounds[2] - 0.1*yRange, bounds[4] - 0.1*zRange);
            xPlaneSource->SetPoint1(position, bounds[3] + 0.1*yRange, bounds[4] - 0.1*zRange);
            xPlaneSource->SetPoint2(position, bounds[2] - 0.1*yRange, bounds[5] + 0.1*zRange);
            break;

        case 1: // Y轴 - XZ平面
            xPlaneSource->SetCenter(0.0, position, 0.0);
            xPlaneSource->SetNormal(0.0, 1.0, 0.0);
            xPlaneSource->SetOrigin(bounds[0] - 0.1*xRange, position, bounds[4] - 0.1*zRange);
            xPlaneSource->SetPoint1(bounds[1] + 0.1*xRange, position, bounds[4] - 0.1*zRange);
            xPlaneSource->SetPoint2(bounds[0] - 0.1*xRange, position, bounds[5] + 0.1*zRange);
            break;

        case 2: // Z轴 - XY平面
            xPlaneSource->SetCenter(0.0, 0.0, position);
            xPlaneSource->SetNormal(0.0, 0.0, 1.0);
            xPlaneSource->SetOrigin(bounds[0] - 0.1*xRange, bounds[2] - 0.1*yRange, position);
            xPlaneSource->SetPoint1(bounds[1] + 0.1*xRange, bounds[2] - 0.1*yRange, position);
            xPlaneSource->SetPoint2(bounds[0] - 0.1*xRange, bounds[3] + 0.1*yRange, position);
            break;
    }
    
    // 创建映射器和Actor - 直接使用平面而不是裁剪的结果
    if (!xPlaneMapper) {
        xPlaneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    }
    xPlaneMapper->SetInputConnection(xPlaneSource->GetOutputPort());
    xPlaneMapper->SetInputConnection(xPlaneSource->GetOutputPort());
    
    if (!xPlaneActor) {
        xPlaneActor = vtkSmartPointer<vtkActor>::New();
    }
    xPlaneActor->SetMapper(xPlaneMapper);

    xPlaneActor->GetProperty()->SetColor(0.0, 0.5, 1.0);
    // xPlaneActor->GetProperty()->SetOpacity(0.3);

    renderer->AddActor(xPlaneActor);
    renderWindow->Render();
}

void vtkDisplayWindow::HidePlanePreview()
{
    // 如果存在预览平面Actor，则从渲染器中移除
    if (xPlaneActor)
    {
        std::cout << "隐藏预览平面" << std::endl;
        renderer->RemoveActor(xPlaneActor);
        renderWindow->Render();
    }
}

void vtkDisplayWindow::SetCutplaneColorMapping(double minValue, double maxValue, int numberOfColors,bool isBanded)
{
    if (!deriveds.cutplaneLookupTable) {
        std::cerr << "No cutplane LookupTable exists" << std::endl;
        return;
    }
    
    // 更新颜色映射参数
    deriveds.cutplaneColorMapping.minValue = minValue;
    deriveds.cutplaneColorMapping.maxValue = maxValue;
    deriveds.cutplaneColorMapping.useCustomRange = true;
    if (isBanded)
        deriveds.cutplaneColorMapping.numberOfColors = numberOfColors;
    else
        deriveds.cutplaneColorMapping.numberOfColors = 256;
    // 更新LookupTable
    UpdateCutplaneColorMapping();
}

void vtkDisplayWindow::UpdateCutplaneColorMapping()
{
    if (!deriveds.cutplaneLookupTable) {
        std::cerr << "No cutplane LookupTable exists" << std::endl;
        return;
    }
    
    auto& colorMapping = deriveds.cutplaneColorMapping;
    
    // 设置LookupTable参数
    deriveds.cutplaneLookupTable->SetNumberOfTableValues(colorMapping.numberOfColors);
    deriveds.cutplaneLookupTable->SetRange(colorMapping.minValue, colorMapping.maxValue);
    deriveds.cutplaneLookupTable->Build();

    // 更新所有cutplane actors的mapper范围
    for (auto& actor : deriveds.cutplaneActors) {
        auto mapper = actor->GetMapper();
        mapper->SetScalarRange(colorMapping.minValue, colorMapping.maxValue);
    }
    
    // 更新ScalarBar
    if (deriveds.cutplaneScalarBar) {
        deriveds.cutplaneScalarBar->SetLookupTable(deriveds.cutplaneLookupTable);
    }
    
    std::cout << "[Debug] Updated shared cutplane color mapping: range[" 
              << colorMapping.minValue << ", " << colorMapping.maxValue 
              << "], colors=" << colorMapping.numberOfColors << std::endl;
}

vtkDisplayWindow::DerivedObject::CutplaneColorMapping vtkDisplayWindow::GetCutplaneColorMapping()
{
    return deriveds.cutplaneColorMapping;
}


void vtkDisplayWindow::InitializeCutplaneScalarBar()
{
    // 确保cutplaneLookupTable存在
    if (!deriveds.cutplaneLookupTable) {
        std::cerr << "[Error] Cannot initialize cutplane ScalarBar: cutplaneLookupTable not found" << std::endl;
        return;
    }
    
    // 如果ScalarBar还没创建，创建它
    if (!deriveds.cutplaneScalarBar) {
        deriveds.cutplaneScalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
        deriveds.cutplaneScalarBar->SetLookupTable(deriveds.cutplaneLookupTable);
        deriveds.cutplaneScalarBar->SetTitle("Slice");
        deriveds.cutplaneScalarBar->SetNumberOfLabels(10);
        
        // 设置位置 - 水平显示在窗口中下方（避免被截断）
        deriveds.cutplaneScalarBar->SetOrientationToHorizontal();
        deriveds.cutplaneScalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
        deriveds.cutplaneScalarBar->GetPositionCoordinate()->SetValue(0.2, 0.1);   // 中下方位置
        deriveds.cutplaneScalarBar->SetWidth(0.6);   // 宽度占窗口60%
        deriveds.cutplaneScalarBar->SetHeight(0.06); // 高度占窗口6%
        
        // 设置字体 - 更小的字体，默认黑色
        deriveds.cutplaneScalarBar->GetTitleTextProperty()->SetFontSize(10);
        deriveds.cutplaneScalarBar->GetTitleTextProperty()->SetColor(0.0, 0.0, 0.0);
        deriveds.cutplaneScalarBar->GetLabelTextProperty()->SetFontSize(8);
        deriveds.cutplaneScalarBar->GetLabelTextProperty()->SetColor(0.0, 0.0, 0.0);
        
        // 确保可见性
        deriveds.cutplaneScalarBar->SetVisibility(1);
        
        std::cout << "[Debug] Initialized cutplane ScalarBar at position (0.2, 0.1) with size (0.6, 0.06)" << std::endl;
    }
}


void vtkDisplayWindow::ShowCutplaneScalarBar()
{
    std::cout << "[Debug] ShowCutplaneScalarBar called" << std::endl;
    
    // 初始化ScalarBar（如果需要）
    InitializeCutplaneScalarBar();
    
    if (deriveds.cutplaneScalarBar) {
        std::cout << "[Debug] cutplaneScalarBar exists" << std::endl;
        
        // 添加到渲染器（如果还没添加）
        if (!renderer->HasViewProp(deriveds.cutplaneScalarBar)) {
            renderer->AddActor2D(deriveds.cutplaneScalarBar);
            std::cout << "[Debug] Added cutplaneScalarBar to renderer" << std::endl;
        } else {
            std::cout << "[Debug] cutplaneScalarBar already in renderer" << std::endl;
        }
        
        // 设置可见
        deriveds.cutplaneScalarBar->SetVisibility(1);
        std::cout << "[Debug] Set cutplaneScalarBar visibility to 1" << std::endl;
        std::cout << "[Debug] Showing cutplane ScalarBar" << std::endl;
    } else {
        std::cout << "[Debug] cutplaneScalarBar is null!" << std::endl;
    }
}

void vtkDisplayWindow::HideCutplaneScalarBar()
{
    if (deriveds.cutplaneScalarBar) {
        // 从渲染器中移除
        if (renderer->HasViewProp(deriveds.cutplaneScalarBar)) {
            renderer->RemoveActor2D(deriveds.cutplaneScalarBar);
        }
        
        // 设置不可见
        deriveds.cutplaneScalarBar->SetVisibility(0);
        std::cout << "[Debug] Hiding cutplane ScalarBar" << std::endl;
    }
}

void vtkDisplayWindow::SetCutplaneColorScheme(int presetIndex, bool reverse)
{
    if (!deriveds.cutplaneLookupTable) {
        std::cerr << "[Error] Cannot set color scheme: cutplaneLookupTable not found" << std::endl;
        return;
    }
    
    std::cout << "[Debug] Setting cutplane color scheme: presetIndex=" << presetIndex 
              << ", reverse=" << reverse << std::endl;
    
    ColorMapPreset& preset = ColorMapPreset::instance();
    
    // 保存当前范围
    double range[2];
    deriveds.cutplaneLookupTable->GetTableRange(range);
    
    if (presetIndex >= 0 && preset.isLoaded()) {
        // 使用 JSON 预设
        preset.applyToLookupTable(presetIndex, deriveds.cutplaneLookupTable, reverse, 256);
        // 恢复范围（applyToLookupTable 可能会重置）
        deriveds.cutplaneLookupTable->SetTableRange(range);
    } else {
        // Fallback 到旧的硬编码方案
        int fallbackScheme = (presetIndex < 0) ? (-presetIndex - 1) : 0;
        switch (fallbackScheme) {
            case 0: // Rainbow
                deriveds.cutplaneLookupTable->SetNumberOfTableValues(256);
                deriveds.cutplaneLookupTable->SetHueRange(0.6667, 0.0);
                deriveds.cutplaneLookupTable->SetSaturationRange(1.0, 1.0);
                deriveds.cutplaneLookupTable->SetValueRange(1.0, 1.0);
                break;
            case 1: // Viridis
                deriveds.cutplaneLookupTable->SetNumberOfTableValues(256);
                deriveds.cutplaneLookupTable->SetHueRange(0.75, 0.167);
                deriveds.cutplaneLookupTable->SetSaturationRange(0.8, 0.9);
                deriveds.cutplaneLookupTable->SetValueRange(0.2, 0.95);
                break;
            case 2: // Gray
                deriveds.cutplaneLookupTable->SetNumberOfTableValues(256);
                deriveds.cutplaneLookupTable->SetHueRange(0.0, 0.0);
                deriveds.cutplaneLookupTable->SetSaturationRange(0.0, 0.0);
                deriveds.cutplaneLookupTable->SetValueRange(0.0, 1.0);
                break;
            default:
                deriveds.cutplaneLookupTable->SetNumberOfTableValues(256);
                deriveds.cutplaneLookupTable->SetHueRange(0.6667, 0.0);
                deriveds.cutplaneLookupTable->SetSaturationRange(1.0, 1.0);
                deriveds.cutplaneLookupTable->SetValueRange(1.0, 1.0);
                break;
        }
        deriveds.cutplaneLookupTable->Build();
    }
    
    // 更新ScalarBar（如果存在）
    if (deriveds.cutplaneScalarBar) {
        deriveds.cutplaneScalarBar->SetLookupTable(deriveds.cutplaneLookupTable);
    }
    
    std::cout << "[Debug] Cutplane color scheme updated successfully" << std::endl;
}

void vtkDisplayWindow::SetBoundaryColorScheme(int flowNumber, int presetIndex, bool reverse)
{
    std::cout << "[Debug] SetBoundaryColorScheme called: flow=" << flowNumber 
              << ", presetIndex=" << presetIndex << ", reverse=" << reverse << std::endl;
    
    if (flowNumber < 0 || flowNumber >= static_cast<int>(aesReader.GetFlows().size())) {
        std::cerr << "[Error] Invalid flow number: " << flowNumber << std::endl;
        return;
    }
    
    // 策略1: 尝试从flow的scalarBar获取LookupTable
    vtkLookupTable* lut = nullptr;
    auto flows = aesReader.GetFlows();
    
    if (flowNumber < static_cast<int>(flows.size()) && flows[flowNumber].scalarBar) {
        lut = vtkLookupTable::SafeDownCast(flows[flowNumber].scalarBar->GetLookupTable());
        if (lut) {
            std::cout << "[Debug] Found LookupTable from flow scalarBar" << std::endl;
        }
    }
    
    // 策略2: 如果scalarBar不存在，从第一个有效的boundary actor中获取
    if (!lut) {
        std::cout << "[Debug] ScalarBar LookupTable not found, searching in boundary actors..." << std::endl;
        for (auto& meshBoundaries : boundarys) {
            for (auto& boundary : meshBoundaries) {
                if (boundary.contourActor && boundary.contourActor->GetMapper()) {
                    lut = vtkLookupTable::SafeDownCast(
                        boundary.contourActor->GetMapper()->GetLookupTable());
                    if (lut) {
                        std::cout << "[Debug] Found LookupTable from contourActor" << std::endl;
                        break;
                    }
                }
                if (!lut && boundary.shadeActor && boundary.shadeActor->GetMapper()) {
                    lut = vtkLookupTable::SafeDownCast(
                        boundary.shadeActor->GetMapper()->GetLookupTable());
                    if (lut) {
                        std::cout << "[Debug] Found LookupTable from shadeActor" << std::endl;
                        break;
                    }
                }
            }
            if (lut) break;
        }
    }
    
    // 如果仍然没有找到，说明还没有显示任何boundary
    if (!lut) {
        std::cerr << "[Error] No LookupTable found. Please display a boundary first." << std::endl;
        return;
    }
    
    std::cout << "[Debug] Setting color scheme: presetIndex=" << presetIndex 
              << ", reverse=" << reverse << std::endl;
    
    // 使用 ColorMapPreset 应用颜色映射
    ColorMapPreset& preset = ColorMapPreset::instance();
    
    if (presetIndex >= 0 && preset.isLoaded()) {
        // 使用 JSON 预设中的颜色映射
        preset.applyToLookupTable(presetIndex, lut, reverse, 256);
        std::cout << "[Debug] Applied color map preset: " << preset.getName(presetIndex).toStdString() << std::endl;
    } else {
        // 回退到旧的硬编码颜色方案（当 presetIndex < 0 或预设未加载时）
        lut->SetNumberOfTableValues(256);
        
        int fallbackScheme = (presetIndex < 0) ? (-presetIndex - 1) : 0;  // -1 -> 0, -2 -> 1, etc.
        
        switch (fallbackScheme) {
            case 0: // Blue → Red
                if (reverse) {
                    lut->SetHueRange(0.0, 0.6667);
                } else {
                    lut->SetHueRange(0.6667, 0.0);
                }
                lut->SetSaturationRange(1.0, 1.0);
                lut->SetValueRange(1.0, 1.0);
                break;
                
            case 1: // Viridis
                if (reverse) {
                    lut->SetHueRange(0.167, 0.75);
                    lut->SetValueRange(0.95, 0.2);
                } else {
                    lut->SetHueRange(0.75, 0.167);
                    lut->SetValueRange(0.2, 0.95);
                }
                lut->SetSaturationRange(0.8, 0.9);
                break;
                
            case 2: // Rainbow
                if (reverse) {
                    lut->SetHueRange(0.0, 0.8333);
                } else {
                    lut->SetHueRange(0.8333, 0.0);
                }
                lut->SetSaturationRange(1.0, 1.0);
                lut->SetValueRange(1.0, 1.0);
                break;
                
            case 3: // Cool-Warm (Diverging)
                if (reverse) {
                    lut->SetHueRange(0.0, 0.6667);
                } else {
                    lut->SetHueRange(0.6667, 0.0);
                }
                lut->SetSaturationRange(0.7, 0.7);
                lut->SetValueRange(0.9, 0.9);
                break;
                
            default:
                lut->SetHueRange(0.6667, 0.0);
                lut->SetSaturationRange(1.0, 1.0);
                lut->SetValueRange(1.0, 1.0);
                break;
        }
        
        lut->Build();
        std::cout << "[Debug] Applied fallback color scheme: " << fallbackScheme << std::endl;
    }
    
    std::cout << "[Debug] LookupTable built successfully, updating actors..." << std::endl;
    
    // 更新所有使用该flow的boundary actors
    int actorCount = 0;
    for (size_t i = 0; i < boundarys.size(); i++) {
        for (size_t j = 0; j < boundarys[i].size(); j++) {
            auto& boundary = boundarys[i][j];
            
            if (boundary.contourActor && boundary.contourActor->GetMapper()) {
                boundary.contourActor->GetMapper()->SetLookupTable(lut);
                boundary.contourActor->GetMapper()->Update();
                actorCount++;
            }
            if (boundary.shadeActor && boundary.shadeActor->GetMapper()) {
                boundary.shadeActor->GetMapper()->SetLookupTable(lut);
                boundary.shadeActor->GetMapper()->Update();
                actorCount++;
            }
        }
    }
    
    std::cout << "[Debug] Updated " << actorCount << " actors" << std::endl;
    
    // 立即刷新渲染
    if (renderWindow) {
        std::cout << "[Debug] Rendering..." << std::endl;
        renderWindow->Render();
        std::cout << "[Debug] Render complete" << std::endl;
    } else {
        std::cerr << "[Warning] renderWindow is null, skipping render" << std::endl;
    }
    
    std::cout << "[Debug] Boundary color scheme updated successfully" << std::endl;
}

void vtkDisplayWindow::SetCutplaneVariable(int flowNumber)
{
    if (flowNumber < 0 || flowNumber >= static_cast<int>(aesReader.GetFlows().size())) {
        std::cerr << "[Error] Invalid flow number: " << flowNumber << std::endl;
        return;
    }
    
    std::cout << "[Debug] Setting cutplane variable to flow number: " << flowNumber 
              << " (" << aesReader.GetFlows()[flowNumber].name << ")" << std::endl;
    
    // 更新所有切片的标量数据
    for (auto& cutplaneActor : deriveds.cutplaneActors) {
        if (cutplaneActor) {
            // 设置活动标量数据
            cutplaneActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(
                aesReader.GetFlows()[flowNumber].name.c_str());
            
            // 更新标量范围
            cutplaneActor->GetMapper()->SetScalarRange(
                aesReader.GetFlows()[flowNumber].range[0],
                aesReader.GetFlows()[flowNumber].range[1]);
            
            // 设置查找表
            if (deriveds.cutplaneLookupTable) {
                deriveds.cutplaneLookupTable->SetRange(
                    aesReader.GetFlows()[flowNumber].range[0],
                    aesReader.GetFlows()[flowNumber].range[1]);
                cutplaneActor->GetMapper()->SetLookupTable(deriveds.cutplaneLookupTable);
            }
            
            cutplaneActor->GetMapper()->Update();
        }
    }
    
    // 更新标量条（如果存在）
    if (deriveds.cutplaneScalarBar && deriveds.cutplaneLookupTable) {
        deriveds.cutplaneScalarBar->SetLookupTable(deriveds.cutplaneLookupTable);
        deriveds.cutplaneScalarBar->SetTitle(aesReader.GetFlows()[flowNumber].name.c_str());
    }
    
    std::cout << "[Debug] Cutplane variable updated successfully" << std::endl;
}

void vtkDisplayWindow::DeleteCutplane(int cutplaneIndex)
{
    // 检查索引是否有效
    if (cutplaneIndex < 0 || cutplaneIndex >= deriveds.cutplanes.size()) {
        return;
    }
    
    // 从渲染器中移除actor
    renderer->RemoveActor(deriveds.cutplaneActors[cutplaneIndex]);
    
    // 从容器中删除对应的元素
    deriveds.cutplanes.erase(deriveds.cutplanes.begin() + cutplaneIndex);
    deriveds.cutters.erase(deriveds.cutters.begin() + cutplaneIndex);
    deriveds.cutplaneActors.erase(deriveds.cutplaneActors.begin() + cutplaneIndex);
}

void vtkDisplayWindow::SetSliceContourMode(const QString &mode)
{
    if (deriveds.cutplaneActors.empty()) {
        std::cout << "[Debug] No cutplanes to update" << std::endl;
        return;
    }
    
    vtkSmartPointer<vtkLookupTable> targetLookupTable;
    
    if (mode == "sync with main") {
        // 使用主模型的颜色映射和变量
        std::cout <<"[Debug] Sync CurFLow is :"<<curFlow<<std::endl;
        auto flows = aesReader.GetFlows();
        if (curFlow >= 0 && curFlow < flows.size()) {
            vtkScalarsToColors* scalarColors = flows[curFlow].scalarBar->GetLookupTable();
            targetLookupTable = vtkLookupTable::SafeDownCast(scalarColors);
            
            if (targetLookupTable) {
                std::cout << "[Debug] Switching slices to sync with main model (flow: " 
                          << flows[curFlow].name << ")" << std::endl;
                
                // 同步变量类型 - 更新所有切片的标量数据
                for (auto& cutplaneActor : deriveds.cutplaneActors) {
                    if (cutplaneActor) {
                        // 设置与主模型相同的标量变量
                        cutplaneActor->GetMapper()->GetInput()->GetPointData()->SetActiveScalars(
                            flows[curFlow].name.c_str());
                        std::cout << "[Debug] Synced cutplane variable to: " << flows[curFlow].name << std::endl;
                    }
                }
                
                // 隐藏切片的scalar bar（与主模型共享）
                HideCutplaneScalarBar();
            } else {
                std::cout << "[Error] Failed to cast LookupTable from main model" << std::endl;
                return;
            }
        } else {
            std::cout << "[Error] Invalid curFlow index: " << curFlow << std::endl;
            return;
        }
    } else if (mode == "isolated") {
        // 使用独立的颜色映射
        targetLookupTable = deriveds.cutplaneLookupTable;
        std::cout << "[Debug] Switching slices to isolated color mapping" << std::endl;
        
        // 显示切片的scalar bar（独立模式）
        ShowCutplaneScalarBar();
    } else {
        std::cout << "[Warning] Unknown contour mode: " << mode.toStdString() << std::endl;
        return;
    }
    
    // 更新所有cutplane的颜色映射
    for (int i = 0; i < deriveds.cutplaneActors.size(); i++) {
        vtkPolyDataMapper* mapper = vtkPolyDataMapper::SafeDownCast(
            deriveds.cutplaneActors[i]->GetMapper());
        if (mapper && targetLookupTable) {
            mapper->SetLookupTable(targetLookupTable);
            mapper->SetScalarRange(targetLookupTable->GetRange());
            std::cout << "[Debug] Updated cutplane " << i << " color mapping" << std::endl;
        }
    }
}