@echo off
setlocal enabledelayedexpansion

echo ========================================
echo FBX to GLTF Animation Converter
echo ========================================

:: Check if fbx2gltf is installed
where fbx2gltf >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: fbx2gltf is not installed or not in PATH
    echo Please install it using: npm install -g fbx2gltf
    echo Or download from: https://github.com/facebookincubator/FBX2glTF
    pause
    exit /b 1
)

:: Set paths
set "SOURCE_DIR=assets\Animations"
set "OUTPUT_DIR=assets\Animations_GLTF"

:: Create output directories if they don't exist
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
if not exist "%OUTPUT_DIR%\Aurora" mkdir "%OUTPUT_DIR%\Aurora"
if not exist "%OUTPUT_DIR%\Great_Sword" mkdir "%OUTPUT_DIR%\Great_Sword"

:: Counter for converted files
set /a count=0
set /a errors=0

:: Convert Aurora animations
echo.
echo Converting Aurora animations...
echo ------------------------------

for %%f in ("%SOURCE_DIR%\Aurora\*.FBX") do (
    set "filename=%%~nf"
    set "output_file=%OUTPUT_DIR%\Aurora\!filename!.gltf"
    
    echo Converting: %%~nxf
    fbx2gltf "%%f" -o "!output_file!" --embed-images
    
    if !errorlevel! equ 0 (
        echo SUCCESS: !filename!.gltf created
        set /a count+=1
    ) else (
        echo ERROR: Failed to convert %%~nxf
        set /a errors+=1
    )
    echo.
)

:: Convert Great Sword animations
echo.
echo Converting Great Sword animations...
echo -----------------------------------

for %%f in ("%SOURCE_DIR%\Great_Sword\*.fbx") do (
    set "filename=%%~nf"
    set "output_file=%OUTPUT_DIR%\Great_Sword\!filename!.gltf"
    
    echo Converting: %%~nxf
    fbx2gltf "%%f" -o "!output_file!" --embed-images
    
    if !errorlevel! equ 0 (
        echo SUCCESS: !filename!.gltf created
        set /a count+=1
    ) else (
        echo ERROR: Failed to convert %%~nxf
        set /a errors+=1
    )
    echo.
)

:: Summary
echo ========================================
echo Conversion Complete!
echo ========================================
echo Total files converted: %count%
echo Total errors: %errors%
echo.
echo Output directory: %OUTPUT_DIR%
echo.

:: Create a log file with conversion results
echo Conversion Log - %date% %time% > "%OUTPUT_DIR%\conversion_log.txt"
echo =============================== >> "%OUTPUT_DIR%\conversion_log.txt"
echo Total files converted: %count% >> "%OUTPUT_DIR%\conversion_log.txt"
echo Total errors: %errors% >> "%OUTPUT_DIR%\conversion_log.txt"
echo. >> "%OUTPUT_DIR%\conversion_log.txt"
echo Converted files: >> "%OUTPUT_DIR%\conversion_log.txt"
dir /b /s "%OUTPUT_DIR%\*.gltf" >> "%OUTPUT_DIR%\conversion_log.txt"

pause