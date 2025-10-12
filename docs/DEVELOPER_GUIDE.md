# Developer Guide

## Getting Started

### Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.10+** for building
- **Git** for version control
- **Make** or **Ninja** for compilation

### Development Environment Setup

#### Linux/macOS
```bash
# Clone the repository
git clone https://github.com/yourusername/stl-to-eznec.git
cd stl-to-eznec

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
make -j4
```

#### Windows (Visual Studio)
```cmd
# Clone the repository
git clone https://github.com/yourusername/stl-to-eznec.git
cd stl-to-eznec

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "Visual Studio 16 2019"

# Build the project
cmake --build . --config Release
```

#### Windows (MinGW)
```cmd
# Clone the repository
git clone https://github.com/yourusername/stl-to-eznec.git
cd stl-to-eznec

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "MinGW Makefiles"

# Build the project
mingw32-make -j4
```

## Project Structure

```
stl-to-eznec/
├── CMakeLists.txt          # CMake configuration
├── README.md               # User documentation
├── LICENSE                 # MIT License
├── build.sh                # Build script
├── include/                # Header files
│   ├── stl_parser.h
│   ├── material_database.h
│   ├── antenna_detector.h
│   ├── frequency_calculator.h
│   ├── nec_generator.h
│   ├── ez_generator.h
│   ├── user_interface.h
│   ├── geometry_utils.h
│   ├── progress_indicator.h
│   ├── ground_modeler.h
│   ├── logger.h
│   ├── validator.h
│   └── memory_manager.h
├── src/                    # Source files
│   ├── main.cpp
│   ├── stl_parser.cpp
│   ├── material_database.cpp
│   ├── antenna_detector.cpp
│   ├── frequency_calculator.cpp
│   ├── nec_generator.cpp
│   ├── ez_generator.cpp
│   ├── user_interface.cpp
│   ├── geometry_utils.cpp
│   ├── progress_indicator.cpp
│   ├── ground_modeler.cpp
│   ├── logger.cpp
│   └── memory_manager.cpp
├── docs/                   # Documentation
│   ├── API_REFERENCE.md
│   ├── ARCHITECTURE.md
│   └── DEVELOPER_GUIDE.md
└── tests/                  # Test files
    ├── test_geometry.cpp
    ├── test_materials.cpp
    └── test_parsing.cpp
```

## Coding Standards

### 1. C++ Style Guide

#### Naming Conventions
```cpp
// Classes: PascalCase
class MaterialDatabase { };

// Methods: camelCase
void calculateWavelength();

// Variables: camelCase
double frequencyMHz;

// Constants: UPPER_CASE
const double SPEED_OF_LIGHT = 3e8;

// Private members: trailing underscore
private:
    std::string filename_;
    bool isLoaded_;
```

#### Code Formatting
```cpp
// Use 4 spaces for indentation
// Use braces for all control structures
if (condition) {
    doSomething();
}

// Use const where possible
const double wavelength = calculateWavelength(frequency);

// Use references for large objects
void processTriangles(const std::vector<Triangle>& triangles);
```

#### Documentation
```cpp
/**
 * @brief Calculate the wavelength from frequency
 * @param frequencyMHz Frequency in MHz
 * @return Wavelength in meters
 * @throws std::invalid_argument if frequency is negative
 */
double calculateWavelength(double frequencyMHz) const;
```

### 2. Error Handling

#### Exception Handling
```cpp
// Use specific exceptions
if (frequency < 0) {
    throw std::invalid_argument("Frequency must be positive");
}

// Use RAII for resource management
class FileHandler {
    std::ifstream file_;
public:
    FileHandler(const std::string& filename) : file_(filename) {
        if (!file_.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
    }
    ~FileHandler() { file_.close(); }
};
```

#### Error Reporting
```cpp
// Use logger for error reporting
Logger& logger = Logger::getInstance();
logger.error("Failed to load STL file: " + filename);

// Provide user-friendly error messages
void printError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    std::cerr << "Please check the file and try again." << std::endl;
}
```

### 3. Memory Management

#### Smart Pointers
```cpp
// Use smart pointers for dynamic allocation
std::unique_ptr<STLStreamProcessor> processor = 
    memoryManager_.createStreamProcessor(filename);

// Use shared_ptr for shared ownership
std::shared_ptr<MaterialProperties> material = 
    std::make_shared<MaterialProperties>();
```

#### Memory Monitoring
```cpp
// Check memory usage
if (memoryManager_.isMemoryLimitExceeded()) {
    logger.warning("Memory limit exceeded, optimizing...");
    memoryManager_.optimizeMemoryUsage();
}
```

## Testing

### 1. Unit Testing

#### Test Structure
```cpp
#include <gtest/gtest.h>
#include "geometry_utils.h"

class GeometryUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test data
    }
    
    void TearDown() override {
        // Cleanup
    }
};

TEST_F(GeometryUtilsTest, CalculateDistance) {
    Point3D p1(0, 0, 0);
    Point3D p2(3, 4, 0);
    double distance = p1.distance(p2);
    EXPECT_EQ(distance, 5.0);
}
```

#### Running Tests
```bash
# Build tests
mkdir build
cd build
cmake .. -DENABLE_TESTS=ON
make

# Run tests
./test_runner
```

### 2. Integration Testing

#### Test Data
```cpp
// Create test STL files
void createTestSTL(const std::string& filename) {
    std::ofstream file(filename);
    file << "solid test\n";
    file << "  facet normal 0 0 1\n";
    file << "    outer loop\n";
    file << "      vertex 0 0 0\n";
    file << "      vertex 1 0 0\n";
    file << "      vertex 0 1 0\n";
    file << "    endloop\n";
    file << "  endfacet\n";
    file << "endsolid test\n";
    file.close();
}
```

### 3. Performance Testing

#### Benchmarking
```cpp
#include <chrono>

void benchmarkSTLParsing() {
    auto start = std::chrono::high_resolution_clock::now();
    
    STLParser parser;
    parser.loadFile("large_model.stl");
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Parsing took: " << duration.count() << " ms" << std::endl;
}
```

## Debugging

### 1. Logging

#### Debug Logging
```cpp
// Enable debug logging
Logger& logger = Logger::getInstance();
logger.setLogLevel(LogLevel::DEBUG);
logger.setLogFile("debug.log");

// Use debug logging
logger.debug("Processing triangle " + std::to_string(i));
logger.debug("Triangle vertices: " + triangle.toString());
```

#### Progress Monitoring
```cpp
// Use progress indicators
ProgressIndicator progress("Processing STL", 100);
for (int i = 0; i < triangles.size(); ++i) {
    processTriangle(triangles[i]);
    progress.update(i);
}
progress.complete();
```

### 2. Memory Debugging

#### Memory Monitoring
```cpp
// Monitor memory usage
MemoryManager& memMgr = MemoryManager::getInstance();
memMgr.printMemoryStats();

// Set memory limits
memMgr.setMemoryLimit(512); // 512 MB limit
```

#### Valgrind (Linux)
```bash
# Run with Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./stl-to-eznec
```

### 3. Profiling

#### gprof (Linux)
```bash
# Compile with profiling
g++ -pg -O2 -o stl-to-eznec src/*.cpp

# Run and generate profile
./stl-to-eznec
gprof stl-to-eznec gmon.out > profile.txt
```

## Contributing

### 1. Code Contributions

#### Pull Request Process
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass
6. Submit a pull request

#### Code Review Checklist
- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] Documentation is updated
- [ ] No memory leaks
- [ ] Error handling is appropriate
- [ ] Performance is acceptable

### 2. Documentation

#### API Documentation
```cpp
/**
 * @brief Calculate the wavelength from frequency
 * @param frequencyMHz Frequency in MHz (must be positive)
 * @return Wavelength in meters
 * @throws std::invalid_argument if frequency is negative or zero
 * @note Uses speed of light = 3e8 m/s
 * @example
 *   double wavelength = calcWavelength(430.0); // Returns ~0.698m
 */
double calculateWavelength(double frequencyMHz) const;
```

#### User Documentation
- Update README.md for new features
- Add examples for new functionality
- Document any breaking changes

### 3. Testing

#### Test Coverage
- Aim for >90% code coverage
- Test all public methods
- Test error conditions
- Test edge cases

#### Test Data
- Include sample STL files
- Test with various file sizes
- Test with different geometries

## Performance Optimization

### 1. Algorithm Optimization

#### Efficient Data Structures
```cpp
// Use appropriate containers
std::vector<Triangle> triangles;  // For sequential access
std::map<Point3D, int> vertexMap; // For lookups
std::set<Point3D> uniqueVertices; // For uniqueness
```

#### Memory Optimization
```cpp
// Use streaming for large files
STLStreamProcessor processor(filename);
while (processor.hasMoreTriangles()) {
    auto chunk = processor.getNextChunk();
    processChunk(chunk);
}
```

### 2. Compiler Optimization

#### Release Build
```bash
# Optimized build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

#### Compiler Flags
```cmake
# Add optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
```

### 3. Profiling and Benchmarking

#### Performance Testing
```cpp
// Benchmark different approaches
void benchmarkWireDetection() {
    auto start = std::chrono::high_resolution_clock::now();
    
    // Test method 1
    auto result1 = detectWireMethod1(triangles);
    
    auto mid = std::chrono::high_resolution_clock::now();
    
    // Test method 2
    auto result2 = detectWireMethod2(triangles);
    
    auto end = std::chrono::high_resolution_clock::now();
    
    // Compare results and timing
}
```

## Troubleshooting

### 1. Common Issues

#### Compilation Errors
```bash
# Check C++ version
g++ --version

# Ensure C++17 support
g++ -std=c++17 -o test test.cpp
```

#### Runtime Errors
```cpp
// Check file existence
if (!std::filesystem::exists(filename)) {
    throw std::runtime_error("File not found: " + filename);
}

// Check file permissions
if (!std::filesystem::is_regular_file(filename)) {
    throw std::runtime_error("Not a regular file: " + filename);
}
```

### 2. Debugging Tips

#### Logging
```cpp
// Use different log levels
logger.debug("Detailed debugging info");
logger.info("General information");
logger.warning("Potential issues");
logger.error("Error conditions");
logger.critical("Critical failures");
```

#### Memory Issues
```cpp
// Monitor memory usage
MemoryManager& memMgr = MemoryManager::getInstance();
memMgr.printMemoryStats();

// Check for memory leaks
if (memMgr.isMemoryLimitExceeded()) {
    logger.warning("Memory limit exceeded");
}
```

### 3. Platform-Specific Issues

#### Linux
```bash
# Check dependencies
ldd ./stl-to-eznec

# Check system limits
ulimit -a
```

#### Windows
```cmd
# Check Visual C++ Redistributable
# Ensure proper DLLs are available
```

#### macOS
```bash
# Check Xcode command line tools
xcode-select --install

# Check system libraries
otool -L ./stl-to-eznec
```

## Future Development

### 1. Planned Features

#### Enhanced STL Processing
- Support for additional 3D formats (OBJ, PLY)
- Advanced mesh optimization
- Automatic mesh repair

#### Improved Antenna Detection
- Machine learning-based detection
- Support for complex antenna geometries
- Automatic antenna classification

#### Advanced Ground Modeling
- 3D ground models
- Terrain integration
- Advanced propagation models

### 2. Architecture Improvements

#### Parallel Processing
- Multi-threaded STL processing
- Parallel antenna detection
- Concurrent file generation

#### Plugin System
- Extensible material database
- Custom ground models
- User-defined antenna types

#### Web Interface
- Browser-based interface
- Cloud processing
- Real-time collaboration

### 3. Performance Enhancements

#### GPU Acceleration
- CUDA-based processing
- OpenCL support
- GPU memory management

#### Advanced Caching
- Intelligent caching strategies
- Persistent cache storage
- Cache optimization

#### Distributed Processing
- Multi-machine processing
- Load balancing
- Fault tolerance
