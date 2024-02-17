#include "VizuCore/Platform.hpp"
#include "VizuCore/Vector.hpp"

namespace Vizu {

    Platform::Platform(Vector<float> position, float width, int frameId) : Object(position), width(width), frameId(frameId) {

    }

    Platform::~Platform() {

    }

    const float& Platform::getWidth() const {
        return this->width;
    }

    const int& Platform::getFrameId() const {
        return this->frameId;
    }
}