import os
import sys
import shutil
import subprocess
import platform

# Get installation path from args or use current directory
path = sys.argv[1] if len(sys.argv) > 1 else os.getcwd()
install_dir = os.path.join(path, "Tamerlane-Chess")
os.makedirs(install_dir, exist_ok=True)

# Create and move to build directory
build_dir = "build"
os.makedirs(build_dir, exist_ok=True)
os.chdir(build_dir)

# Run CMake and build
subprocess.run(["cmake", "..", "-DCMAKE_BUILD_TYPE=Release"], check=True)
subprocess.run(["cmake", "--build", ".", "--config", "Release"], check=True)

# Copy executable to install directory
executable_name = "Tamerlane-Chess.exe" if platform.system() == "Windows" else "Tamerlane-Chess"
if platform.system() == "Windows":
    executable_path = os.path.join("Release", executable_name)
    if not os.path.exists(executable_path):
        # Try the default Visual Studio path structure
        executable_path = os.path.join(".", executable_name)
else:
    executable_path = executable_name

if not os.path.exists(executable_path):
    print(f"Error: Could not find executable at {executable_path}")
    sys.exit(1)

shutil.copy2(executable_path, install_dir)

# Copy assets folder if it exists
assets_dir = os.path.join("..", "assets")
if os.path.exists(assets_dir):
    shutil.copytree(assets_dir, os.path.join(install_dir, "assets"), dirs_exist_ok=True)

# On Windows, copy required DLLs
if platform.system() == "Windows":
    dll_dir = os.path.join("..", "external", "SFML-Windows", "bin")
    print(f"Looking for DLLs in: {os.path.abspath(dll_dir)}")
    sys.stdout.flush()  
    
    dlls = [
        "sfml-system-2.dll",
        "sfml-window-2.dll",
        "sfml-graphics-2.dll",
        "sfml-audio-2.dll",
        "openal32.dll"
    ]
    
    print(f"Will try to copy these DLLs: {dlls}")
    sys.stdout.flush()

    for dll in dlls:
        dll_path = os.path.join(dll_dir, dll)
        if not os.path.exists(dll_path):
            print(f"Warning: Could not find DLL at {dll_path}")
            sys.stdout.flush()
            continue
        print(f"Copying DLL: {dll}")
        sys.stdout.flush()
        try:
            shutil.copy2(dll_path, install_dir)
        except Exception as e:
            print(f"Error copying {dll}: {str(e)}")
            sys.stdout.flush()

print(f"Installation completed at: {install_dir}")

