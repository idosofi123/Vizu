#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace Configuration {

    struct RunConfiguration {

        unsigned int fps;
        float onsetDetectionThreshold;
        float ballRadius;
        float gravitationalForce;
        float ballHorizontalVelocity;
        float energyLossFactor;
        unsigned int ballTrailLength;
        unsigned int antialiasingLevel;
        std::string soundFilePath;
        unsigned int windowWidth;
        unsigned int windowHeight;
        
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(
            RunConfiguration,
            fps,
            onsetDetectionThreshold,
            ballRadius,
            gravitationalForce,
            ballHorizontalVelocity,
            energyLossFactor,
            ballTrailLength,
            antialiasingLevel,
            soundFilePath,
            windowWidth,
            windowHeight
        )
    };

    RunConfiguration loadConfiguration();
}