#include "vtkAesReader.h"

#include "Utilities/H5/H5.h"

using std::string;
using std::cout; using std::endl;
using std::vector;
using std::unordered_map;
using std::min; using std::max; using std::abs;

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkQuad.h>
#include <vtkHexahedron.h>
#include <vtkPyramid.h>
#include <vtkTetra.h>
#include <vtkWedge.h>
#include <vtkLookupTable.h>
#include <vtkTextProperty.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkDataSet.h>
#include <vtkMergeVectorComponents.h>
#include <vtkDataSetMapper.h>
#include <vtkColorSeries.h>

#include "ViridisColor.h"

#include <vtkArrayCalculator.h>

extern vector<vector<double>> viridisColor;

vtkAesReader::vtkAesReader(){}

vtkAesReader::~vtkAesReader(){}

void vtkAesReader::ReadGrid(string gridFileName)
{
    Utilities::H5::PropertyList plist;
    Utilities::H5::File gridFile;
    plist.SetCloseDegree(H5F_CLOSE_STRONG);
#ifdef HAVE_MPI
    plist.SetDriver_mpio(MPI_COMM_WORLD, MPI_INFO_NULL);
#endif
    gridFile.Open(gridFileName, Utilities::H5::FileAccessFlag::READONLY, plist);

    Utilities::ndarray<double> pointArray;
    Utilities::ndarray<char> bndNameArray;
    Utilities::ndarray<int> bndZoneArray;
    Utilities::ndarray<int> quadNodeArray;
    Utilities::ndarray<int> quadGroupArray;
    Utilities::ndarray<int> hexNodeArray;
    Utilities::ndarray<int> prmNodeArray;
    Utilities::ndarray<int> pyrNodeArray;
    Utilities::ndarray<int> tetNodeArray;
    Utilities::ndarray<int> bndTypeArray;
    gridFile.GetDataset("/group_type")->Read(bndTypeArray);
    gridFile.GetDataset("/node_coordinates")->Read(pointArray);
    gridFile.GetDataset("/group_name")->Read(bndNameArray);
    gridFile.GetDataset("/group_zone")->Read(bndZoneArray);
    gridFile.GetDataset("/bnd_quad-->node")->Read(quadNodeArray);
    gridFile.GetDataset("/bnd_quad-->group")->Read(quadGroupArray);

    //* periodic_angle absent for non-annular cascades (e.g. PVD). Do NOT treat it as mandatory.
    angles.clear();
    if (gridFile.Exists("/periodic_angle"))
    {
        Utilities::ndarray<double> period_angle;
        gridFile.GetDataset("/periodic_angle")->Read(period_angle);
        for (int i = 0; i < period_angle.GetLength(); i++)
        {
            angles.push_back(period_angle(i) * 360.0 / (2.0 * 3.14159265358979323846));
        }
    }

    // Optional node_radius for span-based / blade-to-blade operations.
    if(gridFile.Exists("/node_radius"))
    {
        Utilities::ndarray<double> radius;
        gridFile.GetDataset("/node_radius")->Read(radius);
        for(int i = 0; i < radius.GetLength(); i++)
        {
            node_radius.push_back(radius(i));
        }
    }
    struct node
    {
        double coordinates[3];
    };
    vector<node> nodeCoordinate(pointArray.GetDim(1));
    for (int i = 0; i < pointArray.GetDim(1); i++)
    {
        nodeCoordinate[i].coordinates[0] = pointArray(i*3);
        nodeCoordinate[i].coordinates[1] = pointArray(i*3+1);
        nodeCoordinate[i].coordinates[2] = pointArray(i*3+2);
    }

    // Process Boundarys' Dataset
    {
    int nBoundarys = bndNameArray.GetDim(1);
    vector<Boundary> tempBoundarys(nBoundarys);

    // Process Boundary Name
    for (int i = 0; i < nBoundarys; i++)
    {
        string zoneName;
        string bndName;
        bool flag = false;
        for (int j = 0; j < bndNameArray.GetDim(0); j++)
        {
            char x = bndNameArray(i* bndNameArray.GetDim(0) + j);
            if (!flag)
            {
                if (x != ':') zoneName += x;
                else flag = true;
            }
            else
            {
                if (x == ' ') break;
                bndName += x;
            }
        }
        if (bndName.find(':') != std::string::npos) bndName = bndName.substr(bndName.find(':')+1);
      tempBoundarys[i].name = bndName;
      tempBoundarys[i].zoneName = zoneName;
      tempBoundarys[i].type = bndTypeArray(i);
    }
    // End Process Boundary Name

    // Process Boundary's PointSet And Cells.
    for (int i = 0; i < quadNodeArray.GetDim(1); i++)
    {
        tempBoundarys[quadGroupArray(i)-1].pointSet.insert(quadNodeArray(i*4));
        tempBoundarys[quadGroupArray(i)-1].pointSet.insert(quadNodeArray(i*4 + 1));
        tempBoundarys[quadGroupArray(i)-1].pointSet.insert(quadNodeArray(i*4 + 2));
        tempBoundarys[quadGroupArray(i)-1].pointSet.insert(quadNodeArray(i*4 + 3));
    }
    vector<vtkSmartPointer<vtkPoints>> points(nBoundarys);
    for (int i = 0; i < nBoundarys; i++)
    {
        points[i] = vtkSmartPointer<vtkPoints>::New();
        for (auto &y : tempBoundarys[i].pointSet)
        {
            points[i]->InsertNextPoint(nodeCoordinate[y-1].coordinates);
        }
    }
    vector<unordered_map<int,int>> hass(nBoundarys);
    for (int i = 0; i < nBoundarys; i++)
    {
        int number = 0;
        for (auto &x : tempBoundarys[i].pointSet)
        {
            hass[i][x] = number;
            number++;
        }
    }
    vector<vtkSmartPointer<vtkCellArray>> quads(nBoundarys);
    for (auto &x : quads)
    {
        x = vtkSmartPointer<vtkCellArray>::New();
    }
    vtkSmartPointer<vtkQuad> quad = vtkSmartPointer<vtkQuad>::New();
    for (int i = 0; i < quadNodeArray.GetDim(1); i++)
    {
        int group = quadGroupArray(i) - 1;
        quad->GetPointIds()->SetId(0, hass[group][quadNodeArray(i*4)]);
        quad->GetPointIds()->SetId(1, hass[group][quadNodeArray(i*4 + 1)]);
        quad->GetPointIds()->SetId(2, hass[group][quadNodeArray(i*4 + 2)]);
        quad->GetPointIds()->SetId(3, hass[group][quadNodeArray(i*4 + 3)]);
        quads[group]->InsertNextCell(quad);
    }
    for (int i = 0; i < nBoundarys; i++)
    {
        tempBoundarys[i].dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
        tempBoundarys[i].dataset->SetPoints(points[i]);
        tempBoundarys[i].dataset->SetCells(VTK_QUAD, quads[i]);
    }

    int nZone = *(bndZoneArray.end()-1);
    boundarys.resize(nZone);
    for (int i = 0; i < bndZoneArray.GetDim(1); i++)
    {
        boundarys[bndZoneArray(i)-1].emplace_back(tempBoundarys[i]);
    }
    // End Process Boundary's PointSet And Cells.
    }
    //End Process Boundarys' Dataset

    //Process Totalgrid's Dataset
    {
        totalGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkCellArray> quads = vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkCellArray> hexs = vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkCellArray> prms = vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkCellArray> pyrs = vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkCellArray> tets = vtkSmartPointer<vtkCellArray>::New();
        
        for (int i = 0; i < pointArray.GetDim(1); i++)
        {
            points->InsertNextPoint(nodeCoordinate[i].coordinates);
        }
        totalGrid->SetPoints(points);

        vtkSmartPointer<vtkQuad> quad = vtkSmartPointer<vtkQuad>::New();
        for (int i = 0; i < quadNodeArray.GetDim(1); i++)
        {
            quad->GetPointIds()->SetId(0, quadNodeArray(i*4)-1);
            quad->GetPointIds()->SetId(1, quadNodeArray(i*4+1)-1);
            quad->GetPointIds()->SetId(2, quadNodeArray(i*4+2)-1);
            quad->GetPointIds()->SetId(3, quadNodeArray(i*4+3)-1);
            quads->InsertNextCell(quad);
        }
        totalGrid->SetCells(VTK_QUAD, quads);

        if (gridFile.Exists("hex-->node"))
        {
            gridFile.GetDataset("/hex-->node")->Read(hexNodeArray);
            vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();
            for (int i = 0; i < hexNodeArray.GetDim(1); i++)
            {
                hex->GetPointIds()->SetId(0, hexNodeArray(i*8)-1);
                hex->GetPointIds()->SetId(1, hexNodeArray(i*8+1)-1);
                hex->GetPointIds()->SetId(2, hexNodeArray(i*8+2)-1);
                hex->GetPointIds()->SetId(3, hexNodeArray(i*8+3)-1);
                hex->GetPointIds()->SetId(4, hexNodeArray(i*8+4)-1);
                hex->GetPointIds()->SetId(5, hexNodeArray(i*8+5)-1);
                hex->GetPointIds()->SetId(6, hexNodeArray(i*8+6)-1);
                hex->GetPointIds()->SetId(7, hexNodeArray(i*8+7)-1);
                hexs->InsertNextCell(hex);
            }
            totalGrid->SetCells(VTK_HEXAHEDRON, hexs);
        }

        if (gridFile.Exists("tet-->node"))
        {
            gridFile.GetDataset("/tet-->node")->Read(tetNodeArray);
            vtkSmartPointer<vtkTetra> tet = vtkSmartPointer<vtkTetra>::New();
            for (int i = 0; i < tetNodeArray.GetDim(1); i++)
            {
                tet->GetPointIds()->SetId(0, tetNodeArray(i*4)-1);
                tet->GetPointIds()->SetId(1, tetNodeArray(i*4+1)-1);
                tet->GetPointIds()->SetId(2, tetNodeArray(i*4+2)-1);
                tet->GetPointIds()->SetId(3, tetNodeArray(i*4+3)-1);
                tets->InsertNextCell(tet);
            }
            totalGrid->SetCells(VTK_TETRA, tets);
        }

        if (gridFile.Exists("pyr-->node"))
        {
            gridFile.GetDataset("/pyr-->node")->Read(pyrNodeArray);
            vtkSmartPointer<vtkPyramid> pyr = vtkSmartPointer<vtkPyramid>::New();
            for (int i = 0; i < pyrNodeArray.GetDim(1); i++)
            {
                pyr->GetPointIds()->SetId(0, pyrNodeArray(i*5)-1);
                pyr->GetPointIds()->SetId(1, pyrNodeArray(i*5+1)-1);
                pyr->GetPointIds()->SetId(2, pyrNodeArray(i*5+2)-1);
                pyr->GetPointIds()->SetId(3, pyrNodeArray(i*5+3)-1);
                pyr->GetPointIds()->SetId(4, pyrNodeArray(i*5+4)-1);
                pyrs->InsertNextCell(pyr);
            }
            totalGrid->SetCells(VTK_PYRAMID, pyrs);
        }        

        if (gridFile.Exists("prm-->node"))
        {
            gridFile.GetDataset("/prm-->node")->Read(prmNodeArray);
            vtkSmartPointer<vtkWedge> prm = vtkSmartPointer<vtkWedge>::New();
            for (int i = 0; i < prmNodeArray.GetDim(1); i++)
            {
                prm->GetPointIds()->SetId(0, prmNodeArray(i*6)-1);
                prm->GetPointIds()->SetId(1, prmNodeArray(i*6+1)-1);
                prm->GetPointIds()->SetId(2, prmNodeArray(i*6+2)-1);
                prm->GetPointIds()->SetId(3, prmNodeArray(i*6+3)-1);
                prm->GetPointIds()->SetId(4, prmNodeArray(i*6+4)-1);
                prm->GetPointIds()->SetId(5, prmNodeArray(i*6+5)-1);
                prms->InsertNextCell(prm);
            }
            totalGrid->SetCells(VTK_WEDGE, prms);
        }    
    }
    //End Process Totalgrid's Dataset

    //Process ZoneGrids' Dataset
    {
        int nZone = *(bndZoneArray.end()-1);
        zoneGrids.resize(nZone);
        vector<vtkSmartPointer<vtkPoints>> points(nZone);
        for (int i = 0; i < nZone; i++)
        {
            points[i] = vtkSmartPointer<vtkPoints>::New();
            for (auto &x : boundarys[i])
            {
                for (auto &y : x.pointSet)
                {
                    points[i]->InsertNextPoint(nodeCoordinate[y-1].coordinates);
                }
            }
            zoneGrids[i].dataset = vtkSmartPointer<vtkUnstructuredGrid>::New();
            zoneGrids[i].dataset->SetPoints(points[i]);
        }
    }

}

void vtkAesReader::UpdateFlow(string flowFileName)
{
    Utilities::H5::PropertyList plist;
    Utilities::H5::File flowFile;
    plist.SetCloseDegree(H5F_CLOSE_STRONG);
#ifdef HAVE_MPI
    plist.SetDriver_mpio(MPI_COMM_WORLD, MPI_INFO_NULL);
#endif
    flowFile.Open(flowFileName.c_str(), Utilities::H5::FileAccessFlag::READONLY, plist);
    Utilities::ndarray<double> flowArray;
    flowFile.GetDataset("flow")->Read(flowArray);

  double rho_ref = 1.226;
  double p_ref = 1.013e5;
  double uvw_ref = std::sqrt(p_ref / rho_ref);

    for (uint32_t i = 0; i < flowArray.GetDim(1); i++) {
      flowArray({0, i}) = flowArray({0, i }) * rho_ref;
      flowArray({1, i}) = flowArray({1, i }) * uvw_ref;
      flowArray({2, i}) = flowArray({2, i }) * uvw_ref;
      flowArray({3, i}) = flowArray({3, i }) * uvw_ref;
      flowArray({4, i}) = flowArray({4, i }) * p_ref;
    }

    if (flowArray.GetDim(1) != totalGrid->GetNumberOfPoints())
    {
        cout << "match error, please find correct flow file" << endl;
        return;
    }

    flows.clear();
    flowNames = {"rho", "u", "v", "w", "P"};

    for (int i = 0; i < flowArray.GetDim(0); i++)
    {
        if (i >= 5)
        {
            string name = "tb" + std::to_string(i-4);
            flowNames.emplace_back(name);
        }
        FlowData flow;
        flow.datas = vector<double> (flowArray.GetDim(1), 0);
        flow.name = flowNames[i];
        flow.range[0] = flowArray(0);
        flow.range[1] = flowArray(0);
        for (int j = 0; j < flowArray.GetDim(1); j++)
        {
            flow.datas[j] = flowArray(j*flowArray.GetDim(0)+i);
            flow.range[0] = min(flow.range[0], flow.datas[j]);
            flow.range[1] = max(flow.range[1], flow.datas[j]);
        }

        // vtkSmartPointer<vtkColorSeries> colorSeries = vtkSmartPointer<vtkColorSeries>::New();
        // colorSeries->SetColorScheme(0);

        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
        // colorSeries->BuildLookupTable(lut, vtkColorSeries::ORDINAL);
        lut->SetNumberOfColors(10);		// 指定颜色查找表中有多少种颜色
        lut->SetHueRange(0.6667, 0.0);	//蓝到红渐变
        // lut->SetHueRange(0.7777, 0.16667);	//紫到黄渐变
        // for (int i = 0; i < 10; i++)
        // {
        //     int index = i * (255 / (10 - 1));
        //     lut->SetTableValue(i, viridisColor[index][0], viridisColor[index][1], viridisColor[index][2]);
        // }
        lut->SetRange(100000,140000);
        lut->Build();
        flow.scalarBar = vtkSmartPointer< vtkScalarBarActor >::New();
        flow.scalarBar->SetTitle((flowNames[i]).c_str());
        flow.scalarBar->GetTitleTextProperty()->SetJustificationToLeft();
	    flow.scalarBar->GetTitleTextProperty()->SetFontFamilyToArial();
        flow.scalarBar->GetTitleTextProperty()->ItalicOff();
        flow.scalarBar->GetTitleTextProperty()->BoldOn();
        flow.scalarBar->GetTitleTextProperty()->ShadowOff();
        flow.scalarBar->GetTitleTextProperty()->SetColor(0, 0, 0);
        flow.scalarBar->GetTitleTextProperty()->SetFontSize(25);
        flow.scalarBar->GetLabelTextProperty()->ItalicOff();
        flow.scalarBar->GetLabelTextProperty()->BoldOn();
        flow.scalarBar->GetLabelTextProperty()->ShadowOff();
        flow.scalarBar->GetLabelTextProperty()->SetColor(0,0,0);
        flow.scalarBar->GetLabelTextProperty()->SetFontSize(16);

        flow.scalarBar->SetNumberOfLabels(10);
        flow.scalarBar->SetLookupTable(lut);

        flow.scalarBar->SetWidth(0.1);
        flow.scalarBar->SetHeight(0.8);
        flow.scalarBar->UnconstrainedFontSizeOn();
        flow.scalarBar->SetTextPad(4);
        flow.scalarBar->SetMaximumHeightInPixels(1000);
        flow.scalarBar->SetMaximumWidthInPixels(150);

        flows.emplace_back(flow);
    }

    for (int i = 0; i < boundarys.size(); i++)
    {
        for (int j = 0; j < boundarys[i].size(); j++)
        {
            for (int k = 0; k < flows.size(); k++)
            {
                // flows[k].boundaryRange[0] = abs(flows[k].datas[*boundarys[i][j].pointSet.begin()-1]);
                // flows[k].boundaryRange[1] = abs(flows[k].datas[*boundarys[i][j].pointSet.begin()-1]);
                vtkSmartPointer<vtkDoubleArray> boundaryFlow = vtkSmartPointer<vtkDoubleArray>::New();
                boundaryFlow->SetName(flows[k].name.c_str());
                for (auto &x : boundarys[i][j].pointSet)
                {
                    boundaryFlow->InsertNextValue(flows[k].datas[x-1]);
                    // flows[k].boundaryRange[0] = min(flows[k].boundaryRange[0], abs(flows[k].datas[x-1]));
                    // flows[k].boundaryRange[1] = max(flows[k].boundaryRange[1], abs(flows[k].datas[x-1]));
                }
                boundarys[i][j].dataset->GetPointData()->AddArray(boundaryFlow);
            }
            boundarys[i][j].dataset->GetPointData()->SetActiveScalars(flows.front().name.c_str());
        }
    }

    for (int i = 0; i < flows.size(); i++)
    {
        vtkSmartPointer<vtkDoubleArray> gridFlow = vtkSmartPointer<vtkDoubleArray>::New();
        gridFlow->SetName(flows[i].name.c_str());
        for (int j = 0; j < flows[i].datas.size(); j++)
        {
            gridFlow->InsertNextValue(flows[i].datas[j]);
        }
        totalGrid->GetPointData()->AddArray(gridFlow);
    }

    //dyg
    if(!node_radius.empty())
    {
        vtkSmartPointer<vtkDoubleArray> gridFlow = vtkSmartPointer<vtkDoubleArray>::New();
        gridFlow->SetName("radius");
        for(int i = 0; i < node_radius.size(); i++)
        {
            gridFlow->InsertNextValue(node_radius[i]);
        }
        totalGrid->GetPointData()->AddArray(gridFlow);
    }



    totalGrid->GetPointData()->SetActiveScalars(flows.front().name.c_str());
}

void vtkAesReader::CreateVector(int vx, int vy, int vz)
{
    for (int i = 0; i < boundarys.size(); i++)
    {
        for (int j = 0; j < boundarys[i].size(); j++)
        {
            vtkSmartPointer<vtkDoubleArray> boundaryVelocity = vtkSmartPointer<vtkDoubleArray>::New();
            boundaryVelocity->SetNumberOfComponents(3);
            boundaryVelocity->SetNumberOfTuples(boundarys[i][j].pointSet.size());
            boundaryVelocity->SetName("velocity_uvw");
            int k = 0;
            for (auto &x : boundarys[i][j].pointSet)
            {
                boundaryVelocity->SetTuple3(k,flows[vx].datas[x-1],
                flows[vy].datas[x-1],flows[vz].datas[x-1]);
                k++;
            }
            boundarys[i][j].dataset->GetPointData()->AddArray(boundaryVelocity);
            boundarys[i][j].dataset->GetPointData()->SetVectors(boundaryVelocity);

            // auto vectorMerge = vtkSmartPointer<vtkMergeVectorComponents>::New();
            // vectorMerge->SetInputData(boundarys[i][j].dataset);
            // vectorMerge->SetXArrayName("u");
            // vectorMerge->SetYArrayName("v");
            // vectorMerge->SetZArrayName("w");
            // vectorMerge->SetOutputVectorName("velocity_uvw");
            // vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
            // vtkDataSet* ds = static_cast<vtkDataSet*>(vectorMerge->GetOutput());
            // auto array = ds->GetPointData()->GetArray("velocity_uvw");
            // boundarys[i][j].dataset->GetPointData()->AddArray(array);
            // boundarys[i][j].dataset->GetPointData()->SetVectors(array);
        }
    }

    vtkSmartPointer<vtkDoubleArray> totalGridVelocity = vtkSmartPointer<vtkDoubleArray>::New();
    totalGridVelocity->SetNumberOfComponents(3);
    totalGridVelocity->SetNumberOfTuples(totalGrid->GetNumberOfPoints());
    totalGridVelocity->SetName("velocity_uvw");
    for (int i = 0; i < totalGrid->GetNumberOfPoints(); i++)
    {
        totalGridVelocity->SetTuple3(i, flows[vx].datas[i], flows[vy].datas[i], flows[vz].datas[i]);
    }
    totalGrid->GetPointData()->AddArray(totalGridVelocity);
    totalGrid->GetPointData()->SetVectors(totalGridVelocity);
    cout << "succesfully add vector. xComponent is " << flows[vx].name << ", yComponent is " << flows[vy].name
    << ", zComponent is " << flows[vz].name  << endl;

    vectorComponentsNumber[0] = vx; vectorComponentsNumber[1] = vy; vectorComponentsNumber[2] = vz;
    
    CalculateScaleFactor(vx,vy,vz);
}

void vtkAesReader::ChangeScalarBar(double m, double M, int number, int flowNumber)
{
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfColors(number);		// 指定颜色查找表中有多少种颜色
    lut->SetHueRange(0.6667, 0.0);	//蓝到红渐变
    // lut->SetHueRange(0.7777, 0.16667);	//紫到黄渐变
    // for (int i = 0; i < number; i++)
    // {
    //     int index = i * (255 / (number - 1));
    //     lut->SetTableValue(i, viridisColor[index][0], viridisColor[index][1], viridisColor[index][2]);
    // }
    lut->SetRange(m,M);
    lut->Build();
    
    // 限制标签数量，避免标签重叠。最多显示12个标签
    int numberOfLabels = std::min(number, 12);
    flows[flowNumber].scalarBar->SetNumberOfLabels(numberOfLabels);
    flows[flowNumber].scalarBar->SetLookupTable(lut);
}

void vtkAesReader::CalculateScaleFactor(int vx, int vy, int vz)
{
    double m = min(flows[vx].range[0], (flows[vy].range[0], flows[vz].range[0]));
    double M = max(flows[vx].range[1], (flows[vy].range[1], flows[vz].range[1]));
    M = max(abs(m), abs(M));
    auto cell = totalGrid->GetCell(0);
    auto point0 = totalGrid->GetPoint(cell->GetPointId(0));
    double mx = point0[0]; double my = point0[1]; double mz = point0[2];
    double Mx = point0[0]; double My = point0[1]; double Mz = point0[2];
    for (int i = 1; i < cell->GetNumberOfPoints(); i++)
    {
        auto point = totalGrid->GetPoint(cell->GetPointId(i));
        mx = min(mx, point[0]);
        my = min(my, point[1]);
        mz = min(mz, point[2]);
        Mx = max(Mx, point[0]);
        My = max(My, point[1]);
        Mz = max(Mz, point[2]);
    }
    double MCoorDiff = max(Mx - mx, max(My - my, Mz - mz));
    if (M == 0) scaleFactor = 1;
    // else scaleFactor = sqrt(MCoorDiff) / M;
    else scaleFactor = 10 * MCoorDiff / M;
}

void vtkAesReader::CreateNewVariable()
{
    // vtkSmartPointer<vtkArrayCalculator> calc = vtkSmartPointer<vtkArrayCalculator>::New();
    // calc->SetInputData(totalGrid);
    // for (auto &x : flows)
    // {
    //     calc->AddVectorArrayName("velocity_uvw");
    // }
    // calc->SetFunction("mag(velocity_uvw)");
    // calc->SetResultArrayName("mvelocity");
    // calc->Update();

    // auto data = vtkDataSet::SafeDownCast(calc->GetOutput());
    // auto array = data->GetPointData()->GetArray("mvelocity");
    // cout << array->GetRange()[0] << " " << array->GetRange()[1] << endl;
    // cout << flows[1].range[1] << " " << flows[2].range[1] << " " << flows[3].range[1] << endl;
}
