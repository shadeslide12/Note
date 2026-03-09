#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkScalarBarActor.h>

class vtkAesReader
{
public:
    struct Boundary
    {
        vtkSmartPointer<vtkUnstructuredGrid> dataset;
        std::string name;
        std::string zoneName;
        std::unordered_set<int> pointSet;
        int type;
    };

    struct Zone
    {
        vtkSmartPointer<vtkUnstructuredGrid> dataset;
        std::string name;
    };    

    struct FlowData
    {
        std::vector<double> datas;
        std::string name;
        double range[2];
        vtkSmartPointer<vtkScalarBarActor> scalarBar;
        double boundaryRange[2];
    };

public:
    vtkAesReader();
    ~vtkAesReader();

    void ReadGrid(std::string gridFileName);
    void UpdateFlow(std::string flowFileName);
    void CreateVector(int vx = 1, int vy = 2, int vz = 3);
    void ChangeScalarBar(double,double,int,int);
    void CreateNewVariable();
    void CalculateScaleFactor(int vx, int vy, int vz);

    std::vector<std::vector<Boundary>> GetBoundarys(){return boundarys;}
    std::vector<Zone> GetZoneGrids(){return zoneGrids;}
    vtkSmartPointer<vtkUnstructuredGrid> GetTotalGrid(){return totalGrid;}
    std::vector<FlowData> GetFlows(){return flows;}
    double GetScaleFactor(){return scaleFactor;}
    int *GetVectorCompontesNumber(){return vectorComponentsNumber;}
    std::vector<double> angles;
    std::vector<double> node_radius;
private:
    std::vector<std::vector<Boundary>> boundarys;
    std::vector<Zone> zoneGrids;
    vtkSmartPointer<vtkUnstructuredGrid> totalGrid; 
    std::vector<FlowData> flows;
    std::vector<std::string> flowNames;
    int vectorComponentsNumber[3];
    double scaleFactor;
};