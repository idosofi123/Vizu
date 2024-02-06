#include "VizuCore/Ball.hpp"
#include "VizuCore/Object.hpp"

namespace Vizu {

    Ball::Ball(Vector<float> initialPosition, float radius) : Object(initialPosition), radius(radius), velocity({0.0f, 0.0f}) {

    }

    void Ball::addVelocity(const Vector<float> &force) {
        this->velocity += force;
    }

    const Vector<float>& Ball::getVelocity() const {
        return this->velocity;
    }

    void Ball::advance(int frames, const float &gravitationalForce) {

    }

    const float& Ball::getRadius() const {
        return this->radius;
    }

    void Ball::multVelocity(const Vector<float> &scalar) {
        this->velocity *= scalar;
    }


    Ball::~Ball() {

    }
}