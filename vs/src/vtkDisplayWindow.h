#pragma once

#include <QObject>
#include <map>
#include <vtkBox.h>
#include <vtkClipPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarWidget.h>
#include <vtkOutlineFilter.h>
#include <vtkContourFilter.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkStreamTracer.h>
#include <vtkMaskPoints.h>
#include <vtkLineSource.h>
#include <vtkGlyph3D.h>
#include <vtkPointInterpolator.h>

#include "vtkAesReader.h"
#include <vtkPlaneSource.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>

class vtkDisplayWindow : public QObject
{
Q_OBJECT

public:
    struct BasicObject
    {
    public:
        vtkSmartPointer<vtkActor> meshActor;
        vtkSmartPointer<vtkActor> shadeActor;
        vtkSmartPointer<vtkActor> contourActor;
        vtkSmartPointer<vtkActor> edgeActor;
        vtkSmartPointer<vtkActor> velocityActor;
        vtkSmartPointer<vtkGlyph3D> glyph3D = nullptr;
    };

    struct AuxiliaryObject
    {
    public:
        vtkSmartPointer<vtkOrientationMarkerWidget> axisWidget;
        vtkSmartPointer<vtkScalarBarWidget> scalarBarWidget;
        std::vector<vtkSmartPointer<vtkActor>> outlineActors;
    };

    struct DerivedObject
    {
    public:
        vtkSmartPointer<vtkActor> isoSurfaceActor;
        vtkSmartPointer<vtkContourFilter> contourFilter;

        std::vector<vtkSmartPointer<vtkActor>> cutplaneActors;
        std::vector<vtkSmartPointer<vtkPlane>> cutplanes;
        std::vector<vtkSmartPointer<vtkCutter>> cutters;
        
        //* 所有cutplane共用的独立云图映射
        vtkSmartPointer<vtkLookupTable> cutplaneLookupTable;
        vtkSmartPointer<vtkScalarBarActor> cutplaneScalarBar;
        
        //* cutplane云图参数
        struct CutplaneColorMapping {
            double minValue;
            double maxValue;
            int numberOfColors;
            bool useCustomRange;
            
            CutplaneColorMapping() : minValue(0.0), maxValue(1.0), numberOfColors(256), useCustomRange(false) {}
        } cutplaneColorMapping;
    };

public:

    vtkDisplayWindow(QObject *parent = nullptr);
    ~vtkDisplayWindow();

    void ReadAesFile(std::string aesFileName);
    void UpdateFlow(std::string flowFileName);
    
    void VisiableBoundaryActor(int zoneNumber, int bNumber);
    void InVisiableBoundaryActor(int zoneNumber, int bNumber);
    void AddMeshActor();
    void RemoveMeshActor();
    void AddContourActor();
    void RemoveContourActor();
    void AddShadeActor();
    void RemoveShadeActor();
    void AddEdgeActor();
    void RemoveEdgeActor();
    void CreateVelocityActor(int vx = 1, int vy = 2, int vz = 3);
    void AddVelocityActor();
    void RemoveVelocityActor();

    void RemoveMeridianActor();
    void RemoveConstHeight();

    void ActivateAxiesWidget();
    void InActivateAxiesWidget();
    void ActivateScalarBarWidget();
    void InActivateScalarBarWidget();
    void SetScalarBar(double,double,int,int);
    void SetScalarBarSize(double width, double height);
    vtkSmartPointer<vtkScalarBarWidget> GetScalarBarWidget() { return auxiliarys.scalarBarWidget; }
    vtkSmartPointer<vtkScalarBarActor> GetCutplaneScalarBar() { return deriveds.cutplaneScalarBar; }
    
    // 新增ScalarBar控制方法
    void SetScalarBarVisibility(bool visible);
    void SetScalarBarOrientation(bool isVertical);
    void SetScalarBarPosition(double x, double y);
    void SetScalarBarTitle(const std::string& title);
    void SetScalarBarTextColor(double r, double g, double b);
    void SetScalarBarFont(const std::string& family, int size, bool bold, bool italic);
    
    void VisiableOutlineActor();
    void InVisiableOutlineActor();
    void SetVectorScaleFactor(double);

    void xoyViewRender();
    void xozViewRender();
    void yozViewRender();
    void xyzViewRender();

    void CreateIsoSurfaceActor();
    void AddIsoSurfaceActor();
    void RemoveIsoSurfaceActor();
    void ChangeFloodNumber(int);
    void IsoSurfaceChangeFlowNumber(int);
    void SetIsoSurfaceValue(double);

    void AddNewCutplane();
    void AddNewCutplane(double* origin, double* normal);
    void SetCutplane(int cutplaneNumber, double *origin, double *normal);
    void SetCutplaneColorMapping(double minValue, double maxValue, int numberOfColors,bool isBanded);
    void UpdateCutplaneColorMapping();
    DerivedObject::CutplaneColorMapping GetCutplaneColorMapping();
    void SetCutplaneColorScheme(int presetIndex, bool reverse = false);  // 设置切片颜色方案
    void SetCutplaneVariable(int flowNumber);      // 设置切片显示的变量
    void AddCutplaneActors();
    void RemoveCutplaneActors();
    void AddCutplane(int number);
    void RemoveCutplane(int number);
    void DeleteCutplane(int cutplaneIndex);  
    void SetSliceContourMode(const QString &mode); // 设置slice颜色映射模式

    void SetActorTransparancy(double opacity);
    void SetBoundaryTransparency(int meshNumber, int boundaryNumber, double opacity);
    void SetSliceTransparency(int sliceNumber, double opacity);
    void SetActorLighting(bool);
    
    // 颜色映射控制
    void SetBoundaryColorScheme(int flowNumber, int schemeIndex, bool reverse);
    void SetBackgroundStyle(const QString &style);  // 设置背景样式

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> GetRenderWindow(){return renderWindow;}
    std::vector<vtkAesReader::FlowData> GetFlows(){return aesReader.GetFlows();}
    std::vector<std::vector<vtkAesReader::Boundary>> GetBoundaryDatasets(){return aesReader.GetBoundarys();}
    std::vector<std::string> GetZoneNames();
    std::vector<double> GetPeriodicAngles(){return aesReader.angles;}
    void CreatePeriodicCopies(int zoneIndex, int numCopies);
    void CreatePeriodicCopies(int zoneIndex, int numCopies, const std::vector<int> &boundaryIndices);
    void ClearPeriodicCopies();
    void ClearPeriodicCopiesForZone(int zoneIndex);
    DerivedObject GetDeriveds() {return deriveds;}
    std::vector<vtkSmartPointer<vtkPlane>> GetPlanes() {return deriveds.cutplanes;}
    int GetCurFlowNumber(){return curFlow;}
    double GetScaleFactor(){return boundarys[0][0].glyph3D->GetScaleFactor();}
    int *GetVectorComponentsNumber(){return aesReader.GetVectorCompontesNumber();}
    int GetIsoSurfaceFloodNumber(){return isoSurfaceFloodNumber;}
    int GetIsoSurfaceFlowNumber(){return isoSurfaceFlowNumber;}
    bool HasGrid(){return hasGrid;};
    bool HasNodeRadius(){return !aesReader.node_radius.empty();};
    bool HasVector(){return hasVector;}
    bool HasIsoSurface(){return hasIsoSurface;}
    bool HasCutplane(){return deriveds.cutplanes.size() != 0;}
    bool IsAxiesWidgetActive(){return auxiliarys.axisWidget->GetEnabled();}
    bool IsOutlineActorVisiable(){return auxiliarys.outlineActors[0]->GetVisibility();}
    bool IsScalarBarWidgetActive(){return auxiliarys.scalarBarWidget->GetEnabled();}

    double* GetModelBounds();

    std::vector<vtkSmartPointer<vtkActor>> CreateMeridionalPlane(double minRange, double maxRange);
    void CreateConstHeight(double height);
    std::vector<vtkSmartPointer<vtkActor>> ChangeMeridionalFlow(double minRange, double maxRange, int flowNumber);
    void ChangeConstHeightFlow(int flowNumber);
    
    // Blade-to-blade functions (similar to ConstHeight)
    std::vector<vtkSmartPointer<vtkActor>> CreateBladeToBladePlane(double span);
    std::vector<vtkSmartPointer<vtkActor>> ChangeBladeToBladePlaneFlow(int flowNumber);
    
    std::vector<vtkSmartPointer<vtkActor>> MeridionalPlaneActor;
    std::vector<vtkSmartPointer<vtkActor>> ConstHeightPlaneActor;
    std::vector<vtkSmartPointer<vtkPolyData>> MeridionalPlane;
    std::vector<vtkSmartPointer<vtkContourFilter>> ConstHeightPlane;
    std::vector<vtkSmartPointer<vtkActor>> BladeToBladePlaneActor;
    std::vector<vtkSmartPointer<vtkContourFilter>> BladeToBladePlane;
private:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    vtkAesReader aesReader;
    bool hasGrid = false;
    bool hasVector = false;
    bool hasIsoSurface = false;
    int curFlow = 0;
    int isoSurfaceFloodNumber = 0;
    int isoSurfaceFlowNumber = 0;
    std::vector<std::vector<BasicObject>> boundarys;
    AuxiliaryObject auxiliarys;
    DerivedObject deriveds;
    
    //* Periodic copies storage - organized by zone and actor type
    std::map<int, std::vector<vtkSmartPointer<vtkActor>>> periodicCopyShadeActorsByZone;
    std::map<int, std::vector<vtkSmartPointer<vtkActor>>> periodicCopyContourActorsByZone;

    void VisualizeMeridonalPlane();
    void SetBackground();
    void CreateBasicObjects();
    void CreateAuxiliaryObject();
    void CreateAxisWidget();
    void CreateScalarBarWidget();
    void CreateOutlineActor();
    void setBackground();
    
public:
    void CreatePlanePreview(double value,int currenAxis);
    void HidePlanePreview();
    vtkSmartPointer<vtkPlaneSource> xPlaneSource;
    vtkSmartPointer<vtkActor> xPlaneActor;
    vtkSmartPointer<vtkPolyDataMapper> xPlaneMapper;

    //* add Control Cut Bar
    void ShowCutplaneScalarBar();
    void HideCutplaneScalarBar();
    void InitializeCutplaneScalarBar();
};
