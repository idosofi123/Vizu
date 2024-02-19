#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <VizuCore/SimulationMap.hpp>
#include <VizuCore/Sound.hpp>
#include <vector>
#include <deque>
#include "ui/Spectrum.hpp"

int main() {

    constexpr unsigned int FPS = 60;
    constexpr float SECONDS_PER_FRAME = 1.0f / FPS;
    constexpr float ONSET_DETECTION_THRESHOLD = 0.35f;
    constexpr float BALL_RADIUS = 10.0f;
    constexpr float GRAVITATIONAL_FORCE = 0.15f;
    constexpr float BALL_HORIZONTAL_VEL = 4.0f;
    constexpr float ENERGY_LOSS_FACTOR = 0.7f;
    constexpr int BALL_TRAIL_LENGTH = 10;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;

    sf::RenderWindow window{ { 1280u, 720u }, "Vizu", sf::Style::Default, settings };

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("D:\\Users\\USER\\Downloads\\test.wav")) {
        return -1;
    }

    std::vector<float> signal(soundBuffer.getSampleCount());
    for (size_t i = 0; i < signal.size(); i++) {
        signal[i] = static_cast<float>(soundBuffer.getSamples()[i]) / INT16_MAX;
    }

    auto keyFrameIds = Vizu::Sound::detectedOnsetFrames(signal, soundBuffer.getSampleRate(), soundBuffer.getChannelCount(), FPS, ONSET_DETECTION_THRESHOLD);
    Vizu::SimulationMap simulationMap(FPS, keyFrameIds, {100, 100}, BALL_RADIUS, GRAVITATIONAL_FORCE, BALL_HORIZONTAL_VEL, ENERGY_LOSS_FACTOR);

    sf::Font font;
    font.loadFromFile("D:\\Users\\USER\\Documents\\Wondershare\\Wondershare Filmora\\Download\\Fonts\\Roboto-regular.ttf");

    sf::CircleShape ballTexture(BALL_RADIUS);
    ballTexture.setFillColor(sf::Color::White);

    std::deque<Vizu::Vector<float>> ballTrail;

    sf::RectangleShape platformTexture;
    platformTexture.setOutlineThickness(-1);
    platformTexture.setOutlineColor(sf::Color::Cyan);

    Spectrum spectrum({1280 - 200, 720 - 100}, {150, 100}, 15, 100.0f);

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
            if (ballTrail.size() > BALL_TRAIL_LENGTH) {
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

        window.clear(sf::Color{0, 4, 53, 255});
        
        const auto& [ballX, ballY] = simulationMap.getBall().getPosition();
        camera.setCenter(ballX + BALL_RADIUS / 2, ballY + BALL_RADIUS / 2);
        window.setView(camera);

        for (const auto &platform : simulationMap.getPlatforms()) {

            const auto &[platformX, platformY] = platform.getPosition();
            platformTexture.setPosition(platformX, platformY);
            platformTexture.setSize({platform.getWidth(),  Vizu::Platform::PLATFORM_HEIGHT});
            platformTexture.setFillColor(platform.getFrameId() <= currentFrame ? sf::Color::Magenta : sf::Color::Yellow);
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
        spectrum.setBarHeights({50,100,50,100,50,100,50,100,50,100,50,100,50,100,50});
        window.draw(spectrum);

        window.display();
    }
}