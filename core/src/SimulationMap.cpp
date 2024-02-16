#include "VizuCore/SimulationMap.hpp"

namespace Vizu {

    SimulationMap::SimulationMap(
        int frameRate,
        std::vector<size_t> beatFrameIds,
        Vector<float> ballInitialPosition,
        float ballRadius,
        float gravitationalForce,
        float ballMovementSpeed,
        float energyLossFactor) : 
            frameRate(frameRate),
            gravitationalForce(gravitationalForce),
            ballMovementSpeed(ballMovementSpeed),
            energyLossFactor(energyLossFactor),
            ballInitialPosition(ballInitialPosition),
            ball(ballInitialPosition, ballRadius),
            beatFrameIds(std::move(beatFrameIds)) {

        constexpr float ENDS_PLATFORM_PADDING = 20.0f;

        std::vector<std::pair<Vector<float>, Vector<float>>> ballStates;

        this->ball.addVelocity({ballMovementSpeed, gravitationalForce});

        for (int frameId : this->beatFrameIds) {
            this->advance(frameId - this->currentFrame);
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
                ballPos.x - newPosition.x + (ballStates[i + 1].first.x - ballPos.x) / 2;

            if (ballVel.y < 0) {
                newWidth = std::min(
                    newWidth,
                    ballPos.x - newPosition.x + framesToNextCollision(ballVel.y, gravitationalForce) * ballVel.x - this->ball.getRadius()
                );
            }

            this->platforms.push_back({newPosition, newWidth});
        }
    
        this->reset();
    }

    void SimulationMap::advance(int frames) {

        this->ball.advance(frames, this->gravitationalForce);
        this->currentFrame += frames;

        if (this->nextCollisionIndex < this->beatFrameIds.size() && this->beatFrameIds[this->nextCollisionIndex] == this->currentFrame) {
            this->ball.multVelocity({1, -1 * this->energyLossFactor});
            ++this->nextCollisionIndex;
        }
    }
    
    void SimulationMap::reset() {
        this->currentFrame = 0;
        this->nextCollisionIndex = 0;
        this->ball.setPosition(this->ballInitialPosition);
        this->ball.multVelocity({0, 0});
        this->ball.addVelocity({this->ballMovementSpeed, this->gravitationalForce});
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
