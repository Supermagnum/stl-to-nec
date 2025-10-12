#pragma once

#include <string>
#include <vector>
#include "geometry_utils.h"
#include "material_database.h"
#include "frequency_calculator.h"
#include "antenna_detector.h"

namespace stl_to_eznec {

struct NECWire {
    int tag;
    int segments;
    Point3D start;
    Point3D end;
    double radius;
    
    NECWire(int tag, int segments, const Point3D& start, const Point3D& end, double radius)
        : tag(tag), segments(segments), start(start), end(end), radius(radius) {}
};

struct NECLoad {
    int type;        // 0=series RLC, 1=parallel RLC, 4=impedance, 5=conductivity
    int wireTag;
    int segStart;
    int segEnd;
    double R, L, C;  // Resistance, Inductance, Capacitance
    
    NECLoad(int type, int wireTag, int segStart, int segEnd, double R, double L, double C)
        : type(type), wireTag(wireTag), segStart(segStart), segEnd(segEnd), R(R), L(L), C(C) {}
};

class NECGenerator {
public:
    NECGenerator();
    
    // Generate NEC file from STL data
    std::string generateNEC(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        const FrequencyCalculator& frequency,
        const AntennaWire& antenna,
        const std::string& modelName = "STL Model",
        bool hasAntenna = true,
        double waterlineHeight = 0.0,
        const WaterProperties* water = nullptr
    );
    
    // Generate NEC file without antenna
    std::string generateNECStructureOnly(
        const std::vector<Triangle>& triangles,
        const MaterialProperties& material,
        const std::string& modelName = "STL Model"
    );
    
    // Set output options
    void setIncludeComments(bool include) { includeComments_ = include; }
    void setIncludePattern(bool include) { includePattern_ = include; }
    void setIncludeCurrent(bool include) { includeCurrent_ = include; }
    
    // Get generated NEC content
    const std::string& getNECContent() const { return necContent_; }

private:
    std::string necContent_;
    bool includeComments_;
    bool includePattern_;
    bool includeCurrent_;
    
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
    
    // Generate current
    std::string generateCurrent();
    
    // Generate loads
    std::string generateLoads(const std::vector<NECLoad>& loads);
    
    // Helper functions
    std::vector<NECWire> triangulateToWires(const std::vector<Triangle>& triangles, const MaterialProperties& material);
    int calculateSegments(double length, double gridSpacing);
    std::string formatCoordinate(double value);
    std::string formatScientific(double value);
    std::string getMaterialComment(const MaterialProperties& material);
};

} // namespace stl_to_eznec


