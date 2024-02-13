#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <complex>
#include <numbers>

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

        const size_t N = signal.size();
        constexpr float PI = std::numbers::pi;

        std::vector<std::complex<float>> dft(N);

        for (size_t freqI = 0; freqI < N; ++freqI) {

            for (size_t i = 0; i < N; ++i) {
                
                dft[freqI] += signal[i] * std::polar(1.0f, -2.0f * PI * (static_cast<float>(i) / N) * freqI);
            }
        }

        std::vector<float> result;
        std::transform(dft.begin(), dft.end(), std::back_inserter(result), [](auto value) { return std::abs(value); });

        return result;
    }

    std::vector<unsigned long long> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold) {

    }

    std::vector<int> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps) {
        
    }
}