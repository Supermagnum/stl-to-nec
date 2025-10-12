#include "ground_modeler.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace stl_to_eznec {

GroundModeler::GroundModeler() {
}

std::string GroundModeler::generateGroundCommand(const GroundParameters& params) {
    if (!validateGroundParameters(params)) {
        return "CM " + getValidationError(params) + "\n";
    }
    
    switch (params.type) {
        case GroundType::PERFECT_GROUND:
            return generatePerfectGround();
        case GroundType::SOMMERFELD_NORTON:
            return generateSommerfeldNortonGround(params);
        case GroundType::FINITE_GROUND_SCREEN:
            return generateFiniteGroundScreen(params);
        case GroundType::REAL_GROUND:
            return generateRealGround(params);
        case GroundType::WATER_GROUND:
            return generateWaterGround(params, nullptr);
        default:
            return generatePerfectGround();
    }
}

std::string GroundModeler::generateGroundCommand(const GroundParameters& params, const WaterProperties* water) {
    if (params.type == GroundType::WATER_GROUND && water != nullptr) {
        return generateWaterGround(params, water);
    }
    return generateGroundCommand(params);
}

GroundParameters GroundModeler::getGroundParameters(GroundType type) {
    GroundParameters params;
    params.type = type;
    
    switch (type) {
        case GroundType::PERFECT_GROUND:
            params.description = "Perfect ground (infinite conductivity)";
            break;
        case GroundType::SOMMERFELD_NORTON:
            params.conductivity = 0.01;  // 0.01 S/m (average soil)
            params.relativePermittivity = 13.0;
            params.description = "Sommerfeld-Norton ground (average soil)";
            break;
        case GroundType::FINITE_GROUND_SCREEN:
            params.conductivity = 1.0e7;  // 10 MS/m (copper)
            params.relativePermittivity = 1.0;
            params.screenRadius = 10.0;  // 10 meter radius
            params.description = "Finite ground screen (copper)";
            break;
        case GroundType::REAL_GROUND:
            params.conductivity = 0.01;  // 0.01 S/m (average soil)
            params.relativePermittivity = 13.0;
            params.description = "Real ground (average soil)";
            break;
        case GroundType::WATER_GROUND:
            params.conductivity = 4.5;  // 4.5 S/m (salt water)
            params.relativePermittivity = 81.0;
            params.description = "Water ground (salt water)";
            break;
    }
    
    return params;
}

GroundParameters GroundModeler::getGroundParameters(GroundType type, const WaterProperties* water) {
    GroundParameters params = getGroundParameters(type);
    
    if (type == GroundType::WATER_GROUND && water != nullptr) {
        params.conductivity = water->conductivity;
        params.relativePermittivity = water->relativePermittivity;
        params.description = "Water ground (" + water->type + ")";
    }
    
    return params;
}

void GroundModeler::printGroundTypes() {
    std::cout << "\n=== Ground Model Selection ===\n";
    std::cout << "1. Perfect Ground - Infinite conductivity, ideal for most cases\n";
    std::cout << "2. Sommerfeld-Norton - Advanced ground model with soil properties\n";
    std::cout << "3. Finite Ground Screen - Copper screen with specified radius\n";
    std::cout << "4. Real Ground - Average soil properties\n";
    std::cout << "5. Water Ground - Marine environment (salt/fresh water)\n\n";
}

std::string GroundModeler::getGroundDescription(GroundType type) {
    switch (type) {
        case GroundType::PERFECT_GROUND:
            return "Perfect ground (infinite conductivity)";
        case GroundType::SOMMERFELD_NORTON:
            return "Sommerfeld-Norton ground model";
        case GroundType::FINITE_GROUND_SCREEN:
            return "Finite ground screen";
        case GroundType::REAL_GROUND:
            return "Real ground with soil properties";
        case GroundType::WATER_GROUND:
            return "Water ground for marine applications";
        default:
            return "Unknown ground type";
    }
}

bool GroundModeler::validateGroundParameters(const GroundParameters& params) {
    if (params.type == GroundType::PERFECT_GROUND) {
        return true; // Perfect ground has no parameters to validate
    }
    
    if (!isValidConductivity(params.conductivity)) {
        return false;
    }
    
    if (!isValidPermittivity(params.relativePermittivity)) {
        return false;
    }
    
    if (params.type == GroundType::FINITE_GROUND_SCREEN && params.screenRadius <= 0) {
        return false;
    }
    
    return true;
}

std::string GroundModeler::getValidationError(const GroundParameters& params) {
    if (params.type == GroundType::PERFECT_GROUND) {
        return "";
    }
    
    if (!isValidConductivity(params.conductivity)) {
        return "Invalid conductivity value";
    }
    
    if (!isValidPermittivity(params.relativePermittivity)) {
        return "Invalid permittivity value";
    }
    
    if (params.type == GroundType::FINITE_GROUND_SCREEN && params.screenRadius <= 0) {
        return "Invalid screen radius";
    }
    
    return "";
}

std::string GroundModeler::generatePerfectGround() {
    return "GN -1\n";
}

std::string GroundModeler::generateSommerfeldNortonGround(const GroundParameters& params) {
    std::stringstream ss;
    ss << "GN 1 0 0 0 " << formatGroundPermittivity(params.relativePermittivity);
    ss << " " << formatGroundConductivity(params.conductivity) << "\n";
    return ss.str();
}

std::string GroundModeler::generateFiniteGroundScreen(const GroundParameters& params) {
    std::stringstream ss;
    ss << "GN 0 0 0 0 " << formatGroundPermittivity(params.relativePermittivity);
    ss << " " << formatGroundConductivity(params.conductivity) << "\n";
    ss << "GD 0.0 0.0 0.001 0.001 " << params.screenRadius << " " << params.screenRadius << "\n";
    return ss.str();
}

std::string GroundModeler::generateRealGround(const GroundParameters& params) {
    std::stringstream ss;
    ss << "GN 2 0 0 0 " << formatGroundPermittivity(params.relativePermittivity);
    ss << " " << formatGroundConductivity(params.conductivity) << "\n";
    return ss.str();
}

std::string GroundModeler::generateWaterGround(const GroundParameters& params, const WaterProperties* water) {
    std::stringstream ss;
    ss << "GN 2 0 0 0 " << formatGroundPermittivity(params.relativePermittivity);
    ss << " " << formatGroundConductivity(params.conductivity) << "\n";
    return ss.str();
}

std::string GroundModeler::formatGroundConductivity(double conductivity) {
    std::stringstream ss;
    ss << std::scientific << std::setprecision(2) << conductivity;
    return ss.str();
}

std::string GroundModeler::formatGroundPermittivity(double permittivity) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << permittivity;
    return ss.str();
}

bool GroundModeler::isValidConductivity(double conductivity) {
    return conductivity >= 0.0 && conductivity <= 1.0e8; // 0 to 100 MS/m
}

bool GroundModeler::isValidPermittivity(double permittivity) {
    return permittivity >= 1.0 && permittivity <= 100.0; // 1 to 100
}

} // namespace stl_to_eznec
