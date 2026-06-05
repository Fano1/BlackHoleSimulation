param(
    [switch]$NoRun,
    [switch]$Debug,
    [switch]$Release
)

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $root

$buildDir = Join-Path $root "build"
New-Item -ItemType Directory -Force -Path $buildDir | Out-Null

function Find-Cl {
    $cl = Get-Command cl.exe -ErrorAction SilentlyContinue
    if ($cl) { return $cl.Source }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) { return $null }

    $install = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $install) { return $null }

    $vcvars = Join-Path $install "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path $vcvars)) { return $null }

    $envDump = & cmd /c "`"$vcvars`" >nul && set"
    foreach ($line in $envDump) {
        $pair = $line -split "=", 2
        if ($pair.Length -eq 2) {
            [Environment]::SetEnvironmentVariable($pair[0], $pair[1], "Process")
        }
    }
    $cl = Get-Command cl.exe -ErrorAction SilentlyContinue
    if ($cl) { return $cl.Source }
    return $null
}

$clPath = Find-Cl
if (-not $clPath) {
    throw "MSVC cl.exe was not found. Install Visual Studio Build Tools with the C++ workload, then run this script again."
}

$cfg = if ($Debug -and -not $Release) { "Debug" } else { "Release" }
$outDir = Join-Path $buildDir $cfg
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

$common = @(
    "/nologo",
    "/std:c++17",
    "/EHsc",
    "/utf-8",
    "/MP",
    "/Iinclude\imgui\examples\libs\glfw\include",
    "/Iinclude",
    "/Iinclude\imgui",
    "/Iinclude\imgui\backends",
    "/D_CRT_SECURE_NO_WARNINGS"
)

if ($cfg -eq "Release") {
    $common += @("/O2", "/Oi", "/GL", "/DNDEBUG", "/MD")
    $linkExtra = @("/LTCG", "/OPT:REF", "/OPT:ICF")
} else {
    $common += @("/Zi", "/Od", "/MD")
    $linkExtra = @("/DEBUG")
}

$sources = @(
    "src\main.cpp",
    "include\imgui\imgui.cpp",
    "include\imgui\imgui_draw.cpp",
    "include\imgui\imgui_tables.cpp",
    "include\imgui\imgui_widgets.cpp",
    "include\imgui\backends\imgui_impl_glfw.cpp",
    "include\imgui\backends\imgui_impl_opengl3.cpp"
)

$exe = Join-Path $outDir "blackhole_sim.exe"
$objArg = "/Fo$outDir\"
$pdbArg = "/Fd$(Join-Path $outDir 'blackhole_sim.pdb')"
$libs = @(
    "include\imgui\examples\libs\glfw\lib-vc2010-64\glfw3.lib",
    "opengl32.lib",
    "gdi32.lib",
    "user32.lib",
    "shell32.lib"
)

Write-Host "Building $cfg..." -ForegroundColor Magenta
& cl.exe @common $objArg $pdbArg @sources "/Fe$exe" "/link" @linkExtra @libs

if ($LASTEXITCODE -ne 0) {
    throw "Build failed."
}

Write-Host "Built: $exe" -ForegroundColor Green
if (-not $NoRun) {
    Write-Host "Running simulation..." -ForegroundColor Magenta
    & $exe
}
