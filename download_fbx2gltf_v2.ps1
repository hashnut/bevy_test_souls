# Download fbx2gltf Windows binary directly from GitHub

$downloadUrl = "https://github.com/facebookincubator/FBX2glTF/releases/download/v0.9.7/FBX2glTF-windows-x64.exe"
$outputPath = "fbx2gltf_bin/FBX2glTF.exe"

# Create directory if it doesn't exist
$dir = Split-Path -Path $outputPath
if (!(Test-Path $dir)) {
    New-Item -ItemType Directory -Path $dir | Out-Null
}

Write-Host "Downloading fbx2gltf Windows binary..." -ForegroundColor Yellow

# Download the executable directly
try {
    Invoke-WebRequest -Uri $downloadUrl -OutFile $outputPath
    Write-Host "Download completed!" -ForegroundColor Green
    Write-Host "fbx2gltf has been downloaded to: $outputPath" -ForegroundColor Cyan
} catch {
    Write-Host "Error downloading fbx2gltf: $_" -ForegroundColor Red
    Write-Host "Please download manually from: https://github.com/facebookincubator/FBX2glTF/releases" -ForegroundColor Yellow
    exit 1
}