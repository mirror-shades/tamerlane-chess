# Tamerlane Chess C++

[![Windows](https://img.shields.io/badge/GitHub-Download-blue?style=flat-square&logo=github)](https://github.com/mirror-shades/tamerlane-cpp/releases/latest/download/Tamerlane.Chess-Windows.zip)
[![Linux](https://img.shields.io/badge/GitHub-Download-blue?style=flat-square&logo=github)](https://github.com/mirror-shades/tamerlane-cpp/releases/latest/download/Tamerlane-Chess-Linux.zip)

## ^ Download the latest release and play

An engine for Tamerlane chess built in C++. Tamerlane chess is a medieval chess variant from the Timurid Empire. Like modern chess, it is derived from shatranj. It was developed in Central Asia during the reign of Emperor Timur, and its invention is also attributed to him.

Rules and additional information can be [found here](https://www.chessvariants.com/historic.dir/tamerlane.html)

## Compiling

The game is compiled using SFML 2.6.1 built on minGW 13.1.0 using CMake. SFML is included with the source code and no SFML installation should be necessary. You can build as normal using CMake or use the included python script. Currently Windows, Linux, and MacOS builds are supported (macOS is untested).

There is a script to build and run the game in the scripts directory called `build.py`. This will use cmake to build the project based on your platform.

This will build to the build directory in root.

```
python scripts/build.py
```

To run the game after building, use the following command:

```
python scripts/build.py run
```

To install a release of the game, use the following command:

```
python scripts/build.py install [path]
```

## todo

[ ] ai thinking blocks input for exiting the game  
[ ] ai thinking should happen after ai options menu is closed

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
[x] capture piece list  
[x] play move and capture sounds
[x] init cmake
[x] multiplatform build system
[x] render made reactive, huge performance boost

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
