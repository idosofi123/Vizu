#pragma once

#include "Object.hpp"
#include "Vector.hpp"

namespace Vizu {

    class Platform : public Object {

    private:
        float width;

    public:
        static constexpr float PLATFORM_HEIGHT = 10.0f;
        Platform(Vector<float> position, float width);
        const float& getWidth() const;
        ~Platform();
    };

}