#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <VizuCore/SimulationMap.hpp>
#include <vector>
#include <iostream>

int main() {

    constexpr unsigned int FPS = 60;
    constexpr float SECONDS_PER_FRAME = 1.0f / FPS;

    auto window = sf::RenderWindow{ { 1280u, 720u }, "Vizu" };    

    // window.setFramerateLimit(144);

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("D:\\Users\\USER\\Downloads\\ellinia.wav")) {
        return -1;
    }

    auto sampleCount = soundBuffer.getSampleCount();
    auto chan = soundBuffer.getChannelCount();

    sf::Int16 samplesPerFrame = soundBuffer.getSampleRate() / FPS;
    std::vector<sf::Int16> frameAmplitudes(sampleCount / samplesPerFrame + !!(sampleCount % samplesPerFrame));
    auto samples = soundBuffer.getSamples();

    sf::Int16 maxamp = 0;

    for (sf::Uint64 i = 0; i < sampleCount; ++i) {
        frameAmplitudes[i / samplesPerFrame] = std::max(
            frameAmplitudes[i / samplesPerFrame],
            static_cast<sf::Int16>(std::abs(samples[i]))
        );

        maxamp = std::max(maxamp, frameAmplitudes[i / samplesPerFrame]);
    }

    std::vector<int> beatFrameIds; 

    sf::RectangleShape amplitudeIndicator;
    amplitudeIndicator.setFillColor(sf::Color::Red);

    sf::Sound sound;
    
    int currentFrame = 0;
    
    sf::RenderTexture frameTexture;
    frameTexture.create(1280, 1080);

    sf::Font font;
    font.loadFromFile("D:\\Users\\USER\\Documents\\Wondershare\\Wondershare Filmora\\Download\\Fonts\\Roboto-regular.ttf");
    sf::Text text("", font, 24);
    text.setPosition({10, 30});

    float elapsedSeconds = 0;

    sound.setBuffer(soundBuffer);
    sound.play();

    sf::Clock clock;


    while (window.isOpen()) {
        
        elapsedSeconds += clock.restart().asSeconds();

        while (elapsedSeconds >= SECONDS_PER_FRAME) {
            ++currentFrame;
            elapsedSeconds -= SECONDS_PER_FRAME;
        }

        window.clear(sf::Color{51, 171, 240, 255});

        if (currentFrame < frameAmplitudes.size()) {
            amplitudeIndicator.setSize({(static_cast<float>(frameAmplitudes[currentFrame]) / maxamp) * 1280, 20});
        }
        window.draw(amplitudeIndicator);

        text.setString(std::to_string(currentFrame));
        window.draw(text);

        window.display();
    }
}