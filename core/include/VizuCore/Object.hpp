#pragma once

namespace Vizu {

    class Object {

    private:
        Vector<float> position;

    public:
        Object();
        Object(Vector<float> position);
        const Vector<float>& getPosition() const;
        void setPosition(const Vector<float>& newPosition);
        ~Object();

    };
}