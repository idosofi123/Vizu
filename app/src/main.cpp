#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <VizuCore/SimulationMap.hpp>
#include <VizuCore/Sound.hpp>
#include "SoundProcessing.h"
#include <vector>

int main() {

    constexpr unsigned int FPS = 60;
    constexpr float SECONDS_PER_FRAME = 1.0f / FPS;

    auto window = sf::RenderWindow{ { 1280u, 720u }, "Vizu" };

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("D:\\Users\\USER\\Documents\\Audacity\\440.wav")) {
        return -1;
    }

    auto frameAmplitudes = SoundProcessing::generateFrameAmplitudes(soundBuffer, FPS);
    auto keyFrameIds = SoundProcessing::generateKeyFrameIds(frameAmplitudes);

    auto maxamp = *std::max_element(frameAmplitudes.begin(), frameAmplitudes.end());

    std::vector<int> beatFrameIds; 
    Vizu::SimulationMap simulationMap(FPS, keyFrameIds, {100, 100}, 10, 0.15f, 3.5f, 0.65f);

    sf::RectangleShape amplitudeIndicator;
    amplitudeIndicator.setFillColor(sf::Color::White);

    sf::Sound sound;
    
    int currentFrame = 0;
    
    sf::RenderTexture frameTexture;
    frameTexture.create(1280, 1080);

    sf::Font font;
    font.loadFromFile("D:\\Users\\USER\\Documents\\Wondershare\\Wondershare Filmora\\Download\\Fonts\\Roboto-regular.ttf");
    sf::Text text("", font, 24);

    sf::CircleShape ballTexture(10);
    ballTexture.setFillColor(sf::Color::Green);

    sf::RectangleShape platformTexture;
    platformTexture.setFillColor(sf::Color::Magenta);
    platformTexture.setOutlineThickness(-1);
    platformTexture.setOutlineColor(sf::Color::Black);

    sf::View camera;
    camera.setSize(static_cast<sf::Vector2f>(window.getSize()));

    float elapsedSeconds = 0;

    std::vector<float> signal(soundBuffer.getSampleCount());
    for (size_t i = 0; i < signal.size(); i++) {
        signal[i] = static_cast<float>(soundBuffer.getSamples()[i]) / INT16_MAX;
    }
    
    signal = Vizu::Sound::toMonoSignal(signal, soundBuffer.getChannelCount());
    auto fft = Vizu::Sound::fft(signal);

    std::vector<float> fftAbs;
    std::transform(fft.begin(), fft.end(), std::back_inserter(fftAbs), [](auto complexValue) { return std::abs(complexValue); });

    // sound.setBuffer(soundBuffer);
    // sound.play();

    sf::Clock clock;

    while (window.isOpen()) {
        
        // elapsedSeconds += clock.restart().asSeconds();

        // while (elapsedSeconds >= SECONDS_PER_FRAME) {
        //     simulationMap.advance();
        //     ++currentFrame;
        //     elapsedSeconds -= SECONDS_PER_FRAME;
        // }

        window.clear(sf::Color{51, 171, 240, 255});

        const int bw = 200, pw = 10;
        int offset = 0;
        for (size_t i = 0; i < fftAbs.size() / 2; i += bw) {

            auto bar = sf::RectangleShape{};
            float curramp = 0;

            for (size_t j = 0; j < bw; ++j) {
                curramp += fftAbs[i + j] / bw;
            }    

            bar.setPosition(offset + 50, 50);
            bar.setSize({pw, curramp});
            offset += pw + 5;

            window.draw(bar);
        }

        // if (currentFrame < frameAmplitudes.size()) {
        //     amplitudeIndicator.setSize({(static_cast<float>(frameAmplitudes[currentFrame]) / maxamp) * 1280, 20});
        // }
        // window.draw(amplitudeIndicator);

        // // text.setPosition({10, 30});
        // // text.setString("Channel count: " + std::to_string(soundBuffer.getChannelCount()));
        // // window.draw(text);

        // // text.setPosition({10, 50});
        // // text.setString("Platform min. delta: " + std::to_string(PLATFORM_MIN_DELTA));
        // // window.draw(text);

        // // text.setPosition({10, 70});
        // // text.setString("Platform count: " + std::to_string(platcount));
        // // window.draw(text);

        // const auto& [ballX, ballY] = simulationMap.getBall().getPosition();

        // for (const auto &platform : simulationMap.getPlatforms()) {

        //     const auto &[platformX, platformY] = platform.getPosition();
        //     platformTexture.setPosition(platformX, platformY);
        //     platformTexture.setSize({platform.getWidth(),  Vizu::Platform::PLATFORM_HEIGHT});
        //     window.draw(platformTexture);
        // }

        // ballTexture.setPosition(ballX, ballY);
        // camera.setCenter(ballX + 5, ballY + 5);
        // window.setView(camera);

        // window.draw(ballTexture);

        

        window.display();
    }
}