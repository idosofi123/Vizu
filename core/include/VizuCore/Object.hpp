#pragma once
#include "Vector.hpp"

namespace Vizu {

    class Object {

    protected:
        Vector<float> position;

    public:
        Object();
        Object(Vector<float> position);
        const Vector<float>& getPosition() const;
        void setPosition(const Vector<float>& newPosition);
        ~Object();

    };
}