Write-Host "=== Tamerlane Chess Installation Script ==="
Write-Host "This script will build and install Tamerlane Chess."

Write-Host "`n--- Step 1: Building the Release ---"
# Run the buildRelease.bat file
$batchFilePath = Join-Path $PSScriptRoot "buildRelease.bat"
Write-Host "Executing batch file: $batchFilePath"
$process = Start-Process -FilePath $batchFilePath -Wait -NoNewWindow -PassThru

if ($process.ExitCode -ne 0) {
    Write-Host "Error: Batch file execution failed with exit code: $($process.ExitCode)"
    exit $process.ExitCode
}

Write-Host "Success: Batch file executed successfully"

Write-Host "`n--- Step 2: Setting Up Release Directory ---"
# Set the release directory
$parentDir = (Get-Item $PSScriptRoot).Parent.Parent.FullName
$releaseDir = Join-Path $parentDir "Tamerlane Chess"
if (-not (Test-Path $releaseDir)) {
    Write-Host "Creating release directory..."
    New-Item -ItemType Directory -Path $releaseDir -Force
}
$releaseDir = Resolve-Path $releaseDir
Write-Host "Release directory set to: $releaseDir"

Write-Host "`n--- Step 3: Locating Files ---"
# Set the path for the executable
$exePath = Join-Path $releaseDir "main.exe"
# Set the path for the icon (assuming it's in the assets folder)
$iconPath = Join-Path $releaseDir "assets\icon.ico"

Write-Host "`n--- Step 4: Creating Shortcut ---"
# Set the name for the shortcut
$shortcutName = "Tamerlane Chess.lnk"

# Create a shortcut
$WshShell = New-Object -ComObject WScript.Shell
$Shortcut = $WshShell.CreateShortcut((Join-Path $releaseDir $shortcutName))
$Shortcut.TargetPath = $exePath
$Shortcut.IconLocation = $iconPath
$Shortcut.Save()

Write-Host "`n=== Installation Complete ==="
Write-Host "You can now launch Tamerlane Chess using this shortcut in $releaseDir. Enjoy!"