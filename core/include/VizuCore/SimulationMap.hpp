#pragma once

#include "Platform.hpp"
#include "Vector.hpp"
#include "Ball.hpp"
#include <vector>

namespace Vizu {

    class SimulationMap {

    private:
        int frameRate;
        std::vector<Platform> platforms;
        Ball ball;
        int nextPlatformIndex = 0;
        int currentFrame = 0;
        Vector<float> constantForce;
        float energyLossFactor;

    public:
        SimulationMap(
            int frameRate,
            const std::vector<int> &beatFrameIds,
            Vector<float> ballInitialPosition,
            float ballRadius,
            Vector<float> constantForce,
            float energyLossFactor);
        ~SimulationMap();
        void advance(int frames = 1);
        void reset();
        const Ball& getBall() const;
        const std::vector<Platform>& getPlatforms() const;
    };
}
