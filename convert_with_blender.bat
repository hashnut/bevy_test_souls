@echo off
echo ========================================
echo FBX to GLTF/GLB Converter using Blender
echo ========================================
echo.

REM Check if Blender is installed
where blender >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: Blender is not found in PATH
    echo Please install Blender or add it to your PATH
    echo.
    echo Common Blender installation paths:
    echo - C:\Program Files\Blender Foundation\Blender 4.0\blender.exe
    echo - C:\Program Files\Blender Foundation\Blender 3.6\blender.exe
    echo.
    echo You can also specify the full path to blender.exe below
    pause
    exit /b 1
)

REM Run the conversion script
echo Running conversion script...
echo.

blender --background --python blender_fbx_to_gltf.py -- --input_dir assets/Animations --output_dir assets/Animations_GLTF --format glb

echo.
echo Conversion complete!
pause