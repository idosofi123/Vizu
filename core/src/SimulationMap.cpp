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

        std::vector<std::tuple<Vector<float>, Vector<float>, int>> ballStates;

        this->ball.addVelocity({ballMovementSpeed, gravitationalForce});

        for (int frameId : this->beatFrameIds) {
            this->advance(frameId - this->currentFrame);
            ballStates.push_back({this->ball.getPosition(), this->ball.getVelocity(), frameId});
        }

        for (int i = 0; i < ballStates.size(); ++i) {

            const auto& [ballPos, ballVel, frameId] = ballStates[i];

            Vector<float> newPosition;
            float newWidth;

            newPosition.x = (i > 0) ?
                this->platforms[i - 1].getPosition().x + this->platforms[i - 1].getWidth() :
                ballPos.x - ENDS_PLATFORM_PADDING;

            // Prevent floor platforms going in the ball's upward path
            if (i > 0 && ballVel.y < 0) {

                const auto& [prevBallPos, prevBallVel, prevFrame] = ballStates[i - 1];

                if (prevBallPos.y > ballPos.y) {
                    newPosition.x = std::max(
                        newPosition.x,
                        prevBallPos.x + framesToYDelta(prevBallVel.y, gravitationalForce, ballPos.y - prevBallPos.y) * ballVel.x + ballRadius
                    );
                }
            }

            newPosition.y = (ballVel.y < 0) ?
                ballPos.y + this->ball.getRadius() * 2 :
                ballPos.y - Platform::PLATFORM_HEIGHT;

            newWidth = (i == ballStates.size() - 1) ?
                ballPos.x - newPosition.x + ENDS_PLATFORM_PADDING :
                ballPos.x - newPosition.x + (std::get<0>(ballStates[i + 1]).x - ballPos.x) / 2;

            if (ballVel.y < 0) {
                newWidth = std::min(
                    newWidth,
                    ballPos.x - newPosition.x + framesToYDelta(ballVel.y, gravitationalForce, 0) * ballVel.x - ballRadius
                );
            }

            this->platforms.push_back({newPosition, newWidth, frameId});
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

    inline float SimulationMap::framesToYDelta(float currentVelocity, float gravitationalForce, float delta) {
        return (gravitationalForce - 2 * currentVelocity + sqrtf(powf(2 * currentVelocity - gravitationalForce, 2) + 8 * delta * gravitationalForce) * (delta == 0 ? 1 : -1)) / (2 * gravitationalForce);
    }

    SimulationMap::~SimulationMap() {

    }
}
