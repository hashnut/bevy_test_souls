# Download fbx2gltf Windows binary directly from GitHub

$downloadUrl = "https://github.com/facebookincubator/FBX2glTF/releases/download/v0.13.1/FBX2glTF-windows-x64.zip"
$outputPath = "fbx2gltf.zip"
$extractPath = "fbx2gltf_bin"

Write-Host "Downloading fbx2gltf Windows binary..." -ForegroundColor Yellow

# Download the zip file
try {
    Invoke-WebRequest -Uri $downloadUrl -OutFile $outputPath
    Write-Host "Download completed!" -ForegroundColor Green
} catch {
    Write-Host "Error downloading fbx2gltf: $_" -ForegroundColor Red
    exit 1
}

# Extract the zip file
Write-Host "Extracting fbx2gltf..." -ForegroundColor Yellow
if (Test-Path $extractPath) {
    Remove-Item -Path $extractPath -Recurse -Force
}
Expand-Archive -Path $outputPath -DestinationPath $extractPath -Force
Write-Host "Extraction completed!" -ForegroundColor Green

# Clean up zip file
Remove-Item -Path $outputPath -Force

Write-Host "`nfbx2gltf has been downloaded to: $extractPath" -ForegroundColor Cyan
Write-Host "You can now use: .\$extractPath\FBX2glTF.exe" -ForegroundColor Cyan