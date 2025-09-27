#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "geometry_utils.h"

namespace stl_to_eznec {

class STLParser {
public:
    STLParser();
    ~STLParser() = default;
    
    // Load STL file (both ASCII and binary formats)
    bool loadFile(const std::string& filename);
    
    // Get parsed triangles
    const std::vector<Triangle>& getTriangles() const { return triangles_; }
    
    // Get bounding box
    BoundingBox getBoundingBox() const;
    
    // Get total surface area
    double getTotalArea() const;
    
    // Check if file was loaded successfully
    bool isLoaded() const { return loaded_; }
    
    // Get error message if loading failed
    const std::string& getErrorMessage() const { return errorMessage_; }
    
    // Scale the model to specified dimensions
    void scaleToLength(double targetLength);
    void scaleToLength(double targetLength, const std::string& axis); // "x", "y", or "z"
    
    // Get current scale factor
    double getScaleFactor() const { return scaleFactor_; }
    
    // Get original bounding box (before scaling)
    BoundingBox getOriginalBoundingBox() const { return originalBoundingBox_; }

private:
    std::vector<Triangle> triangles_;
    BoundingBox originalBoundingBox_;
    double scaleFactor_;
    bool loaded_;
    std::string errorMessage_;
    
    // Parse ASCII STL
    bool parseASCII(const std::string& content);
    
    // Parse binary STL
    bool parseBinary(const std::vector<uint8_t>& data);
    
    // Helper functions
    bool isASCII(const std::string& content);
    void calculateBoundingBox();
    void applyScaling();
};

} // namespace stl_to_eznec
