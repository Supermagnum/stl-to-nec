#include "antenna_detector.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

namespace stl_to_eznec {

AntennaDetector::AntennaDetector() 
    : maxWireDiameter_(0.01), minWireLength_(0.1), maxWireLength_(10.0) {
}

AntennaWire AntennaDetector::detectAntenna(const std::vector<Triangle>& triangles) {
    antenna_ = AntennaWire();
    
    if (triangles.empty()) {
        return antenna_;
    }
    
    // Find wire-like components
    std::vector<std::vector<Triangle>> components = findWireLikeComponents(triangles);
    
    // Look for the most likely antenna candidate
    for (const auto& component : components) {
        if (isWireLikeComponent(component)) {
            std::vector<Point3D> path = extractWirePath(component);
            double length = calculateWireLength(path);
            double radius = calculateWireRadius(component);
            
            if (isReasonableAntennaLength(length) && isReasonableAntennaRadius(radius)) {
                antenna_.triangles = component;
                antenna_.path = path;
                antenna_.radius = radius;
                antenna_.length = length;
                antenna_.isDetected = true;
                
                if (!path.empty()) {
                    antenna_.startPoint = path.front();
                    antenna_.endPoint = path.back();
                }
                break;
            }
        }
    }
    
    return antenna_;
}

std::vector<std::vector<Triangle>> AntennaDetector::findWireLikeComponents(const std::vector<Triangle>& triangles) {
    // For now, return all triangles as separate components
    // In a more sophisticated implementation, we would group connected triangles
    std::vector<std::vector<Triangle>> components;
    
    for (const auto& triangle : triangles) {
        components.push_back({triangle});
    }
    
    return components;
}

bool AntennaDetector::isWireLikeComponent(const std::vector<Triangle>& component) {
    if (component.empty()) return false;
    
    BoundingBox bbox = GeometryUtils::calculateBoundingBox(component);
    Point3D size = bbox.size();
    
    // Check if it's thin in two dimensions (wire-like)
    std::vector<double> dimensions = {size.x, size.y, size.z};
    std::sort(dimensions.begin(), dimensions.end());
    
    // Wire should be thin in two dimensions
    return (dimensions[0] <= maxWireDiameter_ && dimensions[1] <= maxWireDiameter_);
}

std::vector<Point3D> AntennaDetector::extractWirePath(const std::vector<Triangle>& component) {
    std::vector<Point3D> path;
    
    if (component.empty()) return path;
    
    // For now, return the centers of triangles as a simple path
    // In a more sophisticated implementation, we would trace the actual wire path
    for (const auto& triangle : component) {
        path.push_back(triangle.center());
    }
    
    return simplifyPath(path);
}

double AntennaDetector::calculateWireRadius(const std::vector<Triangle>& component) {
    if (component.empty()) return 0.0;
    
    // Calculate average distance from center to vertices
    Point3D center(0, 0, 0);
    int vertexCount = 0;
    
    for (const auto& triangle : component) {
        for (const auto& vertex : triangle.vertices) {
            center = center + vertex;
            vertexCount++;
        }
    }
    
    if (vertexCount == 0) return 0.0;
    
    center = center * (1.0 / vertexCount);
    
    double totalDistance = 0.0;
    for (const auto& triangle : component) {
        for (const auto& vertex : triangle.vertices) {
            totalDistance += center.distance(vertex);
        }
    }
    
    return totalDistance / vertexCount;
}

double AntennaDetector::calculateWireLength(const std::vector<Point3D>& path) {
    if (path.size() < 2) return 0.0;
    
    double totalLength = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        totalLength += path[i-1].distance(path[i]);
    }
    
    return totalLength;
}

bool AntennaDetector::isReasonableAntennaLength(double length) {
    return length >= minWireLength_ && length <= maxWireLength_;
}

bool AntennaDetector::isReasonableAntennaRadius(double radius) {
    return radius > 0.0 && radius <= 0.01; // 1cm max radius
}

std::vector<Point3D> AntennaDetector::simplifyPath(const std::vector<Point3D>& path, double tolerance) {
    if (path.size() <= 2) return path;
    
    std::vector<Point3D> simplified;
    simplified.push_back(path[0]);
    
    for (size_t i = 1; i < path.size() - 1; ++i) {
        if (path[i].distance(simplified.back()) > tolerance) {
            simplified.push_back(path[i]);
        }
    }
    
    if (path.size() > 1) {
        simplified.push_back(path.back());
    }
    
    return simplified;
}

void AntennaDetector::printAntennaInfo() const {
    if (!antenna_.isDetected) {
        std::cout << "No antenna wire detected in the model.\n";
        return;
    }
    
    std::cout << "\n=== Antenna Detection Results ===\n";
    std::cout << "Antenna wire detected!\n";
    std::cout << "Length: " << std::fixed << std::setprecision(3) << antenna_.length << " m (";
    std::cout << std::fixed << std::setprecision(1) << antenna_.length * 100.0 << " cm)\n";
    std::cout << "Radius: " << std::fixed << std::setprecision(3) << antenna_.radius << " m (";
    std::cout << std::fixed << std::setprecision(1) << antenna_.radius * 100.0 << " cm)\n";
    std::cout << "Start point: (" << std::fixed << std::setprecision(3) << antenna_.startPoint.x;
    std::cout << ", " << antenna_.startPoint.y << ", " << antenna_.startPoint.z << ")\n";
    std::cout << "End point: (" << std::fixed << std::setprecision(3) << antenna_.endPoint.x;
    std::cout << ", " << antenna_.endPoint.y << ", " << antenna_.endPoint.z << ")\n\n";
}

} // namespace stl_to_eznec
