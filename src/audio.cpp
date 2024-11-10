#include "include/audio.h"
#include <SFML/Audio.hpp>
#include <iostream>

// Add these as member variables in the Utility class or as global variables
sf::SoundBuffer moveSoundBuffer;
sf::SoundBuffer captureSoundBuffer;
sf::Sound moveSound;
sf::Sound captureSound;

// In the Utility class constructor or initialization method
void Audio::initializeSounds()
{
    if (!moveSoundBuffer.loadFromFile("assets/audio/move.mp3"))
    {
        std::cerr << "Failed to load move sound" << std::endl;
    }
    if (!captureSoundBuffer.loadFromFile("assets/audio/capture.mp3"))
    {
        std::cerr << "Failed to load capture sound" << std::endl;
    }
    moveSound.setBuffer(moveSoundBuffer);
    captureSound.setBuffer(captureSoundBuffer);
}

void Audio::playMoveSound()
{
    if (moveSound.getStatus() != sf::Sound::Playing)
    {
        moveSound.play();
    }
}

void Audio::playCaptureSound()
{
    if (captureSound.getStatus() != sf::Sound::Playing)
    {
        captureSound.play();
    }
}
