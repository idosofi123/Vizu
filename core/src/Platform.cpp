#include "VizuCore/Platform.hpp"
#include "VizuCore/Vector.hpp"

namespace Vizu {

    Platform::Platform(Vector<float> position, float width) : Object(position), width(width) {

    }

    Platform::~Platform() {

    }

    const float& Platform::getWidth() const {
        return this->width;
    }
}