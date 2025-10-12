#pragma once

#include <string>
#include <vector>
#include "user_interface.h"

namespace stl_to_eznec {

struct ValidationResult {
    bool isValid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    
    ValidationResult() : isValid(true) {}
    
    void addError(const std::string& error) {
        errors.push_back(error);
        isValid = false;
    }
    
    void addWarning(const std::string& warning) {
        warnings.push_back(warning);
    }
    
    bool hasErrors() const { return !errors.empty(); }
    bool hasWarnings() const { return !warnings.empty(); }
    
    std::string getSummary() const;
};

class Validator {
public:
    Validator();
    
    // Input validation
    ValidationResult validateSTLFile(const std::string& filename);
    ValidationResult validateMaterial(const MaterialProperties& material);
    ValidationResult validateFrequency(double frequency);
    ValidationResult validateVehicleType(VehicleType type);
    ValidationResult validateWaterlineHeight(double height);
    ValidationResult validateWaterProperties(const WaterProperties* water);
    
    // Complete input validation
    ValidationResult validateUserInput(const UserInput& input);
    
    // File validation
    ValidationResult validateSTLContent(const std::vector<Triangle>& triangles);
    ValidationResult validateOutputFiles(const std::string& necFile, const std::string& ezFile);
    
    // Geometry validation
    ValidationResult validateGeometry(const std::vector<Triangle>& triangles);
    ValidationResult validateAntenna(const AntennaWire& antenna);
    
    // Utility methods
    static bool fileExists(const std::string& filename);
    static bool isSTLFile(const std::string& filename);
    static bool isValidFrequency(double frequency);
    static bool isValidMaterial(const MaterialProperties& material);
    static bool isValidWaterlineHeight(double height);
    
    // Error message formatting
    static std::string formatValidationErrors(const ValidationResult& result);
    static std::string formatValidationWarnings(const ValidationResult& result);

private:
    // Internal validation methods
    bool validateSTLHeader(const std::string& filename);
    bool validateSTLBinary(const std::string& filename);
    bool validateSTLASCII(const std::string& filename);
    bool validateTriangleCount(size_t count);
    bool validateBoundingBox(const BoundingBox& bbox);
    bool validateWireGeometry(const AntennaWire& antenna);
    
    // Error message generation
    std::string generateSTLError(const std::string& filename);
    std::string generateMaterialError(const MaterialProperties& material);
    std::string generateFrequencyError(double frequency);
    std::string generateGeometryError(const std::vector<Triangle>& triangles);
};

} // namespace stl_to_eznec
