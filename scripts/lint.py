# this is only set up for mirror-shades custom fork of cpplint
# this is not intended for distribution with the project

import os
import subprocess
import argparse

def lint_directory(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith((".h", ".cpp")):
                print(f"linting {file}")
                if(file == "ai.cpp"):
                    subprocess.run(["cpplint", "--filter=-whitespace/comments", os.path.join(root, file)])
                elif(file.endswith(".h")):
                    subprocess.run(["cpplint", "--filter=-whitespace/indent", os.path.join(root, file)])
                else:
                    subprocess.run(["cpplint", os.path.join(root, file)])

# Set up argument parser
parser = argparse.ArgumentParser(description='Lint C++ files in src and/or include directories')
parser.add_argument('-s', '--src', action='store_true', help='lint src directory')
parser.add_argument('-i', '--include', action='store_true', help='lint include directory')
args = parser.parse_args()

# If no arguments provided, lint both directories
if not args.src and not args.include:
    lint_directory("src")
    lint_directory("include")
else:
    if args.include:
        lint_directory("include")
    if args.src:
        lint_directory("src")
        print("chessboard.cpp linted with whitespace/line_length filter to avoid false positives")
        print("ai.cpp linted with whitespace/comments filter to avoid false positives")
