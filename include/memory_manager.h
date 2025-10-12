#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <functional>
#include "geometry_utils.h"

// Forward declarations
namespace stl_to_eznec {
    struct AntennaWire;
}

namespace stl_to_eznec {

class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager() = default;
    
    // Memory monitoring
    size_t getCurrentMemoryUsage() const;
    size_t getPeakMemoryUsage() const;
    void resetPeakMemoryUsage();
    
    // Memory limits
    void setMemoryLimit(size_t limitMB);
    size_t getMemoryLimit() const { return memoryLimitMB_; }
    bool isMemoryLimitExceeded() const;
    
    // Streaming STL processing
    class STLStreamProcessor {
    public:
        STLStreamProcessor(const std::string& filename, size_t chunkSize = 1024 * 1024);
        ~STLStreamProcessor() = default;
        
        bool hasMoreTriangles() const;
        std::vector<Triangle> getNextChunk();
        size_t getTotalTriangles() const { return totalTriangles_; }
        size_t getProcessedTriangles() const { return processedTriangles_; }
        double getProgress() const;
        
    private:
        std::string filename_;
        size_t chunkSize_;
        size_t totalTriangles_;
        size_t processedTriangles_;
        std::ifstream file_;
        bool isBinary_;
        bool headerRead_;
        
        bool readSTLHeader();
        std::vector<Triangle> readBinaryChunk();
        std::vector<Triangle> readASCIIChunk();
    };
    
    // Memory-efficient triangle processing
    std::unique_ptr<STLStreamProcessor> createStreamProcessor(const std::string& filename);
    
    // Memory optimization
    void optimizeMemoryUsage();
    void clearCaches();
    
    // Memory statistics
    struct MemoryStats {
        size_t currentUsage;
        size_t peakUsage;
        size_t limit;
        double usagePercentage;
        size_t availableMemory;
    };
    
    MemoryStats getMemoryStats() const;
    void printMemoryStats() const;
    
    // Memory-safe operations
    template<typename T>
    std::vector<T> processInChunks(const std::vector<T>& data, size_t chunkSize, 
                                   std::function<void(const std::vector<T>&)> processor);

private:
    size_t memoryLimitMB_;
    size_t peakMemoryUsage_;
    mutable size_t currentMemoryUsage_;
    
    size_t calculateMemoryUsage() const;
    void updatePeakMemoryUsage();
    bool checkMemoryLimit() const;
};

// Memory-safe STL processing
class MemoryEfficientSTLParser {
public:
    MemoryEfficientSTLParser();
    ~MemoryEfficientSTLParser() = default;
    
    // Process STL file in chunks
    bool processSTLFile(const std::string& filename, 
                       std::function<void(const std::vector<Triangle>&)> processor,
                       size_t chunkSize = 1024 * 1024);
    
    // Get file statistics without loading entire file
    struct STLFileStats {
        size_t triangleCount;
        BoundingBox boundingBox;
        double totalArea;
        bool isBinary;
        size_t fileSize;
    };
    
    STLFileStats getFileStats(const std::string& filename);
    
    // Memory-efficient antenna detection
    AntennaWire detectAntennaStreaming(const std::string& filename);
    
private:
    MemoryManager memoryManager_;
    
    STLFileStats analyzeSTLFile(const std::string& filename);
    BoundingBox calculateBoundingBoxStreaming(const std::string& filename);
    double calculateTotalAreaStreaming(const std::string& filename);
};

} // namespace stl_to_eznec
