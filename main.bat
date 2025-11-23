@echo off
REM -----------------------------
REM Compile GLAD + GLFW program
REM -----------------------------
REM Set paths (adjust if your project is somewhere else)
set PROJECT_DIR=%~dp0
set INCLUDE_DIR=%PROJECT_DIR%include
set SRC_DIR=%PROJECT_DIR%src
set OUTPUT=%PROJECT_DIR%MosquitoGL.exe

REM Use MSYS2 MinGW64 compiler
REM This assumes you are running this from MSYS2 MinGW64 terminal or MSYS2 PATH is in system
C:\msys64\mingw64\bin\g++.exe main.cpp src\glad.c -o "%OUTPUT%" -Iinclude -I/mingw64/include -L/mingw64/lib -lglfw3 -lopengl32 -lgdi32

if %ERRORLEVEL% neq 0 (
    echo.
    echo -----------------------------
    echo Build failed!
    echo -----------------------------
    pause
) else (
    echo.
    echo -----------------------------
    echo Build successful!
    echo Running "%OUTPUT%"
    echo -----------------------------
    start "" "%OUTPUT%"
)
pause
