#pragma once

#include "Platform.hpp"
#include "Vector.hpp"
#include "Ball.hpp"
#include <vector>

namespace Vizu {

    class SimulationMap {

    private:
        int frameRate;
        std::vector<int> beatFrameIds;
        std::vector<Platform> platforms;
        Vector<float> ballInitialPosition;
        Ball ball;
        int nextCollisionIndex = 0;
        int currentFrame = 0;
        float gravitationalForce;
        float ballMovementSpeed;
        float energyLossFactor;

        inline static float framesToNextCollision(const float &currentVelocity, const float &gravitationalForce);

    public:
        SimulationMap(
            int frameRate,
            std::vector<int> beatFrameIds,
            Vector<float> ballInitialPosition,
            float ballRadius,
            float gravitationalForce,
            float ballMovementSpeed,
            float energyLossFactor);
        void advance(int frames = 1);
        const Ball& getBall() const;
        const std::vector<Platform>& getPlatforms() const;
        void reset();
        ~SimulationMap();
    };
}
