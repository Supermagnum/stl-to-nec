#pragma once

#include <vector>
#include <string>
#include "geometry_utils.h"

namespace stl_to_eznec {

struct AntennaWire {
    std::vector<Triangle> triangles;
    std::vector<Point3D> path;
    double radius;
    double length;
    Point3D startPoint;
    Point3D endPoint;
    bool isDetected;
    
    AntennaWire() : radius(0), length(0), isDetected(false) {}
};

class AntennaDetector {
public:
    AntennaDetector();
    
    // Detect antenna wire from STL triangles
    AntennaWire detectAntenna(const std::vector<Triangle>& triangles);
    
    // Check if antenna was detected
    bool isAntennaDetected() const { return antenna_.isDetected; }
    
    // Get detected antenna
    const AntennaWire& getAntenna() const { return antenna_; }
    
    // Get antenna length in meters
    double getAntennaLength() const { return antenna_.length; }
    
    // Get antenna radius in meters
    double getAntennaRadius() const { return antenna_.radius; }
    
    // Get antenna path points
    const std::vector<Point3D>& getAntennaPath() const { return antenna_.path; }
    
    // Get antenna start and end points
    Point3D getStartPoint() const { return antenna_.startPoint; }
    Point3D getEndPoint() const { return antenna_.endPoint; }
    
    // Print antenna information
    void printAntennaInfo() const;
    
    // Set detection parameters
    void setMaxWireDiameter(double diameter) { maxWireDiameter_ = diameter; }
    void setMinWireLength(double length) { minWireLength_ = length; }
    void setMaxWireLength(double length) { maxWireLength_ = length; }
    
    // Get detection parameters
    double getMaxWireDiameter() const { return maxWireDiameter_; }
    double getMinWireLength() const { return minWireLength_; }
    double getMaxWireLength() const { return maxWireLength_; }

private:
    AntennaWire antenna_;
    double maxWireDiameter_;  // Maximum diameter to consider as wire (default 1cm)
    double minWireLength_;    // Minimum length to consider as antenna (default 10cm)
    double maxWireLength_;    // Maximum length to consider as antenna (default 10m)
    
    // Detection algorithms
    std::vector<std::vector<Triangle>> findWireLikeComponents(const std::vector<Triangle>& triangles);
    bool isWireLikeComponent(const std::vector<Triangle>& component);
    std::vector<Point3D> extractWirePath(const std::vector<Triangle>& component);
    double calculateWireRadius(const std::vector<Triangle>& component);
    double calculateWireLength(const std::vector<Point3D>& path);
    bool isReasonableAntennaLength(double length);
    bool isReasonableAntennaRadius(double radius);
    
    // Helper functions
    std::vector<std::vector<Triangle>> separateConnectedComponents(const std::vector<Triangle>& triangles);
    bool areTrianglesConnected(const Triangle& t1, const Triangle& t2, double tolerance = 1e-6);
    std::vector<Point3D> simplifyPath(const std::vector<Point3D>& path, double tolerance = 1e-3);
};

} // namespace stl_to_eznec
