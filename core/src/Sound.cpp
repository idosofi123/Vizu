#include <VizuCore/Sound.hpp>
#include <algorithm>
#include <numeric>
#include <numbers>
#include <map>
#include <cmath>

namespace Vizu {

    namespace Sound {

        constexpr float PI = std::numbers::pi;

        const std::map<WindowFunctionType, WinFunc> windowFunctions{
            {
                WindowFunctionType::Hann, [](size_t index, size_t length) {
                    return std::pow(std::sin((static_cast<float>(index) / length) * PI), 2);
                }
            },
            {
                WindowFunctionType::Identity, [](size_t index, size_t length) {
                    return 1.0f;
                }
            }
        };

        std::vector<float> toMonoSignal(const std::vector<float> &signal, int channels) {

            std::vector<float> result(signal.size() / channels);

            for (size_t i = 0; i < signal.size(); ++i) {
                result[i / channels] += signal[i] / channels;
            }
            
            return result;
        }

        std::vector<std::vector<float>> windowSignal(const std::vector<float> &signal, size_t windowSize, size_t hopSize, WindowFunctionType windowFunctionType) {


            size_t windows = (signal.size() >= windowSize) + std::max(signal.size() - windowSize, 0ULL) / hopSize;
            if (windows == 0) {
                return {};
            }

            std::vector<std::vector<float>> result(windows, std::vector<float>(windowSize));
            const auto &winFunc = windowFunctions.at(WindowFunctionType::Hann);

            for (size_t winI = 0; winI < windows; ++winI) {
                for (size_t i = 0; i < windowSize; ++i) {
                    result[winI][i] = signal[winI * hopSize + i] * winFunc(i, windowSize - 1);
                }
            }
            
            return result;
        }

        std::vector<float> dft(const std::vector<float> &signal) {

            const size_t N = signal.size();

            std::vector<std::complex<float>> dft(N);

            std::vector<std::complex<float>> unitSqrts(N);
            for (size_t i = 0; i < N; ++i) {
                unitSqrts[i] = std::polar(1.0f, -2.0f * PI * (static_cast<float>(i) / N));
            }

            for (size_t freqI = 0; freqI < N; ++freqI) {
                for (size_t i = 0; i < N; ++i) {
                    dft[freqI] += signal[i] * unitSqrts[(i * freqI) % N];
                }
            }

            std::vector<float> result(N);
            std::transform(dft.begin(), dft.end(), result.begin(), [](const auto &value) { return std::abs(value); });

            return result;
        }

        std::vector<std::complex<float>> fft(std::vector<float> signal) {

            if (signal.size() == 1) {
                return { signal[0] };
            }

            // Zero-pad the signal to reach a length which is a power of 2
            int msb = 0;
            while ((signal.size() >> msb) > 1) {
                ++msb;
            }

            if (signal.size() > (1 << msb)) {
                signal.resize(1 << (msb + 1));
            }

            const size_t N = signal.size();

            std::vector<float> evenSamples(N / 2), oddSamples(N / 2);
            for (size_t i = 0; i < N; i++) {
                if (i % 2 == 0) {
                    evenSamples[i / 2] = signal[i];
                } else {
                    oddSamples[i / 2] = signal[i];
                }
            }
            
            const auto evenFFT = fft(std::move(evenSamples));
            const auto oddFFT = fft(std::move(oddSamples));

            std::vector<std::complex<float>> result(N);

            for (size_t i = 0; i < N; i++) {
                result[i] = evenFFT[i % (N / 2)] + oddFFT[i % (N / 2)] * std::polar(1.0f, -2.0f * PI * (static_cast<float>(i) / N));
            }

            return result;
        }

        float flux(const std::vector<float> &dftCurr, const std::vector<float> &dftPrev) {

            float result{0.0f};

            // Address only the first half of the frequency bins, respecting the Nyquist frequency of the signal
            for (size_t i = 0; i < dftCurr.size() / 2; i++) {
                result += std::max(dftCurr[i] - dftPrev[i], 0.0f);
            }

            return result;
        }

        std::vector<size_t> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold) {

            std::vector<size_t> result;

            for (size_t i = 1; i < dftWindows.size(); i++) {
                if (flux(dftWindows[i], dftWindows[i - 1]) >= threshold) {
                    result.push_back(i);
                }
            }

            return result;
        }

        std::vector<size_t> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps) {

            audioSignal = toMonoSignal(audioSignal, channels);

            const size_t hopSize = static_cast<size_t>(sampleRate) / static_cast<size_t>(fps);

            // Currently hard-coding 50% overlap between windows, subject to change.
            const size_t windowSize = hopSize * 2;

            auto windowedSignal = windowSignal(audioSignal, windowSize, hopSize, WindowFunctionType::Hann);

            std::vector<std::vector<float>> dftWindows;
            for (const auto &window : windowedSignal) {
                auto fftOfWindow = fft(std::move(window));
                std::vector<float> absoluteFft;
                std::transform(fftOfWindow.begin(), fftOfWindow.end(), std::back_inserter(absoluteFft), [](auto &compVal) { return std::abs(compVal); });

                dftWindows.push_back(std::move(absoluteFft));
            }

            return detectOnsets(dftWindows, 300.0f);
        }
    }

}