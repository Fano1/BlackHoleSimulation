# OpenGL + GLFW Setup on Windows (MSYS2)

This guide sets up MSYS2, GCC, GLFW, and VSCode for C++ OpenGL development.

---

## 1. Install MSYS2
1. Download & install MSYS2: [https://www.msys2.org](https://www.msys2.org)  
2. Default installation path: `C:\msys64`

---

## 2. Update & Install Tools
Open **MSYS2 MinGW64 terminal** and run:

```bash
pacman -Syu                             # Update system
pacman -S mingw-w64-ucrt-x86_64-gcc    # Install GCC 64-bit
gcc --version                            # Verify GCC
pacman -S mingw-w64-x86_64-glfw        # Install GLFW
``` 

## 3. Set Environment Variables

Open System Properties → Advanced → Environment Variables
Add a new system variable:
Name: MSYS2
Value: C:\msys64
Edit Path and add:
```bash
C:\msys64\mingw64\bin
```

## 4. Configure VSCode

Go to Settings → C/C++: Default Include Path
Set include path to:
```bash
${env:MSYS2}/mingw64/include/
```

## 5. Run the program
Use main.cpp to write the program and main.bat to run the program