#include "VizuCore/SimulationMap.hpp"

namespace Vizu {

    SimulationMap::SimulationMap(
        int frameRate,
        std::vector<int> beatFrameIds,
        Vector<float> ballInitialPosition,
        float ballRadius,
        float gravitationalForce,
        float ballMovementSpeed,
        float energyLossFactor) : 
            frameRate(frameRate),
            gravitationalForce(gravitationalForce),
            ballMovementSpeed(ballMovementSpeed),
            energyLossFactor(energyLossFactor),
            ball(ballInitialPosition, ballRadius),
            beatFrameIds(std::move(beatFrameIds)) {

        constexpr float ENDS_PLATFORM_PADDING = 15.0f;

        std::vector<std::pair<Vector<float>, Vector<float>>> ballStates;

        this->ball.addVelocity({ballMovementSpeed, 0});

        for (int frameId : beatFrameIds) {
            this->advance(frameId - this->currentFrame);
            this->ball.multVelocity({1, -energyLossFactor});
            ballStates.push_back({this->ball.getPosition(), this->ball.getVelocity()});
        }

        for (int i = 0; i < ballStates.size(); ++i) {

            const auto& [ballPos, ballVel] = ballStates[i];

            Vector<float> newPosition;
            float newWidth;

            newPosition.x = (i > 0) ?
                this->platforms[i - 1].getPosition().x + this->platforms[i - 1].getWidth() :
                ballPos.x - ENDS_PLATFORM_PADDING;

            newPosition.y = (ballVel.y < 0) ?
                ballPos.y + this->ball.getRadius() * 2 :
                ballPos.y - Platform::PLATFORM_HEIGHT;

            newWidth = (i == ballStates.size() - 1) ?
                ballPos.x - newPosition.x + ENDS_PLATFORM_PADDING :
                (ballStates[i + 1].first.x - newPosition.x) / 2;

            if (ballVel.y < 0) {
                newWidth = std::min(newWidth, framesToNextCollision(ballVel.y, gravitationalForce) * ballVel.x - this->ball.getRadius());
            }

            this->platforms.push_back({newPosition, newWidth});
        }
        
        this->currentFrame = 0;
        this->ball.setPosition(ballInitialPosition);
        this->ball.multVelocity({1, 0});
    }

    void SimulationMap::advance(int frames) {
        this->ball.advance(frames, this->gravitationalForce);
        this->currentFrame += frames;
    }
    
    const Ball& SimulationMap::getBall() const {
        return this->ball;
    }

    const std::vector<Platform>& SimulationMap::getPlatforms() const {
        return this->platforms;
    }

    inline float SimulationMap::framesToNextCollision(const float &currentVelocity, const float &gravitationalForce) {
        return 1.0f - (2.0f * currentVelocity / gravitationalForce);
    }

    SimulationMap::~SimulationMap() {

    }
}
