#include <vector>
#include <functional>

namespace Sound {

    std::vector<float> toMonoSignal(const std::vector<float> &signal, int channels) {
        std::vector<float> result(signal.size() / channels);

        for (size_t i = 0; i < signal.size(); ++i) {
            result[i / channels] += signal[i] / channels;
        }
        
        return result;
    }

    std::vector<std::vector<float>> windowSignal(const std::vector<float> &signal, const std::function<float(int, int)> &windowFunc) {

    }

    std::vector<float> dft(const std::vector<float> &signal) {

    }

    std::vector<unsigned long long> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold) {

    }

    std::vector<int> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps) {
        
    }
}