import os
import shutil
import subprocess

# Store the current directory
cur_dir = os.getcwd()

# Navigate to the project root directory
script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.join(script_dir, '..')
os.chdir(project_root)
print(f"Current directory: {os.getcwd()}")

# Run the build script with release flag
result = subprocess.run(['python', 'scripts/run.py', 'release'], check=False)

# Check if the build was successful
if result.returncode != 0:
    print("Build failed. Exiting...")
    os.chdir(cur_dir)
    exit(result.returncode)

# Set and create the release directory
release_dir = os.path.abspath(os.path.join('..', 'Tamerlane Chess'))
os.makedirs(release_dir, exist_ok=True)

# Copy the executable
shutil.copy2('build/main.exe', release_dir)

# Copy the assets folder
shutil.copytree('assets', os.path.join(release_dir, 'assets'), dirs_exist_ok=True)

# Copy the dependencies
deps_dir = os.path.join(script_dir, 'dependancies')
for item in os.listdir(deps_dir):
    src = os.path.join(deps_dir, item)
    dst = os.path.join(release_dir, item)
    if os.path.isdir(src):
        shutil.copytree(src, dst, dirs_exist_ok=True)
    else:
        shutil.copy2(src, dst)

print(f"Release files have been copied to {release_dir}")

# Return to the original directory
os.chdir(cur_dir)