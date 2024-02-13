#pragma once

#include <SFML/Audio.hpp>
#include <vector>

namespace SoundProcessing {
    
    std::vector<sf::Int32> generateFrameAmplitudes(const sf::SoundBuffer &soundBuffer, const int &FPS);
    std::vector<int> generateKeyFrameIds(const std::vector<sf::Int32> &frameAmplitudes);
}