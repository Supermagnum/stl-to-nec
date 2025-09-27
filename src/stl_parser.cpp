#include "stl_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdint>

namespace stl_to_eznec {

STLParser::STLParser() 
    : scaleFactor_(1.0), loaded_(false) {
}

bool STLParser::loadFile(const std::string& filename) {
    triangles_.clear();
    loaded_ = false;
    errorMessage_.clear();
    
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        errorMessage_ = "Could not open file: " + filename;
        return false;
    }
    
    // Read the entire file
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();
    
    // Try to determine if it's ASCII or binary
    std::string content(buffer.begin(), buffer.end());
    
    if (isASCII(content)) {
        if (!parseASCII(content)) {
            return false;
        }
    } else {
        if (!parseBinary(buffer)) {
            return false;
        }
    }
    
    calculateBoundingBox();
    loaded_ = true;
    return true;
}

bool STLParser::isASCII(const std::string& content) {
    // Check for ASCII STL keywords
    std::string lowerContent = content;
    std::transform(lowerContent.begin(), lowerContent.end(), lowerContent.begin(), ::tolower);
    
    return (lowerContent.find("solid") != std::string::npos && 
            lowerContent.find("facet") != std::string::npos);
}

bool STLParser::parseASCII(const std::string& content) {
    std::istringstream stream(content);
    std::string line;
    
    // Skip header line
    std::getline(stream, line);
    
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string keyword;
        lineStream >> keyword;
        
        if (keyword == "facet") {
            // Parse normal
            std::string normal;
            double nx, ny, nz;
            lineStream >> normal >> nx >> ny >> nz;
            
            // Skip "outer loop"
            std::getline(stream, line);
            
            // Parse three vertices
            std::array<Point3D, 3> vertices;
            for (int i = 0; i < 3; ++i) {
                std::getline(stream, line);
                std::istringstream vertexStream(line);
                std::string vertex;
                double x, y, z;
                vertexStream >> vertex >> x >> y >> z;
                vertices[i] = Point3D(x, y, z);
            }
            
            // Skip "endloop" and "endfacet"
            std::getline(stream, line);
            std::getline(stream, line);
            
            Triangle triangle(vertices[0], vertices[1], vertices[2]);
            triangles_.push_back(triangle);
        }
    }
    
    return !triangles_.empty();
}

bool STLParser::parseBinary(const std::vector<uint8_t>& data) {
    if (data.size() < 84) {
        errorMessage_ = "File too small to be a valid binary STL";
        return false;
    }
    
    size_t offset = 0;
    
    // Skip 80-byte header
    offset += 80;
    
    // Read triangle count
    if (offset + 4 > data.size()) {
        errorMessage_ = "Invalid binary STL file";
        return false;
    }
    
    uint32_t triangleCount;
    std::memcpy(&triangleCount, &data[offset], 4);
    offset += 4;
    
    // Each triangle is 50 bytes (12 bytes normal + 36 bytes vertices + 2 bytes attribute)
    size_t expectedSize = 84 + triangleCount * 50;
    if (data.size() < expectedSize) {
        errorMessage_ = "File size doesn't match triangle count";
        return false;
    }
    
    triangles_.reserve(triangleCount);
    
    for (uint32_t i = 0; i < triangleCount; ++i) {
        if (offset + 50 > data.size()) {
            errorMessage_ = "Unexpected end of file";
            return false;
        }
        
        // Read normal (12 bytes)
        float nx, ny, nz;
        std::memcpy(&nx, &data[offset], 4);
        std::memcpy(&ny, &data[offset + 4], 4);
        std::memcpy(&nz, &data[offset + 8], 4);
        offset += 12;
        
        // Read vertices (36 bytes)
        std::array<Point3D, 3> vertices;
        for (int j = 0; j < 3; ++j) {
            float x, y, z;
            std::memcpy(&x, &data[offset], 4);
            std::memcpy(&y, &data[offset + 4], 4);
            std::memcpy(&z, &data[offset + 8], 4);
            vertices[j] = Point3D(x, y, z);
            offset += 12;
        }
        
        // Skip attribute (2 bytes)
        offset += 2;
        
        Triangle triangle(vertices[0], vertices[1], vertices[2]);
        triangles_.push_back(triangle);
    }
    
    return !triangles_.empty();
}

BoundingBox STLParser::getBoundingBox() const {
    return GeometryUtils::calculateBoundingBox(triangles_);
}

double STLParser::getTotalArea() const {
    double totalArea = 0.0;
    for (const auto& triangle : triangles_) {
        totalArea += triangle.area();
    }
    return totalArea;
}

void STLParser::scaleToLength(double targetLength) {
    if (triangles_.empty()) return;
    
    BoundingBox bbox = getBoundingBox();
    Point3D size = bbox.size();
    
    // Find the largest dimension
    double maxDimension = std::max({size.x, size.y, size.z});
    
    if (maxDimension > 0) {
        scaleFactor_ = targetLength / maxDimension;
        applyScaling();
    }
}

void STLParser::scaleToLength(double targetLength, const std::string& axis) {
    if (triangles_.empty()) return;
    
    BoundingBox bbox = getBoundingBox();
    Point3D size = bbox.size();
    
    double currentLength = 0.0;
    if (axis == "x") {
        currentLength = size.x;
    } else if (axis == "y") {
        currentLength = size.y;
    } else if (axis == "z") {
        currentLength = size.z;
    } else {
        return; // Invalid axis
    }
    
    if (currentLength > 0) {
        scaleFactor_ = targetLength / currentLength;
        applyScaling();
    }
}

void STLParser::calculateBoundingBox() {
    originalBoundingBox_ = GeometryUtils::calculateBoundingBox(triangles_);
}

void STLParser::applyScaling() {
    if (scaleFactor_ == 1.0) return;
    
    for (auto& triangle : triangles_) {
        for (auto& vertex : triangle.vertices) {
            vertex.x *= scaleFactor_;
            vertex.y *= scaleFactor_;
            vertex.z *= scaleFactor_;
        }
        triangle.calculateNormal();
    }
}

} // namespace stl_to_eznec
