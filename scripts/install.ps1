# Run the buildRelease.bat file
$batchFilePath = Join-Path $PSScriptRoot "buildRelease.bat"
Write-Host "Executing batch file: $batchFilePath"
$process = Start-Process -FilePath $batchFilePath -Wait -NoNewWindow -PassThru

if ($process.ExitCode -ne 0) {
    Write-Host "Batch file execution failed with exit code: $($process.ExitCode)"
    exit $process.ExitCode
}

Write-Host "Batch file executed successfully"

# Set the release directory
$parentDir = (Get-Item $PSScriptRoot).Parent.Parent.FullName
$releaseDir = Join-Path $parentDir "Tamerlane Chess"
if (-not (Test-Path $releaseDir)) {
    New-Item -ItemType Directory -Path $releaseDir -Force
}
$releaseDir = Resolve-Path $releaseDir
Write-Host "Release directory: $releaseDir"

# Set the path for the executable
$exePath = Join-Path $releaseDir "main.exe"
Write-Host "Executable path: $exePath"

# Set the path for the icon (assuming it's in the assets folder)
$iconPath = Join-Path $releaseDir "assets\icon.ico"
Write-Host "Icon path: $iconPath"

# Set the name for the shortcut
$shortcutName = "Tamerlane Chess.lnk"

# Create a shortcut
$WshShell = New-Object -ComObject WScript.Shell
$Shortcut = $WshShell.CreateShortcut((Join-Path $releaseDir $shortcutName))
$Shortcut.TargetPath = $exePath
$Shortcut.IconLocation = $iconPath
$Shortcut.Save()

Write-Host "Installation complete. Shortcut created in $releaseDir"