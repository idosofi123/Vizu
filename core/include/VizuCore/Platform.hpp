#pragma once

#include "Object.hpp"

namespace Vizu {

    class Platform : public Object {

    private:
        int width;

    public:
        static const int PLATFORM_HEIGHT = 10;
        Platform(Vector<float> position, int width);
        ~Platform();

    };

}