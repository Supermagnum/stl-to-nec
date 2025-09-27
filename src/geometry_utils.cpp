#include "geometry_utils.h"
#include <algorithm>
#include <cmath>

namespace stl_to_eznec {

BoundingBox GeometryUtils::calculateBoundingBox(const std::vector<Triangle>& triangles) {
    BoundingBox bbox;
    
    for (const auto& triangle : triangles) {
        for (const auto& vertex : triangle.vertices) {
            bbox.expand(vertex);
        }
    }
    
    return bbox;
}

double GeometryUtils::calculateTotalLength(const std::vector<Triangle>& triangles) {
    double totalLength = 0.0;
    
    for (const auto& triangle : triangles) {
        // Calculate perimeter of triangle
        double side1 = triangle.vertices[0].distance(triangle.vertices[1]);
        double side2 = triangle.vertices[1].distance(triangle.vertices[2]);
        double side3 = triangle.vertices[2].distance(triangle.vertices[0]);
        totalLength += side1 + side2 + side3;
    }
    
    return totalLength;
}

bool GeometryUtils::isWireLike(const std::vector<Triangle>& triangles, double maxDiameter) {
    if (triangles.empty()) return false;
    
    BoundingBox bbox = calculateBoundingBox(triangles);
    Point3D size = bbox.size();
    
    // Check if it's thin in two dimensions (wire-like)
    std::vector<double> dimensions = {size.x, size.y, size.z};
    std::sort(dimensions.begin(), dimensions.end());
    
    // Wire should be thin in two dimensions
    return (dimensions[0] <= maxDiameter && dimensions[1] <= maxDiameter);
}

std::vector<Point3D> GeometryUtils::extractWirePath(const std::vector<Triangle>& triangles) {
    std::vector<Point3D> path;
    
    if (triangles.empty()) return path;
    
    // For now, return the centers of triangles as a simple path
    // In a more sophisticated implementation, we would trace the actual wire path
    for (const auto& triangle : triangles) {
        path.push_back(triangle.center());
    }
    
    return path;
}

double GeometryUtils::calculateWireRadius(const std::vector<Triangle>& triangles) {
    if (triangles.empty()) return 0.0;
    
    // Calculate average distance from center to vertices
    Point3D center(0, 0, 0);
    int vertexCount = 0;
    
    for (const auto& triangle : triangles) {
        for (const auto& vertex : triangle.vertices) {
            center = center + vertex;
            vertexCount++;
        }
    }
    
    if (vertexCount == 0) return 0.0;
    
    center = center * (1.0 / vertexCount);
    
    double totalDistance = 0.0;
    for (const auto& triangle : triangles) {
        for (const auto& vertex : triangle.vertices) {
            totalDistance += center.distance(vertex);
        }
    }
    
    return totalDistance / vertexCount;
}

bool GeometryUtils::arePointsCoincident(const Point3D& p1, const Point3D& p2, double tolerance) {
    return p1.distance(p2) < tolerance;
}

std::vector<std::vector<Triangle>> GeometryUtils::separateConnectedComponents(const std::vector<Triangle>& triangles) {
    std::vector<std::vector<Triangle>> components;
    
    if (triangles.empty()) return components;
    
    // Simple implementation: each triangle is its own component
    // In a more sophisticated implementation, we would group connected triangles
    for (const auto& triangle : triangles) {
        components.push_back({triangle});
    }
    
    return components;
}

} // namespace stl_to_eznec
