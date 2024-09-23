# Tamerlane Chess C++

[![GitHub](https://img.shields.io/badge/GitHub-Download-blue?style=flat-square&logo=github)](https://github.com/mirror-shades/tamerlane-cpp/releases/latest/download/Tamerlane.Chess.zip)

## ^ Download the latest release and play

An engine for Tamerlane chess built in C++. Tamerlane chess is a medieval chess variant from the Timurid Empire. Like modern chess, it is derived from shatranj. It was developed in Central Asia during the reign of Emperor Timur, and its invention is also attributed to him.

## Compiling

The game is compiled using SFML 2.6.1 built on minGW 13.1.0. All dependancies are included with the source code and no dependancy installation should be necessary. Currently there is only a Windows build.

There is a batch file to compile the game in the root directory called `run.bat`.

Alternatively, run in powershell:  
`mingw32-make -f Makefile; if ($?) { ./build/main.exe }`  
or run in terminal:  
`mingw32-make -f Makefile && build\main.exe`

## Installing

If you prefer to manually install rather than download a release, you can do that as well. There is a powershell script to install the game in the scripts directory called `install.ps1`. This script will create a standalone installation in a folder parallel to the source code. This standalone can also be downloaded from the releases page.

```
parent directory
|-- tamerlane-cpp (source code)
|-- Tamerlane Chess (Standalone installation)
```

## todo

[ ] qol and bug fixes  
[ ] ai thinking blocks input for exiting the game  
[ ] ai thinking should happen after ai options menu is closed  
[ ] capture piece list  

analysis mode:  
[ ] move by move history  
[ ] best move analysis  
[ ] move accuracy evaluation  

## done

[x] init SFML  
[x] draw board  
[x] implement clicking  
[x] draw pieces  
[x] piece highlighting  
[x] implement menus  
[x] implement expanded menu features  
[x] build and release  
[x] install script  
[x] proper highlighting for fortress squares regarding draws  
[x] previous move highlighting  
[x] AI menu, select which side to play as and difficulty

implement moves:  
[x] pawn moves  
[x] rook moves  
[x] Taliah moves  
[x] Elephant moves  
[x] Camel moves  
[x] Mongol moves  
[x] Khan moves  
[x] Adminstrator moves  
[x] Vizir moves  
[x] Giraffe moves  
[x] War Engine moves

implement game logic:  
[x] checks  
[x] checkmates  
[x] draws  
[x] promotions  
[x] turn history  
[x] implement AI
