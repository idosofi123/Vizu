#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Spectrum : public sf::Drawable {

    private:
        sf::Vector2f position;
        sf::Vector2f size;
        std::vector<sf::RectangleShape> bars;
        float maxHeight;

    public:
        Spectrum(sf::Vector2f position, sf::Vector2f size, int barCount, float maxHeight);
        ~Spectrum();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void setBarHeights(const std::vector<float> &heights);
};