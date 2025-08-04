# FBX to GLTF/GLB Converter using Blender

param(
    [string]$BlenderPath = "",  # Optional: specify Blender path
    [string]$InputDir = "assets\Animations",
    [string]$OutputDir = "assets\Animations_GLTF",
    [string]$Format = "glb",  # glb or gltf
    [switch]$SkipExisting = $false
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "FBX to GLTF/GLB Converter using Blender" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Try to find Blender
if ($BlenderPath -eq "") {
    # Check if Blender is in PATH
    $blender = Get-Command blender -ErrorAction SilentlyContinue
    
    if (-not $blender) {
        # Check common installation paths
        $possiblePaths = @(
            "C:\Program Files\Blender Foundation\Blender 4.2\blender.exe",
            "C:\Program Files\Blender Foundation\Blender 4.1\blender.exe",
            "C:\Program Files\Blender Foundation\Blender 4.0\blender.exe",
            "C:\Program Files\Blender Foundation\Blender 3.6\blender.exe",
            "C:\Program Files\Blender Foundation\Blender 3.5\blender.exe",
            "C:\Program Files\Blender Foundation\Blender 3.4\blender.exe",
            "C:\Program Files\Blender Foundation\Blender 3.3\blender.exe",
            "C:\Program Files\Blender Foundation\Blender\blender.exe"
        )
        
        foreach ($path in $possiblePaths) {
            if (Test-Path $path) {
                $BlenderPath = $path
                Write-Host "Found Blender at: $BlenderPath" -ForegroundColor Green
                break
            }
        }
        
        if ($BlenderPath -eq "") {
            Write-Host "ERROR: Blender not found!" -ForegroundColor Red
            Write-Host "Please install Blender from: https://www.blender.org/download/" -ForegroundColor Yellow
            Write-Host "Or specify the path using -BlenderPath parameter" -ForegroundColor Yellow
            exit 1
        }
    } else {
        $BlenderPath = "blender"
    }
}

# Build command arguments
$scriptPath = Join-Path $PSScriptRoot "blender_fbx_to_gltf.py"
$args = @(
    "--background",
    "--python", $scriptPath,
    "--",
    "--input_dir", $InputDir,
    "--output_dir", $OutputDir,
    "--format", $Format
)

if ($SkipExisting) {
    $args += "--skip_existing"
}

Write-Host "Running conversion script..." -ForegroundColor Yellow
Write-Host "Input directory: $InputDir" -ForegroundColor Gray
Write-Host "Output directory: $OutputDir" -ForegroundColor Gray
Write-Host "Format: $($Format.ToUpper())" -ForegroundColor Gray
Write-Host ""

# Run Blender
& $BlenderPath $args

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nConversion completed successfully!" -ForegroundColor Green
    
    # Check if output files were created
    $outputPath = Join-Path $PSScriptRoot $OutputDir
    if (Test-Path $outputPath) {
        $fileCount = (Get-ChildItem -Path $outputPath -Filter "*.$Format" -Recurse).Count
        Write-Host "Total $($Format.ToUpper()) files created: $fileCount" -ForegroundColor Cyan
    }
} else {
    Write-Host "`nConversion failed with exit code: $LASTEXITCODE" -ForegroundColor Red
}

Write-Host "`nPress any key to continue..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")