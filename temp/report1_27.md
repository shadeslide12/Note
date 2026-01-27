# Group Meeting Report (1/27)

### Post Part

##### 1.Control Panel 右键菜单功能的添加

使用CustomContextMenu并配合EventFilter，实现了类似 Tecplot 的交互方式，现在可以在 ControlPanel 的 Transculency 列右键弹出菜单，快速设置多个 boundary 的透明度。

<video src=".\Screencast From 2026-01-27 10-45-05.webm"></video>

##### 2.Main Model 里 Contour Preset 预设切替的实现

 为`ColorBarDialog`添加了颜色预设Preset切替功能，现在Main Model像主流的后处理软件一样快速的切换配色，而不再是单一的`Blue->Red`配色。同时借鉴Tecplot，添加了Reverse Map选项，可以倒置原有的颜色映射。

![image-20260127171031283](C:\Users\N\AppData\Roaming\Typora\typora-user-images\image-20260127171031283.png)



##### 3.ColorBarDialog 部分问题的修复

- 修复了原有ColorBarDialog会阻塞主窗口交互的问题

- 修复了    原来每次点击Button 都会重复创建 ColorBarDialog 的问题，同时原有的代码会在每次调出Dialog时都去读取一次Flow 的最大值和最小值，造成了不必要的性能浪费。
