# this runs the cmake build to create the proper build 
# then runs the Tamerlane-Chess executable in the build dir

import subprocess
import os
import sys
import platform

def run_cmake_build():
    # Create build directory if it doesn't exist
    if not os.path.exists("build"):
        os.makedirs("build")
    
    # Change to build directory
    os.chdir("build")
    
    # Run cmake and build
    subprocess.run(["cmake", ".."], check=True)
    subprocess.run(["cmake", "--build", "."], check=True)

def run_executable():
    # Determine executable name based on platform
    executable = "Tamerlane-Chess.exe" if platform.system() == "Windows" else "./Tamerlane-Chess"
    
    # Run the executable from the build directory
    subprocess.run([executable], check=True)

def main():
    try:
        run_cmake_build()
        run_executable()
    except subprocess.CalledProcessError as e:
        print(f"Error: Build or execution failed with error code {e.returncode}")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    main()