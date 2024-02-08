#include <SFML/Audio.hpp>
#include <vector>
#include "SoundProcessing.h"

namespace SoundProcessing {

    std::vector<sf::Int32> generateFrameAmplitudes(const sf::SoundBuffer &soundBuffer, const int &fps) {

        auto channelCount = soundBuffer.getChannelCount();
        auto sampleCount = soundBuffer.getSampleCount() / channelCount;

        sf::Int32 samplesPerFrame = soundBuffer.getSampleRate() / fps;

        std::vector<sf::Int32> frameAmplitudes(sampleCount / samplesPerFrame + !!(sampleCount % samplesPerFrame));
        auto samples = soundBuffer.getSamples();

        for (sf::Uint64 i = 0; i < sampleCount; ++i) {

            sf::Int32 monoSample{0};
            for (unsigned int j = 0; j < channelCount; j++) {
                monoSample += (static_cast<sf::Int32>(samples[i * channelCount + j]) * samples[i * channelCount + j]) / channelCount;
            }

            frameAmplitudes[i / samplesPerFrame] += monoSample / samplesPerFrame;
        }

        return frameAmplitudes;
    }

    std::vector<int> generateKeyFrameIds(const std::vector<sf::Int32> &frameAmplitudes) {

        constexpr sf::Int32 PLATFORM_MIN_DELTA = 750000;

        std::vector<int> keyFrames{};
        for (sf::Uint64 i = 1; i < frameAmplitudes.size(); ++i) {
            if (frameAmplitudes[i] - frameAmplitudes[i - 1] > PLATFORM_MIN_DELTA) {
                keyFrames.push_back(i);
            }
        }

        return keyFrames;
    }
}
