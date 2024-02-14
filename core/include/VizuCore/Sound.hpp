#pragma once
#include <vector>
#include <functional>

namespace Vizu {
    namespace Sound {
        
        std::vector<float> toMonoSignal(const std::vector<float> &signal, int channels);
        std::vector<std::vector<float>> windowSignal(const std::vector<float> &signal, size_t windowSize, size_t hopSize, const std::function<float(size_t, size_t)> &windowFunc);
        std::vector<float> dft(const std::vector<float> &signal);
        std::vector<unsigned long long> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold);

        // Eventually this will be the only public function, leaving the others public now for testing purposes
        std::vector<int> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps);
    }
}