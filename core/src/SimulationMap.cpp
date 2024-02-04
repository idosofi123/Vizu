#include "VizuCore/SimulationMap.hpp"

namespace Vizu {

    SimulationMap::SimulationMap(
        int frameRate,
        const std::vector<int> &beatFrameIds, // store this too as collisionframeids
        Vector<float> ballInitialPosition,
        float ballRadius,
        Vector<float> constantForce,
        float energyLossFactor) : 
            frameRate(frameRate),
            constantForce(constantForce),
            energyLossFactor(energyLossFactor),
            ball(ballInitialPosition, ballRadius) {

        for (int frameId : beatFrameIds) {
            
        }

        this->currentFrame = 0;
    }

    SimulationMap::~SimulationMap() {

    }

    void SimulationMap::advance(int frames) {

    }

    void SimulationMap::reset() {

    }
    
    const Ball& SimulationMap::getBall() const {

    }

    const std::vector<Platform>& SimulationMap::getPlatforms() const {

    }
}
