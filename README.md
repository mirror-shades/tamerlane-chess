# Tamerlane Chess C++

I am building a Tamerlane chess engine in C++. This engine was previously built in Pygame but the performance made building a proper AI messy. This engine will let us better explore the strategies of this ancient chess varient.

## Compiling

using minGW 6.1.0 > 6.3.0, run in powershell:
`mingw32-make -f Makefile; if ($?) { ./build/main.exe }`
or run in terminal:
`mingw32-make -f Makefile && build\main.exe`

## todo

[ ] qol and bug fixes
[ ] proper highlighting for fortress squares
[ ] previous move highlighting
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
[x] implement menus  
[x] implement more menu features  
[x] implement AI
