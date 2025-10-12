#include "memory_manager.h"
#include "antenna_detector.h"
#include <iostream>
#include <fstream>
#include <sys/resource.h>
#include <unistd.h>
#include <functional>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdint>

namespace stl_to_eznec {

MemoryManager::MemoryManager() 
    : memoryLimitMB_(1024), peakMemoryUsage_(0), currentMemoryUsage_(0) {
}

size_t MemoryManager::getCurrentMemoryUsage() const {
    currentMemoryUsage_ = calculateMemoryUsage();
    return currentMemoryUsage_;
}

size_t MemoryManager::getPeakMemoryUsage() const {
    return peakMemoryUsage_;
}

void MemoryManager::resetPeakMemoryUsage() {
    peakMemoryUsage_ = 0;
}

void MemoryManager::setMemoryLimit(size_t limitMB) {
    memoryLimitMB_ = limitMB;
}

bool MemoryManager::isMemoryLimitExceeded() const {
    return getCurrentMemoryUsage() > (memoryLimitMB_ * 1024 * 1024);
}

MemoryManager::STLStreamProcessor::STLStreamProcessor(const std::string& filename, size_t chunkSize)
    : filename_(filename), chunkSize_(chunkSize), totalTriangles_(0), 
      processedTriangles_(0), isBinary_(false), headerRead_(false) {
    
    file_.open(filename, std::ios::binary);
    if (!file_.is_open()) {
        throw std::runtime_error("Cannot open STL file: " + filename);
    }
    
    // Determine if file is binary or ASCII
    std::string line;
    std::getline(file_, line);
    isBinary_ = (line.find("solid") == std::string::npos);
    file_.seekg(0);
    
    if (isBinary_) {
        // Read binary header and triangle count
        file_.seekg(80); // Skip 80-byte header
        uint32_t triangleCount;
        file_.read(reinterpret_cast<char*>(&triangleCount), 4);
        totalTriangles_ = triangleCount;
        file_.seekg(84); // Position after header
    } else {
        // Count triangles in ASCII file
        std::string content;
        file_.seekg(0, std::ios::end);
        size_t fileSize = file_.tellg();
        file_.seekg(0);
        
        // Rough estimate: count "facet" keywords
        std::string buffer;
        buffer.resize(fileSize);
        file_.read(&buffer[0], fileSize);
        
        size_t facetCount = 0;
        size_t pos = 0;
        while ((pos = buffer.find("facet", pos)) != std::string::npos) {
            facetCount++;
            pos += 5;
        }
        totalTriangles_ = facetCount;
        file_.seekg(0);
    }
}

bool MemoryManager::STLStreamProcessor::hasMoreTriangles() const {
    return processedTriangles_ < totalTriangles_;
}

std::vector<Triangle> MemoryManager::STLStreamProcessor::getNextChunk() {
    if (!hasMoreTriangles()) {
        return std::vector<Triangle>();
    }
    
    std::vector<Triangle> chunk;
    
    if (isBinary_) {
        chunk = readBinaryChunk();
    } else {
        chunk = readASCIIChunk();
    }
    
    processedTriangles_ += chunk.size();
    return chunk;
}

double MemoryManager::STLStreamProcessor::getProgress() const {
    if (totalTriangles_ == 0) return 0.0;
    return static_cast<double>(processedTriangles_) / totalTriangles_ * 100.0;
}

std::vector<Triangle> MemoryManager::STLStreamProcessor::readBinaryChunk() {
    std::vector<Triangle> chunk;
    
    size_t trianglesToRead = std::min(chunkSize_ / (50 * sizeof(float)), 
                                      totalTriangles_ - processedTriangles_);
    
    for (size_t i = 0; i < trianglesToRead && hasMoreTriangles(); ++i) {
        Triangle triangle;
        
        // Read normal (12 bytes)
        float nx, ny, nz;
        file_.read(reinterpret_cast<char*>(&nx), 4);
        file_.read(reinterpret_cast<char*>(&ny), 4);
        file_.read(reinterpret_cast<char*>(&nz), 4);
        
        // Read vertices (36 bytes)
        for (int j = 0; j < 3; ++j) {
            float x, y, z;
            file_.read(reinterpret_cast<char*>(&x), 4);
            file_.read(reinterpret_cast<char*>(&y), 4);
            file_.read(reinterpret_cast<char*>(&z), 4);
            triangle.vertices[j] = Point3D(x, y, z);
        }
        
        // Skip attribute (2 bytes)
        file_.seekg(2, std::ios::cur);
        
        triangle.calculateNormal();
        chunk.push_back(triangle);
    }
    
    return chunk;
}

std::vector<Triangle> MemoryManager::STLStreamProcessor::readASCIIChunk() {
    std::vector<Triangle> chunk;
    std::string line;
    
    size_t trianglesToRead = std::min(chunkSize_ / 1000, totalTriangles_ - processedTriangles_);
    
    for (size_t i = 0; i < trianglesToRead && hasMoreTriangles() && std::getline(file_, line); ++i) {
        if (line.find("facet") != std::string::npos) {
            Triangle triangle;
            
            // Skip "outer loop"
            std::getline(file_, line);
            
            // Read three vertices
            for (int j = 0; j < 3; ++j) {
                std::getline(file_, line);
                std::istringstream iss(line);
                std::string vertex;
                double x, y, z;
                iss >> vertex >> x >> y >> z;
                triangle.vertices[j] = Point3D(x, y, z);
            }
            
            // Skip "endloop" and "endfacet"
            std::getline(file_, line);
            std::getline(file_, line);
            
            triangle.calculateNormal();
            chunk.push_back(triangle);
        }
    }
    
    return chunk;
}

std::unique_ptr<MemoryManager::STLStreamProcessor> MemoryManager::createStreamProcessor(const std::string& filename) {
    return std::make_unique<STLStreamProcessor>(filename);
}

void MemoryManager::optimizeMemoryUsage() {
    // Force garbage collection
    clearCaches();
    
    // Update memory usage
    updatePeakMemoryUsage();
}

void MemoryManager::clearCaches() {
    // Clear any internal caches
    // This is a placeholder for actual cache clearing
}

MemoryManager::MemoryStats MemoryManager::getMemoryStats() const {
    MemoryStats stats;
    stats.currentUsage = getCurrentMemoryUsage();
    stats.peakUsage = peakMemoryUsage_;
    stats.limit = memoryLimitMB_ * 1024 * 1024;
    stats.usagePercentage = static_cast<double>(stats.currentUsage) / stats.limit * 100.0;
    stats.availableMemory = stats.limit - stats.currentUsage;
    return stats;
}

void MemoryManager::printMemoryStats() const {
    MemoryStats stats = getMemoryStats();
    
    std::cout << "\n=== Memory Statistics ===\n";
    std::cout << "Current Usage: " << (stats.currentUsage / 1024 / 1024) << " MB\n";
    std::cout << "Peak Usage: " << (stats.peakUsage / 1024 / 1024) << " MB\n";
    std::cout << "Memory Limit: " << memoryLimitMB_ << " MB\n";
    std::cout << "Usage Percentage: " << std::fixed << std::setprecision(1) 
              << stats.usagePercentage << "%\n";
    std::cout << "Available Memory: " << (stats.availableMemory / 1024 / 1024) << " MB\n";
}

size_t MemoryManager::calculateMemoryUsage() const {
    // Get current memory usage from system
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss * 1024; // Convert to bytes
}

void MemoryManager::updatePeakMemoryUsage() {
    size_t current = getCurrentMemoryUsage();
    if (current > peakMemoryUsage_) {
        peakMemoryUsage_ = current;
    }
}

// MemoryEfficientSTLParser implementation
MemoryEfficientSTLParser::MemoryEfficientSTLParser() {
}

bool MemoryEfficientSTLParser::processSTLFile(const std::string& filename, 
                                              std::function<void(const std::vector<Triangle>&)> processor,
                                              size_t chunkSize) {
    try {
        auto streamProcessor = memoryManager_.createStreamProcessor(filename);
        
        while (streamProcessor->hasMoreTriangles()) {
            std::vector<Triangle> chunk = streamProcessor->getNextChunk();
            if (!chunk.empty()) {
                processor(chunk);
            }
            
            // Check memory usage
            if (memoryManager_.isMemoryLimitExceeded()) {
                std::cerr << "Memory limit exceeded. Processing stopped.\n";
                return false;
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error processing STL file: " << e.what() << "\n";
        return false;
    }
}

MemoryEfficientSTLParser::STLFileStats MemoryEfficientSTLParser::getFileStats(const std::string& filename) {
    return analyzeSTLFile(filename);
}

AntennaWire MemoryEfficientSTLParser::detectAntennaStreaming(const std::string& filename) {
    AntennaWire antenna;
    
    // Process file in chunks and detect antenna
    processSTLFile(filename, [&antenna](const std::vector<Triangle>& chunk) {
        // Simple antenna detection on chunk
        // This is a simplified version - full implementation would be more complex
        for (const auto& triangle : chunk) {
            if (GeometryUtils::isWireLike({triangle})) {
                antenna.triangles.push_back(triangle);
            }
        }
    });
    
    // Analyze detected antenna
    if (!antenna.triangles.empty()) {
        antenna.path = GeometryUtils::extractWirePathAdvanced(antenna.triangles);
        antenna.length = GeometryUtils::calculateWireLength(antenna.path);
        antenna.radius = GeometryUtils::calculateWireRadius(antenna.triangles);
        antenna.isDetected = true;
    }
    
    return antenna;
}

MemoryEfficientSTLParser::STLFileStats MemoryEfficientSTLParser::analyzeSTLFile(const std::string& filename) {
    STLFileStats stats;
    
    // Get file size
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        stats.fileSize = file.tellg();
        file.close();
    }
    
    // Determine if binary or ASCII
    std::ifstream testFile(filename);
    std::string line;
    std::getline(testFile, line);
    stats.isBinary = (line.find("solid") == std::string::npos);
    testFile.close();
    
    // Count triangles (simplified)
    if (stats.isBinary) {
        std::ifstream binaryFile(filename, std::ios::binary);
        binaryFile.seekg(80);
        uint32_t triangleCount;
        binaryFile.read(reinterpret_cast<char*>(&triangleCount), 4);
        stats.triangleCount = triangleCount;
        binaryFile.close();
    } else {
        // Count triangles in ASCII file
        std::ifstream asciiFile(filename);
        std::string content;
        std::string line;
        size_t facetCount = 0;
        while (std::getline(asciiFile, line)) {
            if (line.find("facet") != std::string::npos) {
                facetCount++;
            }
        }
        stats.triangleCount = facetCount;
        asciiFile.close();
    }
    
    return stats;
}

} // namespace stl_to_eznec
