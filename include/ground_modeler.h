#pragma once

#include <string>
#include "material_database.h"

namespace stl_to_eznec {

enum class GroundType {
    PERFECT_GROUND,
    SOMMERFELD_NORTON,
    FINITE_GROUND_SCREEN,
    REAL_GROUND,
    WATER_GROUND
};

struct GroundParameters {
    GroundType type;
    double conductivity;      // S/m
    double relativePermittivity;
    double groundHeight;       // meters above ground
    double screenRadius;       // for finite ground screen
    std::string description;
    
    GroundParameters() : type(GroundType::PERFECT_GROUND), conductivity(0.0), 
                        relativePermittivity(1.0), groundHeight(0.0), screenRadius(0.0) {}
};

class GroundModeler {
public:
    GroundModeler();
    
    // Generate ground commands for NEC
    std::string generateGroundCommand(const GroundParameters& params);
    std::string generateGroundCommand(const GroundParameters& params, const WaterProperties* water);
    
    // Ground type selection
    GroundParameters getGroundParameters(GroundType type);
    GroundParameters getGroundParameters(GroundType type, const WaterProperties* water);
    
    // Ground type descriptions
    void printGroundTypes();
    std::string getGroundDescription(GroundType type);
    
    // Validation
    bool validateGroundParameters(const GroundParameters& params);
    std::string getValidationError(const GroundParameters& params);

private:
    // Ground type specific generators
    std::string generatePerfectGround();
    std::string generateSommerfeldNortonGround(const GroundParameters& params);
    std::string generateFiniteGroundScreen(const GroundParameters& params);
    std::string generateRealGround(const GroundParameters& params);
    std::string generateWaterGround(const GroundParameters& params, const WaterProperties* water);
    
    // Helper methods
    std::string formatGroundConductivity(double conductivity);
    std::string formatGroundPermittivity(double permittivity);
    bool isValidConductivity(double conductivity);
    bool isValidPermittivity(double permittivity);
};

} // namespace stl_to_eznec
