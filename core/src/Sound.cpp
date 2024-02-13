#include <vector>
#include <functional>
#include <algorithm>

namespace Sound {

    std::vector<float> toMonoSignal(const std::vector<float> &signal, int channels) {

        std::vector<float> result(signal.size() / channels);

        for (size_t i = 0; i < signal.size(); ++i) {
            result[i / channels] += signal[i] / channels;
        }
        
        return result;
    }

    std::vector<std::vector<float>> windowSignal(const std::vector<float> &signal, size_t windowSize, size_t hopSize, const std::function<float(size_t, size_t)> &windowFunc) {


        size_t windows = (signal.size() >= windowSize) + std::max(signal.size() - windowSize, 0ULL) / hopSize;
        if (windows == 0) {
            return {};
        }

        std::vector<std::vector<float>> result(windows, std::vector<float>(windowSize));

        for (size_t winI = 0; winI < windows; ++winI) {
            for (size_t i = 0; i < windowSize; ++i) {
                result[winI][i] = signal[winI * hopSize + i] * windowFunc(i, windowSize);
            }
        }
        
        return result;
    }

    std::vector<float> dft(const std::vector<float> &signal) {

    }

    std::vector<unsigned long long> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold) {

    }

    std::vector<int> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps) {
        
    }
}