# API Reference

## Overview

This document provides comprehensive API documentation for the STL to EZ/NEC Converter tool.

## Core Classes

### GeometryUtils

Utility class for geometric operations and calculations.

#### Static Methods

```cpp
// Calculate bounding box for a set of triangles
static BoundingBox calculateBoundingBox(const std::vector<Triangle>& triangles);

// Calculate total length of triangle edges
static double calculateTotalLength(const std::vector<Triangle>& triangles);

// Check if triangles represent a wire-like object
static bool isWireLike(const std::vector<Triangle>& triangles, double maxDiameter = 0.01);

// Extract wire path from triangles
static std::vector<Point3D> extractWirePath(const std::vector<Triangle>& triangles);

// Advanced wire path extraction with endpoint detection
static std::vector<Point3D> extractWirePathAdvanced(const std::vector<Triangle>& triangles);

// Simplify wire path by removing redundant points
static std::vector<Point3D> simplifyWirePath(const std::vector<Point3D>& path, double tolerance = 1e-3);

// Calculate wire length from path points
static double calculateWireLength(const std::vector<Point3D>& path);

// Check if geometry is reasonable for a wire
static bool isReasonableWireGeometry(const std::vector<Triangle>& triangles);

// Find wire endpoints (vertices that appear only once)
static std::vector<Point3D> findWireEndpoints(const std::vector<Triangle>& triangles);

// Calculate wire aspect ratio (length/width)
static double calculateWireAspectRatio(const std::vector<Triangle>& triangles);

// Interpolate wire path for better segmentation
static std::vector<Point3D> interpolateWirePath(const std::vector<Point3D>& path, int segments);
```

### STLParser

Handles STL file parsing and processing.

#### Constructor
```cpp
STLParser();
```

#### Public Methods
```cpp
// Load and parse STL file
bool loadFile(const std::string& filename);

// Get parsed triangles
const std::vector<Triangle>& getTriangles() const;

// Get bounding box of loaded model
BoundingBox getBoundingBox() const;

// Get total surface area
double getTotalArea() const;

// Check if file was loaded successfully
bool isLoaded() const;

// Get error message if loading failed
const std::string& getErrorMessage() const;

// Scale model to specified length
void scaleToLength(double targetLength);
void scaleToLength(double targetLength, const std::string& axis);

// Get current scale factor
double getScaleFactor() const;
```

### MaterialDatabase

Manages material properties for electromagnetic simulation.

#### Constructor
```cpp
MaterialDatabase();
```

#### Public Methods
```cpp
// Get all available materials
const std::vector<MaterialProperties>& getAllMaterials() const;

// Get material by name
const MaterialProperties* getMaterial(const std::string& name) const;

// Get material by index
const MaterialProperties* getMaterial(int index) const;

// Get material count
size_t getMaterialCount() const;

// Print material list for user selection
void printMaterialList() const;

// Validate material selection
bool isValidMaterial(const std::string& name) const;
bool isValidMaterialIndex(int index) const;
```

### AntennaDetector

Detects and analyzes antenna wires in STL models.

#### Constructor
```cpp
AntennaDetector();
```

#### Public Methods
```cpp
// Detect antenna wire from STL triangles
AntennaWire detectAntenna(const std::vector<Triangle>& triangles);

// Check if antenna was detected
bool isAntennaDetected() const;

// Get detected antenna
const AntennaWire& getAntenna() const;

// Get antenna length in meters
double getAntennaLength() const;

// Get antenna radius in meters
double getAntennaRadius() const;

// Get antenna path points
const std::vector<Point3D>& getAntennaPath() const;

// Get antenna start and end points
Point3D getStartPoint() const;
Point3D getEndPoint() const;

// Print antenna information
void printAntennaInfo() const;

// Set detection parameters
void setMaxWireDiameter(double diameter);
void setMinWireLength(double length);
void setMaxWireLength(double length);
```

### FrequencyCalculator

Calculates electromagnetic parameters based on frequency.

#### Constructor
```cpp
FrequencyCalculator();
```

#### Public Methods
```cpp
// Set frequency in MHz
void setFrequency(double frequencyMHz);

// Get wavelength in meters
double getWavelength() const;

// Get high accuracy grid spacing (λ/20) in meters
double getHighAccuracyGridSpacing() const;

// Get standard accuracy grid spacing (λ/10) in meters
double getStandardAccuracyGridSpacing() const;

// Get recommended grid spacing (5cm) in meters
double getRecommendedGridSpacing() const;

// Calculate number of segments for a wire
int calculateSegments(double wireLength, double gridSpacing) const;

// Get frequency in MHz
double getFrequencyMHz() const;

// Check if frequency is valid
bool isValidFrequency() const;

// Print frequency information
void printFrequencyInfo() const;

// Get frequency band name
std::string getFrequencyBand() const;
```

### NECGenerator

Generates NEC format output files.

#### Constructor
```cpp
NECGenerator();
```

#### Public Methods
```cpp
// Generate NEC file from STL data
std::string generateNEC(
    const std::vector<Triangle>& triangles,
    const MaterialProperties& material,
    const FrequencyCalculator& frequency,
    const AntennaWire& antenna,
    const std::string& modelName = "STL Model",
    bool hasAntenna = true,
    double waterlineHeight = 0.0,
    const WaterProperties* water = nullptr
);

// Generate NEC file without antenna
std::string generateNECStructureOnly(
    const std::vector<Triangle>& triangles,
    const MaterialProperties& material,
    const std::string& modelName = "STL Model"
);

// Set output options
void setIncludeComments(bool include);
void setIncludePattern(bool include);
void setIncludeCurrent(bool include);

// Get generated NEC content
const std::string& getNECContent() const;
```

### EZGenerator

Generates EZ format output files for 4NEC2 compatibility.

#### Constructor
```cpp
EZGenerator();
```

#### Public Methods
```cpp
// Generate EZ file from STL data
std::string generateEZ(
    const std::vector<Triangle>& triangles,
    const MaterialProperties& material,
    const FrequencyCalculator& frequency,
    const AntennaWire& antenna,
    const std::string& modelName = "STL Model",
    bool hasAntenna = true,
    double waterlineHeight = 0.0,
    const WaterProperties* water = nullptr
);

// Generate EZ file without antenna
std::string generateEZStructureOnly(
    const std::vector<Triangle>& triangles,
    const MaterialProperties& material,
    const std::string& modelName = "STL Model"
);

// Set output options
void setIncludeComments(bool include);
void setIncludePattern(bool include);

// Get generated EZ content
const std::string& getEZContent() const;
```

### UserInterface

Handles user interaction and input collection.

#### Constructor
```cpp
UserInterface();
```

#### Public Methods
```cpp
// Main interaction loop
UserInput getUserInput();

// Get STL filename
std::string getSTLFilename();

// Get material selection
MaterialProperties getMaterialSelection();

// Get frequency input
double getFrequencyInput();

// Get antenna detection result
bool getAntennaConfirmation(bool antennaDetected);

// Get antenna detection enable/disable option
bool getAntennaDetectionOption();

// Get vehicle type
VehicleType getVehicleType();

// Get waterline height (for ships/boats)
double getWaterlineHeight();

// Get water type (for ships/boats)
WaterProperties* getWaterType();

// Get output filenames
std::string getOutputNECFilename(const std::string& stlFilename);
std::string getOutputEZFilename(const std::string& stlFilename);

// Get model name
std::string getModelName(const std::string& stlFilename);

// Print various help messages
void printWelcome();
void printMaterialHelp();
void printFrequencyHelp();
void printVehicleTypeHelp();
void printWaterlineHelp();
void printAntennaDetectionResult(const AntennaWire& antenna);
void printConversionSummary(const UserInput& input);

// Print status messages
void printError(const std::string& message);
void printSuccess(const std::string& message);
void printInfo(const std::string& message);
```

## Data Structures

### Point3D

Represents a 3D point in space.

```cpp
struct Point3D {
    double x, y, z;
    
    // Constructors
    Point3D();
    Point3D(double x, double y, double z);
    
    // Operations
    double distance(const Point3D& other) const;
    Point3D operator+(const Point3D& other) const;
    Point3D operator-(const Point3D& other) const;
    Point3D operator*(double scalar) const;
    bool operator<(const Point3D& other) const;
    bool operator==(const Point3D& other) const;
};
```

### Triangle

Represents a triangular face with vertices and normal.

```cpp
struct Triangle {
    std::array<Point3D, 3> vertices;
    Point3D normal;
    
    // Constructors
    Triangle();
    Triangle(const Point3D& v1, const Point3D& v2, const Point3D& v3);
    
    // Methods
    void calculateNormal();
    Point3D center() const;
    double area() const;
};
```

### BoundingBox

Represents a 3D bounding box.

```cpp
struct BoundingBox {
    Point3D min, max;
    
    // Constructors
    BoundingBox();
    BoundingBox(const Point3D& min, const Point3D& max);
    
    // Methods
    void expand(const Point3D& point);
    Point3D center() const;
    Point3D size() const;
    double diagonal() const;
};
```

### MaterialProperties

Contains electromagnetic material properties.

```cpp
struct MaterialProperties {
    std::string name;
    double conductivity;  // S/m
    double relativePermittivity;
    std::string description;
    
    // Constructors
    MaterialProperties();
    MaterialProperties(const std::string& name, double conductivity, 
                      double relativePermittivity, const std::string& description = "");
};
```

### AntennaWire

Represents a detected antenna wire.

```cpp
struct AntennaWire {
    std::vector<Triangle> triangles;
    std::vector<Point3D> path;
    double radius;
    double length;
    Point3D startPoint;
    Point3D endPoint;
    bool isDetected;
    
    // Constructor
    AntennaWire();
};
```

### UserInput

Contains all user input parameters.

```cpp
struct UserInput {
    std::string stlFilename;
    MaterialProperties material;
    double frequencyMHz;
    bool hasAntenna;
    bool enableAntennaDetection;
    VehicleType vehicleType;
    double waterlineHeight;  // For ships/boats only
    WaterProperties* waterProperties;  // For ships/boats only
    std::string outputNECFilename;
    std::string outputEZFilename;
    std::string modelName;
    
    // Constructor
    UserInput();
};
```

## Enumerations

### VehicleType

```cpp
enum class VehicleType {
    UNKNOWN,
    SHIP,
    BOAT,
    AIRPLANE,
    HELICOPTER,
    CAR,
    LAND_VEHICLE
};
```

### GroundType

```cpp
enum class GroundType {
    PERFECT_GROUND,
    SOMMERFELD_NORTON,
    FINITE_GROUND_SCREEN,
    REAL_GROUND,
    WATER_GROUND
};
```

### LogLevel

```cpp
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};
```

## Usage Examples

### Basic STL Processing

```cpp
#include "stl_parser.h"
#include "antenna_detector.h"
#include "nec_generator.h"

// Load STL file
STLParser parser;
if (parser.loadFile("model.stl")) {
    auto triangles = parser.getTriangles();
    
    // Detect antenna
    AntennaDetector detector;
    AntennaWire antenna = detector.detectAntenna(triangles);
    
    // Generate NEC file
    NECGenerator necGen;
    std::string necContent = necGen.generateNEC(triangles, material, frequency, antenna);
}
```

### Material Selection

```cpp
#include "material_database.h"

MaterialDatabase materialDB;
materialDB.printMaterialList();

// Get material by index
const MaterialProperties* material = materialDB.getMaterial(1); // Aluminum

// Get material by name
const MaterialProperties* steel = materialDB.getMaterial("Mild Steel");
```

### Frequency Analysis

```cpp
#include "frequency_calculator.h"

FrequencyCalculator freq;
freq.setFrequency(430.0); // 430 MHz

double wavelength = freq.getWavelength();
double gridSpacing = freq.getRecommendedGridSpacing();
int segments = freq.calculateSegments(wireLength, gridSpacing);
```

### Progress Monitoring

```cpp
#include "progress_indicator.h"

ProgressIndicator progress("Processing STL file", 100);
progress.update(25);
progress.setMessage("Detecting antenna...");
progress.complete("Processing completed successfully");
```

### Logging

```cpp
#include "logger.h"

Logger& logger = Logger::getInstance();
logger.setLogLevel(LogLevel::INFO);
logger.setLogFile("conversion.log");

logger.info("Starting STL conversion");
logger.logStep("Loading STL file", "model.stl");
logger.logSuccess("Conversion completed");
```

## Error Handling

All classes provide comprehensive error handling:

- **STLParser**: Returns error messages via `getErrorMessage()`
- **AntennaDetector**: Returns empty `AntennaWire` if no antenna detected
- **MaterialDatabase**: Validates material selections
- **FrequencyCalculator**: Validates frequency ranges
- **UserInterface**: Provides clear error messages and recovery options

## Thread Safety

- **Logger**: Thread-safe with mutex protection
- **MemoryManager**: Thread-safe memory monitoring
- **Other classes**: Not thread-safe, use single-threaded

## Memory Management

- **STLStreamProcessor**: Processes large files in chunks
- **MemoryManager**: Monitors and limits memory usage
- **MemoryEfficientSTLParser**: Handles large STL files efficiently

## Performance Considerations

- Use `STLStreamProcessor` for large files (>100MB)
- Set appropriate memory limits
- Enable progress indicators for long operations
- Use logging for debugging and monitoring
