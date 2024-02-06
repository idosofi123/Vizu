#pragma once
#include "VizuCore/Object.hpp"
#include "VizuCore/Vector.hpp"

namespace Vizu {

    class Ball : public Object {

    private:
        Vector<float> velocity;
        float radius;

    public:
        Ball(Vector<float> initialPosition, float radius);
        void addVelocity(const Vector<float> &force);
        void multVelocity(const Vector<float> &scalar);
        const Vector<float>& getVelocity() const;
        void advance(int frames = 1, const float &gravitationalForce = 0.0f);
        const float& getRadius() const;
        ~Ball();

    };

}