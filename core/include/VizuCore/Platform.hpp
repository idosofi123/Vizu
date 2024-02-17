#pragma once

#include "Object.hpp"
#include "Vector.hpp"

namespace Vizu {

    class Platform : public Object {

    private:
        float width;
        int frameId;

    public:
        static constexpr float PLATFORM_HEIGHT = 10.0f;
        Platform(Vector<float> position, float width, int frameId);
        const float& getWidth() const;
        const int& getFrameId() const;
        ~Platform();
    };

}