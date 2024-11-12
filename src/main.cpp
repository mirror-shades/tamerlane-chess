// Copyright 2024. mirror-shades. GPL-2.0 License.
/**
 * Tamerlane Chess - A historical chess variant from medieval Persia
 *
 * This is the main entry point for the Tamerlane Chess game, an implementation of a medieval
 * chess variant featuring unique pieces and rules. The game uses SFML for graphics rendering
 * and window management.
 *
 * The game implements an AI opponent using minimax with alpha-beta pruning and piece-square
 * tables for evaluation. Players can play against the AI or another human player.
 *
 * SFML dependencies are included in the external folder and configured in CMakeLists.txt
 */

#include "game.h"

int main()
{
    Game game;
    game.run();
    return 0;
}
