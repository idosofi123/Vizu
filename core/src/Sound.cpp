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

        namespace {

            std::vector<std::complex<float>> fftRecursive(const std::vector<float> &signal, size_t spacing = 1, size_t offset = 0) {

                const size_t N = signal.size() / spacing;

                if (N == 1) {
                    return { signal[offset] };
                }
                
                const auto evenFFT = fftRecursive(signal, spacing * 2, offset);
                const auto oddFFT = fftRecursive(signal, spacing * 2, offset + spacing);

                std::vector<std::complex<float>> result(N);

                for (size_t i = 0; i < N; i++) {
                    result[i] = evenFFT[i % (N / 2)] + oddFFT[i % (N / 2)] * std::polar(1.0f, -2.0f * PI * (static_cast<float>(i) / N));
                }

                return result;
            }
        }

        std::vector<std::complex<float>> fft(std::vector<float> signal) {

            // Zero-pad the signal to reach a length which is a power of 2
            int msb = 0;
            while ((signal.size() >> msb) > 1) {
                ++msb;
            }

            if (signal.size() > (1 << msb)) {
                signal.resize(1 << (msb + 1));
            }

            return fftRecursive(signal);
        }

        float flux(const std::vector<float> &dftCurr, const std::vector<float> &dftPrev) {

            constexpr float EPSILON = 0.0001f;

            float result{0.0f};

            // Address only the first half of the frequency bins, respecting the Nyquist frequency of the signal
            for (size_t i = 0; i < dftCurr.size() / 2; i++) {
                result += std::max((dftCurr[i] / std::max(dftPrev[i], EPSILON)) - 1.0f, 0.0f);
            }

            return result;
        }

        std::vector<size_t> detectOnsets(const std::vector<std::vector<float>> &dftWindows, float threshold) {

            std::vector<size_t> result;
            float currFlux{0}, prevFlux{0};

            for (size_t i = 1; i < dftWindows.size(); i++) {
                currFlux = flux(dftWindows[i], dftWindows[i - 1]);
                if (currFlux >= threshold && prevFlux < threshold) {
                    result.push_back(i);
                }
                prevFlux = currFlux;
            }

            return result;
        }

        std::vector<size_t> detectedOnsetFrames(std::vector<float> audioSignal, int sampleRate, int channels, int fps) {

            audioSignal = toMonoSignal(audioSignal, channels);

            const size_t hopSize = static_cast<size_t>(sampleRate) / static_cast<size_t>(fps);

            // Currently hard-coding 75% overlap between windows, subject to change.
            const size_t windowSize = hopSize * 4;

            auto windowedSignal = windowSignal(audioSignal, windowSize, hopSize, WindowFunctionType::Hann);

            std::vector<std::vector<float>> dftWindows;
            for (const auto &window : windowedSignal) {
                auto fftOfWindow = fft(std::move(window));
                std::vector<float> absoluteFft(fftOfWindow.size());
                std::transform(fftOfWindow.begin(), fftOfWindow.end(), absoluteFft.begin(), [](auto &compVal) { return std::abs(compVal); });

                dftWindows.push_back(std::move(absoluteFft));
            }

            size_t freqBins = dftWindows[0].size();
            return detectOnsets(dftWindows, 0.35f * freqBins);
        }
    }

}