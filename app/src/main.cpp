#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <VizuCore/SimulationMap.hpp>
#include <VizuCore/Sound.hpp>
#include "ui/Spectrum.hpp"
#include "config/Configuration.hpp"
#include <vector>
#include <deque>
#include <iostream>

int main() {

    Configuration::RunConfiguration config;
    try {
        config = Configuration::loadConfiguration();
    } catch (std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
        return -1;
    }
    
    const float SECONDS_PER_FRAME = 1.0f / config.fps;

    sf::ContextSettings settings;
    settings.antialiasingLevel = config.antialiasingLevel;

    sf::RenderWindow window{ { config.windowWidth, config.windowHeight }, "Vizu", sf::Style::Default, settings };

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(config.soundFilePath)) {
        return -1;
    }

    std::vector<float> signal(soundBuffer.getSampleCount());
    for (size_t i = 0; i < signal.size(); i++) {
        signal[i] = static_cast<float>(soundBuffer.getSamples()[i]) / INT16_MAX;
    }

    auto frequencyFrames = Vizu::Sound::generateFrequencyFrames(signal, soundBuffer.getSampleRate(), soundBuffer.getChannelCount(), config.fps);
    auto keyFrameIds = Vizu::Sound::detectOnsets(frequencyFrames, config.onsetDetectionThreshold);

    Vizu::SimulationMap simulationMap(
        config.fps,
        keyFrameIds,
        {100, 100},
        config.ballRadius,
        config.gravitationalForce,
        config.ballHorizontalVelocity,
        config.energyLossFactor
    );

    sf::CircleShape ballTexture(config.ballRadius);
    ballTexture.setFillColor(sf::Color::White);

    std::deque<Vizu::Vector<float>> ballTrail;

    sf::RectangleShape platformTexture;

    constexpr size_t SPECTRUM_BARS = 150;
    const size_t BINS_IN_BAR = (frequencyFrames[0].size() / 2) / SPECTRUM_BARS + !!((frequencyFrames[0].size() / 2) % SPECTRUM_BARS);

    std::vector<std::vector<float>> condensedWindows(frequencyFrames.size(), std::vector<float>(SPECTRUM_BARS));
    for (size_t i = 0; i < frequencyFrames.size(); i++) {
        for (size_t j = 0; j < frequencyFrames[i].size() / 2; j++) {

            // TODO: Handle last bar's avg calculation accurately
            condensedWindows[i][j / BINS_IN_BAR] += frequencyFrames[i][j] / BINS_IN_BAR;
        }
    }

    float maxAmplitude = 0.0f;
    for (const auto &window : condensedWindows) {
        maxAmplitude = std::max(maxAmplitude, *std::max_element(window.begin(), window.begin() + window.size() / 2));
    }

    Spectrum spectrum{
        {50, static_cast<float>(static_cast<int>(config.windowHeight) - 120)},
        {SPECTRUM_BARS * 2.0f, 120},
        SPECTRUM_BARS, 
        maxAmplitude
    };

    sf::View camera;
    camera.setSize(static_cast<sf::Vector2f>(window.getSize()));

    sf::Sound sound;
    sound.setBuffer(soundBuffer);
    sound.play();

    int currentFrame = 0;
    float elapsedSeconds = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        
        elapsedSeconds += clock.restart().asSeconds();

        while (elapsedSeconds >= SECONDS_PER_FRAME) {

            ballTrail.push_back(simulationMap.getBall().getPosition());
            if (ballTrail.size() > config.ballTrailLength) {
                ballTrail.pop_front();
            }

            simulationMap.advance();
            ++currentFrame;
            elapsedSeconds -= SECONDS_PER_FRAME;
        }

        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(20, 108, 148));
        
        const auto& [ballX, ballY] = simulationMap.getBall().getPosition();
        camera.setCenter(ballX + config.ballRadius / 2, ballY + config.ballRadius / 2);
        window.setView(camera);

        for (const auto &platform : simulationMap.getPlatforms()) {

            const auto &[platformX, platformY] = platform.getPosition();
            platformTexture.setPosition(platformX, platformY);
            platformTexture.setSize({platform.getWidth(),  Vizu::Platform::PLATFORM_HEIGHT});
            platformTexture.setFillColor(platform.getFrameId() <= currentFrame ? sf::Color(25, 167, 206) : sf::Color::White);
            window.draw(platformTexture);
        }

        for (size_t i = 0; i < ballTrail.size(); i++) {
            ballTexture.setPosition(ballTrail[i].x, ballTrail[i].y);
            ballTexture.setFillColor(sf::Color(255, 255, 255, (static_cast<float>(i + 1) / ballTrail.size()) * 100));
            window.draw(ballTexture);
        }

        ballTexture.setPosition(ballX, ballY);
        ballTexture.setFillColor(sf::Color::White);
        window.draw(ballTexture);

        window.setView(window.getDefaultView());
        spectrum.setBarHeights(condensedWindows[std::min(static_cast<size_t>(currentFrame), condensedWindows.size() - 1)]);
        window.draw(spectrum);

        window.display();
    }
}