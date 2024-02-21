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
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = config.antialiasingLevel;

    sf::RenderWindow window{ { config.windowWidth, config.windowHeight }, "Vizu", sf::Style::Default, settings };

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(config.soundFilePath)) {
        std::cout << "Failed to load audio file" << std::endl;
        return -1;
    }

    std::vector<float> signal(soundBuffer.getSampleCount());

    // Normalize signal values to [-1, 1]
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
    constexpr float SPECTRUM_HEIGHT = 150;
    constexpr float SPECTRUM_BAR_WIDTH = 2;

    Spectrum spectrum{
        {50, static_cast<int>(config.windowHeight) - SPECTRUM_HEIGHT},
        {SPECTRUM_BARS * SPECTRUM_BAR_WIDTH, SPECTRUM_HEIGHT},
        SPECTRUM_BARS,
        frequencyFrames
    };

    sf::View camera;
    camera.setSize(static_cast<sf::Vector2f>(window.getSize()));


    const float SECONDS_PER_FRAME = 1.0f / config.fps;
    int currentFrame = 0;
    float elapsedSeconds = 0;

    sf::Sound sound;
    sound.setBuffer(soundBuffer);
    sound.play();

    sf::Clock clock;

    while (window.isOpen()) {
        
        elapsedSeconds += clock.restart().asSeconds();

        // Update simulation state in accordance to the time passed since the last update tick
        while (elapsedSeconds >= SECONDS_PER_FRAME) {

            ballTrail.push_back(simulationMap.getBall().getPosition());
            if (ballTrail.size() > config.ballTrailLength) {
                ballTrail.pop_front();
            }

            simulationMap.advance();
            ++currentFrame;
            elapsedSeconds -= SECONDS_PER_FRAME;
        }

        // Handle window events
        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(20, 108, 148));
        
        // Set the camera to point at the ball's center point
        const auto& [ballX, ballY] = simulationMap.getBall().getPosition();
        camera.setCenter(ballX + config.ballRadius / 2, ballY + config.ballRadius / 2);
        window.setView(camera);

        // Draw the note platforms
        for (const auto &platform : simulationMap.getPlatforms()) {

            const auto &[platformX, platformY] = platform.getPosition();
            platformTexture.setPosition(platformX, platformY);
            platformTexture.setSize({platform.getWidth(),  Vizu::Platform::PLATFORM_HEIGHT});
            platformTexture.setFillColor(platform.getFrameId() <= currentFrame ? sf::Color(25, 167, 206) : sf::Color::White);
            window.draw(platformTexture);
        }

        // Draw the trail of the ball based on previous states
        for (size_t i = 0; i < ballTrail.size(); i++) {
            ballTexture.setPosition(ballTrail[i].x, ballTrail[i].y);
            ballTexture.setFillColor(sf::Color(255, 255, 255, (static_cast<float>(i + 1) / ballTrail.size()) * 100));
            window.draw(ballTexture);
        }

        // Draw the actual ball
        ballTexture.setPosition(ballX, ballY);
        ballTexture.setFillColor(sf::Color::White);
        window.draw(ballTexture);

        // Draw UI elements
        window.setView(window.getDefaultView());
        spectrum.setCurrentFrame(std::min<int>(currentFrame, frequencyFrames.size() - 1));
        window.draw(spectrum);

        window.display();
    }
}