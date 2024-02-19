#include "Configuration.hpp"
#include <fstream>

using json = nlohmann::json;

namespace Configuration {

    RunConfiguration loadConfiguration() {

        std::ifstream fileStream("config.json");

        if (fileStream.fail()) {
            throw std::runtime_error("Configuration file not found");
        }

        json data = json::parse(fileStream);
        return data.get<RunConfiguration>();
    }
}