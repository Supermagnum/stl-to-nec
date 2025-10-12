#include "geometry_utils.h"
#include <algorithm>
#include <cmath>
#include <map>

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

std::vector<Point3D> GeometryUtils::extractWirePathAdvanced(const std::vector<Triangle>& triangles) {
    std::vector<Point3D> path;
    
    if (triangles.empty()) return path;
    
    // Find wire endpoints first
    std::vector<Point3D> endpoints = findWireEndpoints(triangles);
    
    if (endpoints.size() >= 2) {
        // Use endpoints to guide path extraction
        path = endpoints;
    } else {
        // Fall back to center-based extraction
        path = extractWirePath(triangles);
    }
    
    // Simplify the path
    path = simplifyWirePath(path);
    
    return path;
}

std::vector<Point3D> GeometryUtils::simplifyWirePath(const std::vector<Point3D>& path, double tolerance) {
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

double GeometryUtils::calculateWireLength(const std::vector<Point3D>& path) {
    if (path.size() < 2) return 0.0;
    
    double totalLength = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        totalLength += path[i-1].distance(path[i]);
    }
    
    return totalLength;
}

bool GeometryUtils::isReasonableWireGeometry(const std::vector<Triangle>& triangles) {
    if (triangles.empty()) return false;
    
    BoundingBox bbox = calculateBoundingBox(triangles);
    Point3D size = bbox.size();
    
    // Check aspect ratio
    double aspectRatio = calculateWireAspectRatio(triangles);
    if (aspectRatio < 5.0) return false; // Not wire-like enough
    
    // Check dimensions
    std::vector<double> dimensions = {size.x, size.y, size.z};
    std::sort(dimensions.begin(), dimensions.end());
    
    // Wire should be thin in two dimensions
    return (dimensions[0] <= 0.01 && dimensions[1] <= 0.01);
}

std::vector<Point3D> GeometryUtils::findWireEndpoints(const std::vector<Triangle>& triangles) {
    std::vector<Point3D> endpoints;
    
    if (triangles.empty()) return endpoints;
    
    // Find vertices that appear only once (endpoints)
    std::map<Point3D, int> vertexCount;
    
    for (const auto& triangle : triangles) {
        for (const auto& vertex : triangle.vertices) {
            vertexCount[vertex]++;
        }
    }
    
    // Find vertices that appear only once
    for (const auto& pair : vertexCount) {
        if (pair.second == 1) {
            endpoints.push_back(pair.first);
        }
    }
    
    return endpoints;
}

double GeometryUtils::calculateWireAspectRatio(const std::vector<Triangle>& triangles) {
    if (triangles.empty()) return 0.0;
    
    BoundingBox bbox = calculateBoundingBox(triangles);
    Point3D size = bbox.size();
    
    std::vector<double> dimensions = {size.x, size.y, size.z};
    std::sort(dimensions.begin(), dimensions.end());
    
    if (dimensions[0] == 0) return 0.0;
    
    return dimensions[2] / dimensions[0]; // Length / width
}

std::vector<Point3D> GeometryUtils::interpolateWirePath(const std::vector<Point3D>& path, int segments) {
    if (path.size() < 2) return path;
    
    std::vector<Point3D> interpolated;
    
    for (size_t i = 0; i < path.size() - 1; ++i) {
        const Point3D& start = path[i];
        const Point3D& end = path[i + 1];
        
        interpolated.push_back(start);
        
        for (int j = 1; j < segments; ++j) {
            double t = static_cast<double>(j) / segments;
            Point3D interpolatedPoint = start + (end - start) * t;
            interpolated.push_back(interpolatedPoint);
        }
    }
    
    interpolated.push_back(path.back());
    
    return interpolated;
}

} // namespace stl_to_eznec


