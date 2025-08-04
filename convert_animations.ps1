# FBX to GLTF Animation Converter PowerShell Script
# Requires fbx2gltf to be installed

param(
    [string]$SourceDir = "assets\Animations",
    [string]$OutputDir = "assets\Animations_GLTF",
    [switch]$Binary = $false,  # Use GLB format instead of GLTF
    [switch]$SkipExisting = $false  # Skip files that already exist
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "FBX to GLTF Animation Converter" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Check if fbx2gltf is available
$fbx2gltfPath = "C:\Users\USER\AppData\Roaming\npm\fbx2gltf.cmd"
if (!(Test-Path $fbx2gltfPath)) {
    Write-Host "ERROR: fbx2gltf is not installed" -ForegroundColor Red
    Write-Host "Please install it using: npm install -g fbx2gltf" -ForegroundColor Yellow
    Write-Host "Or download from: https://github.com/facebookincubator/FBX2glTF" -ForegroundColor Yellow
    exit 1
}

# Set file extension based on format
$extension = if ($Binary) { "glb" } else { "gltf" }

# Create output directories
$directories = @(
    $OutputDir,
    "$OutputDir\Aurora",
    "$OutputDir\Great_Sword"
)

foreach ($dir in $directories) {
    if (!(Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir | Out-Null
        Write-Host "Created directory: $dir" -ForegroundColor Green
    }
}

# Initialize counters
$converted = 0
$skipped = 0
$errors = 0
$results = @()

# Function to convert FBX to GLTF
function Convert-FBXtoGLTF {
    param(
        [string]$InputFile,
        [string]$OutputFile
    )
    
    # Check if output file already exists
    if ($SkipExisting -and (Test-Path $OutputFile)) {
        Write-Host "  SKIPPED: Output file already exists" -ForegroundColor Yellow
        return "skipped"
    }
    
    # Build fbx2gltf command
    $args = @($InputFile, "-o", $OutputFile, "--embed-images")
    if ($Binary) {
        $args += "--binary"
    }
    
    # Run conversion
    $process = Start-Process -FilePath $fbx2gltfPath -ArgumentList $args -Wait -PassThru -NoNewWindow
    
    if ($process.ExitCode -eq 0) {
        Write-Host "  SUCCESS: Created $OutputFile" -ForegroundColor Green
        return "success"
    } else {
        Write-Host "  ERROR: Failed to convert (Exit code: $($process.ExitCode))" -ForegroundColor Red
        return "error"
    }
}

# Convert Aurora animations
Write-Host "`nConverting Aurora animations..." -ForegroundColor Yellow
Write-Host "------------------------------"

$auroraFiles = Get-ChildItem -Path "$SourceDir\Aurora" -Filter "*.FBX" -ErrorAction SilentlyContinue
foreach ($file in $auroraFiles) {
    $outputFile = "$OutputDir\Aurora\$($file.BaseName).$extension"
    Write-Host "Converting: $($file.Name)"
    
    $result = Convert-FBXtoGLTF -InputFile $file.FullName -OutputFile $outputFile
    
    switch ($result) {
        "success" { $converted++; $results += "$($file.BaseName) - Converted" }
        "skipped" { $skipped++; $results += "$($file.BaseName) - Skipped (exists)" }
        "error" { $errors++; $results += "$($file.BaseName) - Failed" }
    }
    Write-Host ""
}

# Convert Great Sword animations
Write-Host "Converting Great Sword animations..." -ForegroundColor Yellow
Write-Host "-----------------------------------"

$swordFiles = Get-ChildItem -Path "$SourceDir\Great_Sword" -Filter "*.fbx" -ErrorAction SilentlyContinue
foreach ($file in $swordFiles) {
    $outputFile = "$OutputDir\Great_Sword\$($file.BaseName).$extension"
    Write-Host "Converting: $($file.Name)"
    
    $result = Convert-FBXtoGLTF -InputFile $file.FullName -OutputFile $outputFile
    
    switch ($result) {
        "success" { $converted++; $results += "Great_Sword\$($file.BaseName) - Converted" }
        "skipped" { $skipped++; $results += "Great_Sword\$($file.BaseName) - Skipped (exists)" }
        "error" { $errors++; $results += "Great_Sword\$($file.BaseName) - Failed" }
    }
    Write-Host ""
}

# Summary
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Conversion Complete!" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Total files converted: $converted" -ForegroundColor Green
Write-Host "Total files skipped: $skipped" -ForegroundColor Yellow
Write-Host "Total errors: $errors" -ForegroundColor Red
Write-Host ""
Write-Host "Output directory: $OutputDir" -ForegroundColor Cyan
Write-Host "Format: $($extension.ToUpper())" -ForegroundColor Cyan

# Create log file
$logFile = "$OutputDir\conversion_log.txt"
$logContent = @"
FBX to GLTF Conversion Log
Generated: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
=======================================

Settings:
- Source Directory: $SourceDir
- Output Directory: $OutputDir
- Format: $($extension.ToUpper())
- Skip Existing: $SkipExisting

Results:
- Total files converted: $converted
- Total files skipped: $skipped
- Total errors: $errors

File Details:
$($results -join "`n")

Converted Files:
$(Get-ChildItem -Path $OutputDir -Filter "*.$extension" -Recurse | ForEach-Object { $_.FullName })
"@

$logContent | Out-File -FilePath $logFile -Encoding UTF8
Write-Host "`nLog file created: $logFile" -ForegroundColor Gray

# Optional: Create a mapping file for easy reference
$mappingFile = "$OutputDir\animation_mapping.json"
$animationMapping = @{
    "Aurora" = @{
        "idle" = "Idle.$extension"
        "walk" = "Jog_Fwd.$extension"
        "run" = "Jog_Fwd.$extension"
        "walk_combat" = "Jog_Fwd_Combat.$extension"
        "roll_forward" = "Ability_RMB_Fwd.$extension"
        "roll_backward" = "Ability_RMB_Bwd.$extension"
        "roll_left" = "Ability_RMB_Left.$extension"
        "roll_right" = "Ability_RMB_Right.$extension"
        "jump_start" = "Jump_Start.$extension"
        "jump_apex" = "Jump_Apex.$extension"
        "jump_land" = "Jump_Land.$extension"
        "attack_1" = "Primary_Attack_A.$extension"
        "attack_2" = "Primary_Attack_B.$extension"
        "attack_3" = "Primary_Attack_C.$extension"
        "parry" = "HitReact_Front.$extension"
        "death" = "Death.$extension"
    }
    "GreatSword" = @{
        "idle" = "Great Sword Idle.$extension"
        "walk" = "Great Sword Walk.$extension"
        "run" = "Great Sword Run.$extension"
        "slash" = "Great Sword Slash.$extension"
    }
}

$animationMapping | ConvertTo-Json -Depth 3 | Out-File -FilePath $mappingFile -Encoding UTF8
Write-Host "Animation mapping file created: $mappingFile" -ForegroundColor Gray