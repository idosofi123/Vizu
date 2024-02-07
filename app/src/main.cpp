#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <VizuCore/SimulationMap.hpp>
#include "SoundProcessing.h"
#include <vector>

int main() {

    constexpr unsigned int FPS = 60;
    constexpr float SECONDS_PER_FRAME = 1.0f / FPS;

    auto window = sf::RenderWindow{ { 1280u, 720u }, "Vizu" };    

    // window.setFramerateLimit(144);

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("D:\\Users\\USER\\Downloads\\evree.wav")) {
        return -1;
    }

    auto frameAmplitudes = SoundProcessing::generateFrameAmplitudes(soundBuffer, FPS);
    auto maxamp = *std::max_element(frameAmplitudes.begin(), frameAmplitudes.end());

    std::vector<int> beatFrameIds; 

    sf::RectangleShape amplitudeIndicator;
    amplitudeIndicator.setFillColor(sf::Color::White);

    sf::Sound sound;
    
    int currentFrame = 0;
    
    sf::RenderTexture frameTexture;
    frameTexture.create(1280, 1080);

    sf::Font font;
    font.loadFromFile("D:\\Users\\USER\\Documents\\Wondershare\\Wondershare Filmora\\Download\\Fonts\\Roboto-regular.ttf");
    sf::Text text("", font, 24);

    float elapsedSeconds = 0;

    sound.setBuffer(soundBuffer);
    sound.play();

    sf::Clock clock;

    constexpr sf::Int32 PLATFORM_MIN_DELTA = 500000;
    int platcount = 0;

    while (window.isOpen()) {
        
        elapsedSeconds += clock.restart().asSeconds();

        while (elapsedSeconds >= SECONDS_PER_FRAME) {
            ++currentFrame;
            if (currentFrame < frameAmplitudes.size() && frameAmplitudes[currentFrame] - frameAmplitudes[currentFrame - 1] >= PLATFORM_MIN_DELTA) {
                amplitudeIndicator.setFillColor(sf::Color::Red);
                ++platcount;
            } else {
                amplitudeIndicator.setFillColor(sf::Color::White);
            }
            elapsedSeconds -= SECONDS_PER_FRAME;
        }

        window.clear(sf::Color{51, 171, 240, 255});

        if (currentFrame < frameAmplitudes.size()) {
            amplitudeIndicator.setSize({(static_cast<float>(frameAmplitudes[currentFrame]) / maxamp) * 1280, 20});
        }
        window.draw(amplitudeIndicator);

        text.setPosition({10, 30});
        text.setString("Channel count: " + std::to_string(soundBuffer.getChannelCount()));
        window.draw(text);

        text.setPosition({10, 50});
        text.setString("Platform min. delta: " + std::to_string(PLATFORM_MIN_DELTA));
        window.draw(text);

        text.setPosition({10, 70});
        text.setString("Platform count: " + std::to_string(platcount));
        window.draw(text);

        window.display();
    }
}