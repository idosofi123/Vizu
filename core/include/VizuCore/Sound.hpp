#pragma once
#include <vector>
#include <functional>
#include <complex>

namespace Vizu {

    namespace Sound {

        using WinFunc = std::function<float(size_t, size_t)>;
        enum class WindowFunctionType {
            Hann
        };

        std::vector<float> toMonoSignal(const std::vector<float> &signal, int channels);
        std::vector<std::vector<float>> windowSignal(const std::vector<float> &signal, size_t windowSize, size_t hopSize, WindowFunctionType windowFunctionType);
        std::vector<float> dft(const std::vector<float> &signal);
        std::vector<std::complex<float>> fft(std::vector<float> signal);
        std::vector<unsigned long long> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold);

        // Eventually this will be the only public function, leaving the others public now for testing purposes
        std::vector<int> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps);
    }
}