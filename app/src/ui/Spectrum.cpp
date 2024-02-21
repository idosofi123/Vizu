#include "Spectrum.hpp"

Spectrum::Spectrum(sf::Vector2f position, sf::Vector2f size, int barCount, const std::vector<std::vector<float>> &frequencyWindows)
    : position(position), size(size) {

    const size_t BINS_IN_BAR = (frequencyWindows[0].size() / 2) / barCount + !!((frequencyWindows[0].size() / 2) % barCount);
    
    this->frameData.resize(frequencyWindows.size(), std::vector<float>(barCount));

    for (size_t i = 0; i < frequencyWindows.size(); i++) {
        for (size_t j = 0; j < frequencyWindows[i].size() / 2; j++) {

            // TODO: Handle last bar's avg calculation accurately
            this->frameData[i][j / BINS_IN_BAR] += frequencyWindows[i][j] / BINS_IN_BAR;
        }
    }

    for (const auto &window : this->frameData) {
        this->maxHeight = std::max(this->maxHeight, *std::max_element(window.begin(), window.begin() + window.size() / 2));
    }

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
        target.draw(bar, states);
    }
}

void Spectrum::setCurrentFrame(int currentFrame) {

    for (size_t i = 0; i < this->bars.size(); i++) {
        
        this->bars[i].setSize({ 
            this->size.x / this->bars.size(),
            (this->frameData[currentFrame][i] / this->maxHeight) * this->size.y 
        });

        this->bars[i].setPosition({
            this->position.x + (this->size.x / this->bars.size()) * i,
            this->position.y + this->size.y - this->bars[i].getSize().y
        });
    }
}
