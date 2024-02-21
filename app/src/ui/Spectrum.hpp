#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Spectrum : public sf::Drawable {

    private:
        sf::Vector2f position;
        sf::Vector2f size;
        std::vector<sf::RectangleShape> bars;
        std::vector<std::vector<float>> frameData;
        float maxHeight = 0;

    public:
        Spectrum(sf::Vector2f position, sf::Vector2f size, int barCount, const std::vector<std::vector<float>> &frequencyWindows);
        ~Spectrum();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void setCurrentFrame(int frame);
};