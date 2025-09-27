#pragma once

#include <string>
#include <vector>
#include "geometry_utils.h"
#include "material_database.h"
#include "frequency_calculator.h"
#include "antenna_detector.h"

namespace stl_to_eznec {

class EZGenerator {
public:
    EZGenerator();
    
    // Generate EZ file from STL data
    std::string generateEZ(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        const FrequencyCalculator& frequency,
        const AntennaWire& antenna,
        const std::string& modelName = "STL Model",
        bool hasAntenna = true,
        double waterlineHeight = 0.0,
        const WaterProperties* water = nullptr
    );
    
    // Generate EZ file without antenna
    std::string generateEZStructureOnly(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        const std::string& modelName = "STL Model"
    );
    
    // Set output options
    void setIncludeComments(bool include) { includeComments_ = include; }
    void setIncludePattern(bool include) { includePattern_ = include; }
    
    // Get generated EZ content
    const std::string& getEZContent() const { return ezContent_; }

private:
    std::string ezContent_;
    bool includeComments_;
    bool includePattern_;
    
    // Generate file header
    std::string generateHeader(const std::string& modelName, const FrequencyCalculator& frequency);
    
    // Generate geometry section
    std::string generateGeometry(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        const AntennaWire& antenna,
        bool hasAntenna
    );
    
    // Generate wire definitions
    std::string generateWires(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        const AntennaWire& antenna,
        bool hasAntenna
    );
    
    // Generate antenna wire
    std::string generateAntennaWire(const AntennaWire& antenna, int& wireTag);
    
    // Generate structure wires
    std::string generateStructureWires(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        int& wireTag
    );
    
    // Generate excitation
    std::string generateExcitation(const AntennaWire& antenna);
    
    // Generate ground
    std::string generateGround(double waterlineHeight, const WaterProperties* water);
    
    // Generate frequency
    std::string generateFrequency(const FrequencyCalculator& frequency);
    
    // Generate pattern
    std::string generatePattern();
    
    // Helper functions
    int calculateSegments(double length, double gridSpacing);
    std::string formatCoordinate(double value);
    std::string formatScientific(double value);
    std::string getMaterialComment(const MaterialProperties& material);
    
    // EZ-specific formatting
    std::string formatEZCoordinate(double value);
    std::string formatEZScientific(double value);
};

} // namespace stl_to_eznec
