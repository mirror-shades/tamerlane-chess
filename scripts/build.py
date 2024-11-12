# no flag builds release
# `run` flag builds and runs release
# `install [path]` installs game to a directory

import platform
import subprocess
import os
import sys
import shutil

def run_cmake_build():    
    # Create build directory if it doesn't exist
    if os.path.exists("build"):
        print("Removing existing build directory")
        shutil.rmtree("build", ignore_errors=True)
        
    os.makedirs("build")

    print("Creating build directory")

    # Change to build directory
    os.chdir("build")
    
    print("Running cmake")
    try:
        # Check the operating system
        if platform.system() == "Windows":
            # Use MinGW Makefiles generator for Windows
            command = ["cmake", "..", "-G", "MinGW Makefiles"]
        else:
            # Use default generator for other OS
            command = ["cmake", ".."]
        subprocess.run(command, check=True)

        # Build using cmake --build
        subprocess.run(["cmake", "--build", "."], check=True)
    except Exception as e:
        print(f"CMake build failed: {str(e)}")
        sys.exit(1)
    
    os.chdir("..")
    print("Build complete")

def run_executable():
    # Run the executable from the build directory
    print("Running executable")
    try:
        subprocess.run(["./build/Tamerlane-Chess"], check=True)
    except Exception as e:
        print(f"Executable failed to run: {str(e)}")
        sys.exit(1)

def run_install(path):
    print(f"Installing from build directory to {path}/Tamerlane-Chess")
    # check if the path exists
    if not os.path.exists(path):
        print(f"Error: Path {path} does not exist")
        sys.exit(1)
    
    #check if the Tamerlane-Chess directory exists
    if os.path.exists(os.path.join(path, "Tamerlane-Chess")):
        #ask to overwrite
        overwrite = input(f"Tamerlane-Chess directory already exists in {path}. Overwrite? (y/N): ")
        if overwrite.lower() != "y":
            print("Installation cancelled.")
            sys.exit(1)
        else:
            shutil.rmtree(os.path.join(path, "Tamerlane-Chess"))
    
    try:    
        # Create the target directory first
        print("Creating Tamerlane-Chess directory")
        os.makedirs(os.path.join(path, "Tamerlane-Chess"), exist_ok=True)
        
        if platform.system() == "Windows":
            # Windows-specific files
            files_to_copy = [
                "Tamerlane-Chess.exe",
                "sfml-audio-2.dll",
                "sfml-graphics-2.dll",
                "sfml-window-2.dll",
                "sfml-system-2.dll",
                "openal32.dll"
            ]
            
            print("Copying files to Tamerlane-Chess directory")
            # Copy files from build directory
            for file in files_to_copy:
                print(f"Copying {file}")
                shutil.copy(f"build/{file}", os.path.join(path, "Tamerlane-Chess"))
                
            # Copy files from external DLL directory
            external_dll_path = "external/RELEASE-DLL-Windows"
            if os.path.exists(external_dll_path):
                print("Copying external DLL files")
                for file in os.listdir(external_dll_path):
                    print(f"Copying {file}")
                    shutil.copy(
                        os.path.join(external_dll_path, file),
                        os.path.join(path, "Tamerlane-Chess")
                    )
        else:
            # Linux installation - just copy the executable
            print("Copying executable")
            shutil.copy("build/Tamerlane-Chess", os.path.join(path, "Tamerlane-Chess"))
                     
        # Copy the assets folder
        print("Copying assets folder")
        shutil.copytree("assets", os.path.join(path, "Tamerlane-Chess/assets"))
        
    except Exception as e:
        print(f"Error during installation: {str(e)}")
        #remove the Tamerlane-Chess directory if it exists
        if os.path.exists(os.path.join(path, "Tamerlane-Chess")):
            shutil.rmtree(os.path.join(path, "Tamerlane-Chess"))
        sys.exit(1)

    print(f"Installation complete. Tamerlane-Chess executable is in {path}/Tamerlane-Chess")
    


def main():
    runAfterBuild = False
    install = False
    installPath = ""
    if len(sys.argv) > 1 and sys.argv[1] == "run":
        runAfterBuild = True
    elif len(sys.argv) > 1 and sys.argv[1] == "install":
        install = True
        if not len(sys.argv) > 2:
            print("Error: No path provided for installation")
            print("Install to current directory? (y/n):     ")
            if input().lower() != "y":
                sys.exit(1)
            else:
                installPath = os.getcwd()
        else:
            installPath = sys.argv[2]
    try:
        run_cmake_build()
        if runAfterBuild:
            run_executable()
        if install and installPath != "":
            run_install(installPath)
    except subprocess.CalledProcessError as e:
        print(f"Error: Build or execution failed with error code {e.returncode}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    main()