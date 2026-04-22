# 3/10 Update
## Post Part
### 1. Peridoic Copy的复制体的同步问题修复

若用户一开始让Model 模型处于Shade 模式，而非Contour着色，那么此时复制出来的流道仍会处于着色状态，二者会不统一。同理Mesh 和 Edge 也会发生类似问题

<video src="C:\Users\N\OneDrive\视频\Clipchamp\Video Project 4\Exports\Video Project 5.mp4"></video>

### 2. Periodic Copy 时 Outline 显示异常修复

当用户通过进行流道复制后，Outline只会包围原始几何体。

- 现在当监测到程序内存在复制体的时候，会自动隐藏掉Outline



## 3. Blade-to-Blade 平面创建相关问题                                                              

1.用户切换到 Meridional、Blade-Blade 或 Three View 模式时，如果从未移动过 `Sli_Span` slider，则 B2B 平面不会显示（空白视图）。

- 现在会先检查 `BladeToBladePlaneActor` 是否为空，若为空则使用当前 slider 值（默认 0.5 span）自动创建一次 B2B 平面

2.B2B 视图允许旋转操作，但作为 2D 剖面图，旋转会导致视角混乱。

- Meridional View 已正确限制为仅平移/缩放。

3.B2B 平面的生成直接由 `Sli_Span` 的 `valueChanged` 信号触发，每次拖动 slider 都会执行高开销的平面生成操作从而发生卡顿。

- 在控制面版新增Apply Button，现在slider只负责传递数值，最后的平面创建触发操作由Button负责

---

## 4. 新Case加载时的重置问题

按下 NewWindow 或通过 OpenFile 加载新 grid 文件时，程序不会完整重置内部状态：
- 右侧 Meridional / Blade-to-Blade 视图残留前一个模型的内容

- 视图模式不会切换回单视图（3D View）

- 左侧控制 面板仍处于 Enabled 状态，用户此时点击 `Check_Meri` 等按钮会导致程序崩溃

  <video src="C:\Users\N\OneDrive\视频\Clipchamp\Video Project 4\Exports\Video Project 4.mp4"></video>
