#pragma once

namespace Vizu {

    template <typename T>
    struct Vector {
        T x;
        T y;

        Vector& operator+=(const Vector<T> &val) {
            this->x += val.x;
            this->y += val.y;
            return *this;
        }

        Vector& operator*=(const Vector<T> &val) {
            this->x += val.x;
            this->y += val.y;
            return *this;
        }

        Vector& operator=(const Vector<T> &val) {
            this->x = val.x;
            this->y = val.y;
            return *this;
        }
    };
} 