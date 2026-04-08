整体渲染流程
1. 数据读取阶段 — ReadGrid() + UpdateFlow()
网格读取 (vtkAesReader.cpp:38)，从 HDF5 文件读取三类数据结构：

① Boundary dataset — 每个 boundary 是一组 quad 面片（2D 表面）


// vtkAesReader.cpp:132-178
// 每个 boundary 只包含属于它的 quad 面
for (int i = 0; i < quadNodeArray.GetDim(1); i++) {
    int group = quadGroupArray(i) - 1;
    quad->GetPointIds()->SetId(0, hass[group][quadNodeArray(i*4)]);
    // ...
    quads[group]->InsertNextCell(quad);
}
tempBoundarys[i].dataset->SetPoints(points[i]);
tempBoundarys[i].dataset->SetCells(VTK_QUAD, quads[i]);
注意 hass[group] 是全局节点 ID → 局部 ID 的映射，每个 boundary 有自己独立的局部点编号。

② totalGrid — 包含所有点 + 所有面/体单元


// vtkAesReader.cpp:191-283
totalGrid->SetPoints(points);           // 所有节点
totalGrid->SetCells(VTK_QUAD, quads);   // 所有边界 quad
// 可选：hex, tet, pyramid, wedge 体单元
if (gridFile.Exists("hex-->node")) { ... totalGrid->SetCells(VTK_HEXAHEDRON, hexs); }
if (gridFile.Exists("tet-->node")) { ... totalGrid->SetCells(VTK_TETRA, tets); }
③ zoneGrids — 只有点，没有 cell！


// vtkAesReader.cpp:287-304
for (int i = 0; i < nZone; i++) {
    // 只收集该 zone 所有 boundary 的点
    for (auto &x : boundarys[i])
        for (auto &y : x.pointSet)
            points[i]->InsertNextPoint(nodeCoordinate[y-1].coordinates);
    zoneGrids[i].dataset->SetPoints(points[i]);
    // 没有 SetCells！
}
流场数据绑定 (vtkAesReader.cpp:403-450)：


// 给每个 boundary 的 dataset 绑定流场数据（按 pointSet 抽取对应节点值）
for (auto &x : boundarys[i][j].pointSet) {
    boundaryFlow->InsertNextValue(flows[k].datas[x-1]);  // x 是全局节点ID
}
boundarys[i][j].dataset->GetPointData()->AddArray(boundaryFlow);

// totalGrid 绑定全量流场数据
totalGrid->GetPointData()->AddArray(gridFlow);
2. 渲染对象创建 — CreateBasicObjects()
(vtkDisplayWindow.cpp:112) 对每个 boundary 创建 4 个 actor：


// 1. meshActor — 线框网格，黑色，无标量着色
meshMapper->SetInputData(y.dataset);
meshMapper->ScalarVisibilityOff();
boundary.meshActor->GetProperty()->SetRepresentationToWireframe();
boundary.meshActor->GetProperty()->SetColor(0,0,0);

// 2. shadeActor — 实体灰色，无标量着色
shadeMapper->SetInputData(y.dataset);
shadeMapper->ScalarVisibilityOff();
boundary.shadeActor->GetProperty()->SetColor(0.83,0.83,0.83);

// 3. contourActor — 标量云图，开启颜色插值
contourMapper->SetInputData(y.dataset);
contourMapper->ScalarVisibilityOn();
contourMapper->SetInterpolateScalarsBeforeMapping(true);  // 数据空间插值，颜色更平滑
contourMapper->SetScalarRange(flow.range);
contourMapper->SetLookupTable(flow.mainScalarBar->GetLookupTable());

// 4. edgeActor — 特征边/边界边，黑色粗线
vtkFeatureEdges → edgeMapper
boundary.edgeActor->GetProperty()->SetLineWidth(5.0);
默认只有 contourActor 被加入 renderer：


// vtkDisplayWindow.cpp:169
renderer->AddActor(boundary.contourActor);  // 只有这一行！
// meshActor / shadeActor / edgeActor 创建了但未加入
3. Zone 数据在渲染中的作用
Zone 的 dataset（只有点）被用于 OutlineFilter，即包围盒线框：


// vtkDisplayWindow.cpp:274-288
auto zoneGrids = aesReader.GetZoneGrids();
for (auto &x : zoneGrids) {
    vtkSmartPointer<vtkOutlineFilter> filter = ...;
    filter->SetInputData(x.dataset);   // 用 zone 的点集算包围盒
    // → 渲染为黑色线框包围盒
    renderer->AddActor(outline);
}
Zone 本身没有任何表面或体渲染，只贡献包围盒。

4. Mesh Actor vs Contour Actor 的区别
meshActor	contourActor
数据源	boundary.dataset	boundary.dataset
着色	ScalarVisibilityOff 黑色	ScalarVisibilityOn + LUT
表示	Wireframe（线框）	默认 Surface（实体面）
插值	无	SetInterpolateScalarsBeforeMapping(true)
默认显示	否（需调用 AddMeshActor()）	是（构造时加入 renderer）
SetInterpolateScalarsBeforeMapping(true) 的意义：标量值先在数据空间做线性插值，再映射到颜色，比直接插值颜色更准确（避免色彩空间非线性导致的伪影）。

5. IsoSurface 和 Cutplane — 使用 totalGrid

// IsoSurface: vtkDisplayWindow.cpp:891
deriveds.contourFilter->SetInputData(aesReader.GetTotalGrid());  // 用体网格
deriveds.contourFilter->SetValue(0, range[0] + (range[1]-range[0]) * 0.5);
// vtkContourFilter 从体网格中提取等值面 → 输出 polydata（表面）

// Cutplane: vtkDisplayWindow.cpp:946
auto totalMesh = aesReader.GetTotalGrid();
cutter->SetInputData(totalMesh);   // 用体网格
cutter->SetCutFunction(plane);
// vtkCutter 切割体网格 → 输出截面 polydata（表面）
两者都是从 totalGrid 的体单元中提取表面，不是体渲染。

6. 回答你的核心问题
是否只有 boundary 渲染，没有 zone 体渲染？

是的，确认如下：

boundary.dataset → 所有表面 actor（contour/mesh/shade/edge/velocity）
zoneGrids.dataset → 只用于 vtkOutlineFilter 包围盒，且该 dataset 本身没有 cell
totalGrid → 用于 vtkContourFilter（等值面）和 vtkCutter（切面），两者输出的都是表面，不是体积渲染
代码中没有任何 vtkVolume、vtkVolumeMapper、vtkSmartVolumeMapper 等体渲染组件
数据结构关系总结：


HDF5 文件
  ├─ boundary quads ──→ Boundary.dataset (quad面片 + 流场点数据)
  │                          ↓
  │              contourActor / meshActor / shadeActor / edgeActor / velocityActor
  │
  ├─ zone points ────→ Zone.dataset (仅点，无cell)
  │                          ↓
  │                    OutlineFilter → 包围盒 actor
  │
  └─ volume cells ───→ totalGrid (所有点 + quad + hex/tet/pyr/wedge + 流场)
                             ↓
                   ContourFilter → isoSurfaceActor (等值面)
                   Cutter        → cutplaneActors  (截面)