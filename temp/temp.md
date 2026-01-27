# Code to fix
## ColorBarDialog
 - 修復json查找路徑問題，讓它能夠在其他電腦上移植時依然能夠正確加載
 - 修改Dialog 裏的 ButtonBox ，只留下一個close按鈕就可以了

## Control Panel
 - 移除GlobalTransparency 控件 
 - 将showZone那一列的右键菜单，改为Activate 和 Deactivate 两种，并增加相关的控制函数，控制boundary的显示与否。此外ContourMode 和 Zone Delete 两列不应该有右键菜单，至少现在还不该有

## Slice 部分
 - 增加slice 部分创建的ScalarBarWidget的右键功能，使其能和Main Model一样右键ScalarBar 跳出 CutPlaneDialog
 - 为 CutPlaneDialog 新增标签页 ，参照ColorBarDialog相同的Legend配置标签页，让他控制 Slice的ScalarBarWidget
 - 修改原有的标签页的风格，使其与新增的Legend风格一致

## MainWindow
- 删除StreamTrace 按钮，同时移除相关代码
- 删除Calculator 按钮
- 删除Specialized 下方 的Meridional plane 按钮，，同时移除相关函数
- 调整Turbo内部控件的排布
- 增加Blade to Blade 区域的按钮相应
- 增加Meridional 区域按钮的响应