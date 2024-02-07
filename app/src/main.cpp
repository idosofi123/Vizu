#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <VizuCore/SimulationMap.hpp>
#include <vector>

int main() {

    constexpr unsigned int FPS = 60;

    window.setFramerateLimit(FPS);
    
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
    sound.setBuffer(soundBuffer);
    sound.play();
    
    int currentFrame = 0;
    
    sf::RenderTexture frameTexture;
    frameTexture.create(1280, 1080);

    while (window.isOpen()) {

        for (auto event = sf::Event{}; window.pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color{51, 171, 240, 255});

        if (currentFrame < frameAmplitudes.size()) {
            amplitudeIndicator.setSize({(static_cast<float>(frameAmplitudes[currentFrame]) / maxamp) * 1280, 20});
        }
        window.draw(amplitudeIndicator);

        window.display();
        ++currentFrame;
    }
}