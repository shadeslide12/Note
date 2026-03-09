# zjui-post Windows Deployment Script
# 自动收集所有依赖 DLL 和资源文件，创建可分发的程序包

param(
    [string]$BuildDir = "cmake-build-release-visual-studio26",
    [string]$DeployDir = "zjui-post-deploy"
)

$ErrorActionPreference = "Continue"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "zjui-post Windows Deployment Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# 1. 创建部署目录
Write-Host "`n[1/6] Creating deployment directory..." -ForegroundColor Yellow
if (Test-Path $DeployDir) {
    Remove-Item -Recurse -Force $DeployDir
}
New-Item -ItemType Directory -Path $DeployDir | Out-Null
New-Item -ItemType Directory -Path "$DeployDir\platforms" | Out-Null

# 2. 复制可执行文件和项目 DLL
Write-Host "[2/6] Copying executable and project libraries..." -ForegroundColor Yellow
Copy-Item "$BuildDir\bin\cfd_post.exe" "$DeployDir\"
Copy-Item "$BuildDir\bin\post_base_lib.dll" "$DeployDir\"
Copy-Item "$BuildDir\bin\ColorMaps.json" "$DeployDir\"

# 3. 使用 windeployqt 自动收集 Qt 依赖
Write-Host "[3/6] Collecting Qt dependencies with windeployqt..." -ForegroundColor Yellow
$QtBinDir = "D:\Compile\Qt_Win\5.15.2\msvc2019_64\bin"
& "$QtBinDir\windeployqt.exe" --release --no-translations "$DeployDir\cfd_post.exe"

# 4. 手动复制 VTK DLL（windeployqt 不会自动处理）
Write-Host "[4/6] Copying VTK libraries..." -ForegroundColor Yellow
$VtkLibDir = "D:\Compile_Libs\VTK\9.1.0\x64_release_new\bin"

# Copy ALL VTK DLLs to avoid missing dependencies
Write-Host "  Copying all VTK DLLs from $VtkLibDir..." -ForegroundColor Gray
$VtkDlls = Get-ChildItem "$VtkLibDir\vtk*.dll" | Select-Object -ExpandProperty Name

foreach ($dll in $VtkDlls) {
    $sourcePath = Join-Path $VtkLibDir $dll
    if (Test-Path $sourcePath) {
        try {
            Copy-Item $sourcePath "$DeployDir\" -ErrorAction Stop
            Write-Host "  Copied: $dll" -ForegroundColor Gray
        } catch {
            Write-Host "  Error copying $dll : $_" -ForegroundColor Red
        }
    } else {
        Write-Host "  Warning: $dll not found" -ForegroundColor Yellow
    }
}

# 5. 复制 HDF5 DLL
Write-Host "[5/6] Copying HDF5 libraries..." -ForegroundColor Yellow
$Hdf5LibDir = "D:\Compile_Libs\HDF5\1.12.2\x64_Release_new\bin"
$Hdf5Dlls = @("hdf5.dll", "hdf5_hl.dll", "zlib.dll", "szip.dll")
foreach ($dll in $Hdf5Dlls) {
    $sourcePath = Join-Path $Hdf5LibDir $dll
    if (Test-Path $sourcePath) {
        Copy-Item $sourcePath "$DeployDir\"
        Write-Host "  Copied: $dll" -ForegroundColor Gray
    } else {
        Write-Host "  Warning: $dll not found (may be optional)" -ForegroundColor Yellow
    }
}

# 6. 复制 MSVC 运行时（如果需要）
Write-Host "[6/6] Checking MSVC runtime..." -ForegroundColor Yellow
$VcRedistDlls = @("vcruntime140.dll", "msvcp140.dll", "vcruntime140_1.dll")
$SystemDir = "C:\Windows\System32"
foreach ($dll in $VcRedistDlls) {
    $sourcePath = Join-Path $SystemDir $dll
    if (Test-Path $sourcePath) {
        Copy-Item $sourcePath "$DeployDir\" -ErrorAction SilentlyContinue
    }
}

# 7. 创建 README
Write-Host "`nCreating README.txt..." -ForegroundColor Yellow
$readme = @"
zjui-post - CFD Post-Processing Tool
=====================================

运行要求：
- Windows 10/11 (64-bit)
- 如果提示缺少 MSVC 运行时，请安装：
  https://aka.ms/vs/17/release/vc_redist.x64.exe

使用方法：
1. 双击 cfd_post.exe 启动程序
2. File → OpenFile 打开 HDF5 网格文件
3. 使用工具栏进行可视化操作

文件说明：
- cfd_post.exe        - 主程序
- post_base_lib.dll   - 基础库
- ColorMaps.json      - 颜色映射配置
- *.dll               - 依赖库（Qt, VTK, HDF5）
- platforms/          - Qt 平台插件

版本信息：
- 编译日期: $(Get-Date -Format "yyyy-MM-dd HH:mm")
- Qt 版本: 5.15.2
- VTK 版本: 9.1.0
- HDF5 版本: 1.12.2
"@
Set-Content -Path "$DeployDir\README.txt" -Value $readme -Encoding UTF8

# 8. 统计信息
Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "Deployment Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
$fileCount = (Get-ChildItem -Recurse $DeployDir | Where-Object {!$_.PSIsContainer}).Count
$totalSize = (Get-ChildItem -Recurse $DeployDir | Measure-Object -Property Length -Sum).Sum / 1MB
Write-Host "Total files: $fileCount" -ForegroundColor Green
Write-Host "Total size: $([math]::Round($totalSize, 2)) MB" -ForegroundColor Green
Write-Host "Deployment directory: $(Resolve-Path $DeployDir)" -ForegroundColor Green

Write-Host "`n[SUCCESS] Deployment package created!" -ForegroundColor Green
Write-Host "You can now zip '$DeployDir' and distribute it." -ForegroundColor Green
