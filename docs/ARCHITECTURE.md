# Architecture Documentation

## System Overview

The STL to EZ/NEC Converter is a cross-platform C++ application that converts 3D STL models into electromagnetic simulation files for antenna modeling software.

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    User Interface Layer                    │
├─────────────────────────────────────────────────────────────┤
│  UserInterface  │  ProgressIndicator  │  Logger           │
├─────────────────────────────────────────────────────────────┤
│                    Processing Layer                        │
├─────────────────────────────────────────────────────────────┤
│  STLParser  │  AntennaDetector  │  GeometryUtils          │
├─────────────────────────────────────────────────────────────┤
│                    Generation Layer                        │
├─────────────────────────────────────────────────────────────┤
│  NECGenerator  │  EZGenerator  │  GroundModeler           │
├─────────────────────────────────────────────────────────────┤
│                    Data Layer                              │
├─────────────────────────────────────────────────────────────┤
│  MaterialDatabase  │  FrequencyCalculator  │  Validator     │
├─────────────────────────────────────────────────────────────┤
│                    Memory Management                       │
├─────────────────────────────────────────────────────────────┤
│  MemoryManager  │  STLStreamProcessor  │  MemoryEfficient  │
└─────────────────────────────────────────────────────────────┘
```

## Component Details

### 1. User Interface Layer

#### UserInterface
- **Purpose**: Handles all user interaction and input collection
- **Responsibilities**:
  - Collect user input (STL file, material, frequency, etc.)
  - Validate user input
  - Provide help and guidance
  - Display results and status messages
- **Dependencies**: None (top-level component)

#### ProgressIndicator
- **Purpose**: Provides real-time progress feedback
- **Responsibilities**:
  - Display progress bars
  - Show elapsed time
  - Provide step-by-step status updates
- **Dependencies**: None

#### Logger
- **Purpose**: Comprehensive logging system
- **Responsibilities**:
  - Log all operations and errors
  - Provide different log levels
  - Support both console and file output
  - Thread-safe logging
- **Dependencies**: None

### 2. Processing Layer

#### STLParser
- **Purpose**: Parse and process STL files
- **Responsibilities**:
  - Load STL files (ASCII and binary)
  - Parse triangle data
  - Calculate bounding boxes
  - Scale models to correct dimensions
- **Dependencies**: GeometryUtils

#### AntennaDetector
- **Purpose**: Detect antenna wires in STL models
- **Responsibilities**:
  - Identify wire-like objects
  - Extract wire paths
  - Calculate wire properties
  - Validate antenna geometry
- **Dependencies**: GeometryUtils

#### GeometryUtils
- **Purpose**: Geometric calculations and utilities
- **Responsibilities**:
  - Calculate distances, areas, volumes
  - Perform geometric transformations
  - Validate geometric properties
  - Provide geometric algorithms
- **Dependencies**: None (utility class)

### 3. Generation Layer

#### NECGenerator
- **Purpose**: Generate NEC format output files
- **Responsibilities**:
  - Convert STL data to NEC wire format
  - Generate NEC commands (GW, EX, GN, FR, RP)
  - Handle material properties
  - Support antenna and structure modeling
- **Dependencies**: MaterialDatabase, FrequencyCalculator

#### EZGenerator
- **Purpose**: Generate EZ format output files for 4NEC2
- **Responsibilities**:
  - Convert STL data to EZ format
  - Generate EZ commands
  - Handle 4NEC2-specific features
  - Support antenna and structure modeling
- **Dependencies**: MaterialDatabase, FrequencyCalculator

#### GroundModeler
- **Purpose**: Model ground plane effects
- **Responsibilities**:
  - Generate ground commands
  - Support different ground types
  - Handle marine environments
  - Validate ground parameters
- **Dependencies**: MaterialDatabase

### 4. Data Layer

#### MaterialDatabase
- **Purpose**: Manage electromagnetic material properties
- **Responsibilities**:
  - Store material properties
  - Provide material selection
  - Validate material data
  - Support different material types
- **Dependencies**: None

#### FrequencyCalculator
- **Purpose**: Calculate electromagnetic parameters
- **Responsibilities**:
  - Calculate wavelength from frequency
  - Determine grid spacing
  - Calculate segment counts
  - Validate frequency ranges
- **Dependencies**: None

#### Validator
- **Purpose**: Validate user input and data
- **Responsibilities**:
  - Validate STL files
  - Check material properties
  - Validate frequency ranges
  - Verify geometric properties
- **Dependencies**: GeometryUtils

### 5. Memory Management

#### MemoryManager
- **Purpose**: Monitor and manage memory usage
- **Responsibilities**:
  - Track memory usage
  - Enforce memory limits
  - Provide memory statistics
  - Optimize memory usage
- **Dependencies**: None

#### STLStreamProcessor
- **Purpose**: Process large STL files in chunks
- **Responsibilities**:
  - Stream STL data
  - Process files in chunks
  - Monitor memory usage
  - Handle large files efficiently
- **Dependencies**: MemoryManager

#### MemoryEfficientSTLParser
- **Purpose**: Memory-efficient STL processing
- **Responsibilities**:
  - Process large files without loading entirely
  - Provide streaming access
  - Optimize memory usage
  - Handle file statistics
- **Dependencies**: MemoryManager, STLStreamProcessor

## Data Flow

### 1. Input Processing
```
User Input → UserInterface → Validation → STLParser → Triangle Data
```

### 2. Antenna Detection
```
Triangle Data → AntennaDetector → GeometryUtils → AntennaWire
```

### 3. File Generation
```
Triangle Data + AntennaWire + Material + Frequency → NECGenerator/EZGenerator → Output Files
```

### 4. Memory Management
```
Large Files → STLStreamProcessor → MemoryManager → Chunked Processing
```

## Design Patterns

### 1. Singleton Pattern
- **Logger**: Single instance for application-wide logging
- **MemoryManager**: Single instance for memory management

### 2. Factory Pattern
- **STLStreamProcessor**: Created by MemoryManager
- **MaterialDatabase**: Creates MaterialProperties objects

### 3. Strategy Pattern
- **GroundModeler**: Different ground modeling strategies
- **STLParser**: Different parsing strategies for ASCII/binary

### 4. Observer Pattern
- **ProgressIndicator**: Observes processing progress
- **Logger**: Observes application events

## Error Handling Strategy

### 1. Input Validation
- **Validator**: Comprehensive input validation
- **UserInterface**: User-friendly error messages
- **STLParser**: File format validation

### 2. Processing Errors
- **Logger**: Log all errors with context
- **ProgressIndicator**: Show error status
- **MemoryManager**: Handle memory errors

### 3. Recovery Mechanisms
- **Graceful degradation**: Continue with partial data
- **User guidance**: Clear error messages and suggestions
- **Fallback options**: Alternative processing methods

## Performance Considerations

### 1. Memory Management
- **Streaming processing**: Handle large files without loading entirely
- **Memory limits**: Prevent out-of-memory errors
- **Chunked processing**: Process data in manageable chunks

### 2. Processing Optimization
- **Parallel processing**: Where possible and safe
- **Caching**: Cache frequently used calculations
- **Lazy evaluation**: Calculate values only when needed

### 3. User Experience
- **Progress indicators**: Show processing progress
- **Responsive interface**: Non-blocking operations
- **Clear feedback**: Inform users of current status

## Thread Safety

### 1. Thread-Safe Components
- **Logger**: Mutex-protected logging
- **MemoryManager**: Thread-safe memory monitoring
- **ProgressIndicator**: Thread-safe progress updates

### 2. Non-Thread-Safe Components
- **STLParser**: Single-threaded file processing
- **AntennaDetector**: Single-threaded detection
- **Generators**: Single-threaded file generation

### 3. Threading Strategy
- **Single-threaded by default**: Simpler and more reliable
- **Thread-safe logging**: For debugging and monitoring
- **Future parallelization**: Architecture supports future threading

## Extension Points

### 1. New File Formats
- **STLParser**: Extensible for new 3D formats
- **Generators**: Extensible for new output formats

### 2. New Material Types
- **MaterialDatabase**: Easy addition of new materials
- **MaterialProperties**: Extensible property system

### 3. New Ground Models
- **GroundModeler**: Pluggable ground modeling strategies
- **GroundParameters**: Extensible parameter system

### 4. New Antenna Types
- **AntennaDetector**: Extensible detection algorithms
- **GeometryUtils**: Extensible geometric operations

## Testing Strategy

### 1. Unit Testing
- **Individual components**: Test each class independently
- **Mock objects**: Isolate components for testing
- **Edge cases**: Test boundary conditions

### 2. Integration Testing
- **Component interaction**: Test component integration
- **Data flow**: Test complete processing pipelines
- **Error handling**: Test error propagation

### 3. Performance Testing
- **Memory usage**: Test with large files
- **Processing time**: Benchmark performance
- **Scalability**: Test with various file sizes

## Deployment Considerations

### 1. Cross-Platform Support
- **CMake**: Cross-platform build system
- **Standard C++**: Portable code
- **Platform-specific optimizations**: Where beneficial

### 2. Dependencies
- **Minimal dependencies**: Only standard library
- **Optional features**: Graceful degradation
- **Version compatibility**: Backward compatibility

### 3. Distribution
- **Static linking**: Self-contained executables
- **Dynamic linking**: Shared libraries where appropriate
- **Package management**: Support for package managers
