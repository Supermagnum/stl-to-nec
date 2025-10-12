#pragma once

#include <vector>
#include <array>
#include <cmath>

namespace stl_to_eznec {

struct Point3D {
    double x, y, z;
    
    Point3D() : x(0), y(0), z(0) {}
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    double distance(const Point3D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
    
    Point3D operator+(const Point3D& other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }
    
    Point3D operator-(const Point3D& other) const {
        return Point3D(x - other.x, y - other.y, z - other.z);
    }
    
    Point3D operator*(double scalar) const {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }
    
    // Comparison operators for use in std::map
    bool operator<(const Point3D& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }
    
    bool operator==(const Point3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Triangle {
    std::array<Point3D, 3> vertices;
    Point3D normal;
    
    Triangle() = default;
    Triangle(const Point3D& v1, const Point3D& v2, const Point3D& v3) 
        : vertices{v1, v2, v3} {
        calculateNormal();
    }
    
    void calculateNormal() {
        Point3D v1 = vertices[1] - vertices[0];
        Point3D v2 = vertices[2] - vertices[0];
        
        // Cross product
        normal.x = v1.y * v2.z - v1.z * v2.y;
        normal.y = v1.z * v2.x - v1.x * v2.z;
        normal.z = v1.x * v2.y - v1.y * v2.x;
        
        // Normalize
        double length = std::sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
        if (length > 0) {
            normal.x /= length;
            normal.y /= length;
            normal.z /= length;
        }
    }
    
    Point3D center() const {
        return Point3D(
            (vertices[0].x + vertices[1].x + vertices[2].x) / 3.0,
            (vertices[0].y + vertices[1].y + vertices[2].y) / 3.0,
            (vertices[0].z + vertices[1].z + vertices[2].z) / 3.0
        );
    }
    
    double area() const {
        Point3D v1 = vertices[1] - vertices[0];
        Point3D v2 = vertices[2] - vertices[0];
        
        // Cross product magnitude / 2
        double cross_x = v1.y * v2.z - v1.z * v2.y;
        double cross_y = v1.z * v2.x - v1.x * v2.z;
        double cross_z = v1.x * v2.y - v1.y * v2.x;
        
        return std::sqrt(cross_x*cross_x + cross_y*cross_y + cross_z*cross_z) / 2.0;
    }
};

struct BoundingBox {
    Point3D min, max;
    
    BoundingBox() = default;
    BoundingBox(const Point3D& min, const Point3D& max) : min(min), max(max) {}
    
    void expand(const Point3D& point) {
        if (min.x == 0 && min.y == 0 && min.z == 0 && 
            max.x == 0 && max.y == 0 && max.z == 0) {
            min = max = point;
        } else {
            min.x = std::min(min.x, point.x);
            min.y = std::min(min.y, point.y);
            min.z = std::min(min.z, point.z);
            max.x = std::max(max.x, point.x);
            max.y = std::max(max.y, point.y);
            max.z = std::max(max.z, point.z);
        }
    }
    
    Point3D center() const {
        return Point3D(
            (min.x + max.x) / 2.0,
            (min.y + max.y) / 2.0,
            (min.z + max.z) / 2.0
        );
    }
    
    Point3D size() const {
        return Point3D(
            max.x - min.x,
            max.y - min.y,
            max.z - min.z
        );
    }
    
    double diagonal() const {
        return min.distance(max);
    }
};

class GeometryUtils {
public:
    static BoundingBox calculateBoundingBox(const std::vector<Triangle>& triangles);
    static double calculateTotalLength(const std::vector<Triangle>& triangles);
    static bool isWireLike(const std::vector<Triangle>& triangles, double maxDiameter = 0.01);
    static std::vector<Point3D> extractWirePath(const std::vector<Triangle>& triangles);
    static double calculateWireRadius(const std::vector<Triangle>& triangles);
    static bool arePointsCoincident(const Point3D& p1, const Point3D& p2, double tolerance = 1e-6);
    static std::vector<std::vector<Triangle>> separateConnectedComponents(const std::vector<Triangle>& triangles);
    
    // Enhanced STL processing methods
    static std::vector<Point3D> extractWirePathAdvanced(const std::vector<Triangle>& triangles);
    static std::vector<Point3D> simplifyWirePath(const std::vector<Point3D>& path, double tolerance = 1e-3);
    static double calculateWireLength(const std::vector<Point3D>& path);
    static bool isReasonableWireGeometry(const std::vector<Triangle>& triangles);
    static std::vector<Point3D> findWireEndpoints(const std::vector<Triangle>& triangles);
    static double calculateWireAspectRatio(const std::vector<Triangle>& triangles);
    static std::vector<Point3D> interpolateWirePath(const std::vector<Point3D>& path, int segments);
};

} // namespace stl_to_eznec


