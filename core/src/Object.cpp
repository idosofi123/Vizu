#include "VizuCore/Object.hpp"
#include "VizuCore/Vector.hpp"

namespace Vizu {

    Object::Object() : Object({0.0f, 0.0f}) { }

    Object::Object(Vector<float> position) : position(position) { }

    Object::~Object() { }

    const Vector<float>& Object::getPosition() const {
        return this->position;
    }

    void Object::setPosition(const Vector<float>& newPosition) {
        this->position = newPosition;
    }
}