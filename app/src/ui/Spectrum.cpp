#include "Spectrum.hpp"

Spectrum::Spectrum(sf::Vector2f position, sf::Vector2f size, int barCount, float maxHeight) : position(position), size(size), maxHeight(maxHeight) {

    for (size_t i = 0; i < barCount; i++) {
        sf::RectangleShape newBar;
        newBar.setFillColor(sf::Color(255, 255, 255));
        this->bars.push_back(newBar);
    }
    
}

Spectrum::~Spectrum() {

}

void Spectrum::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    
    for (const auto &bar : this->bars) {
        target.draw(bar);
    }
}

void Spectrum::setBarHeights(const std::vector<float>& heights) {

    for (size_t i = 0; i < this->bars.size(); i++) {
        
        this->bars[i].setSize({ 
            this->size.x / this->bars.size(),
            (heights[i] / this->maxHeight) * this->size.y 
        });

        this->bars[i].setPosition({
            this->position.x + (this->size.x / this->bars.size()) * i,
            this->position.y + this->size.y - this->bars[i].getSize().y
        });
    }
}
