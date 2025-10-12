# STL to EZ/NEC Converter

A cross-platform tool for converting STL files to EZ and NEC formats for electromagnetic simulation of vehicles with antennas. This tool bridges the gap between 3D CAD models and electromagnetic simulation software, enabling accurate antenna modeling on vehicles.


It compiles, but it is not tested.

## What This Tool Does

This tool converts 3D STL models of vehicles (ships, boats, airplanes, cars, etc.) into electromagnetic simulation files that can be analyzed for antenna performance. It automatically:

- **Detects antenna wires** in your 3D model
- **Calculates proper material properties** for electromagnetic simulation
- **Determines correct grid spacing** based on operating frequency
- **Generates simulation files** compatible with industry-standard software

## Features

- **Cross-platform**: Supports Linux, Windows, and macOS
- **Vehicle Support**: Ships, boats, airplanes, helicopters, cars, and land vehicles
- **Material Properties**: Comprehensive database of vehicle materials with electromagnetic properties
- **Antenna Detection**: Automatic detection of antenna wires in STL models
- **Frequency Analysis**: Proper grid spacing calculation based on operating frequency
- **Marine Modeling**: Special support for ships/boats with waterline and water properties
- **Output Formats**: Generates both NEC and EZ files for different simulation software

## Supported Materials

- **Aluminum** (aircraft, car parts): σ = 1.5×10⁷ S/m, εᵣ = 1.0
- **Mild Steel** (car bodies, ship hulls): σ = 7.0×10⁶ S/m, εᵣ = 1.0
- **Stainless Steel** (marine, aerospace): σ = 1.2×10⁶ S/m, εᵣ = 1.0
- **Galvanized Steel** (common vehicles): σ = 4.0×10⁶ S/m, εᵣ = 1.0
- **Spring Steel** (suspension, landing gear): σ = 3.0×10⁶ S/m, εᵣ = 1.0
- **Concrete** (buildings): σ = 0.5 S/m, εᵣ = 8.0

## Water Properties

- **Fresh Water**: σ = 0.001 S/m, εᵣ = 81.0
- **Salt Water**: σ = 4.5 S/m, εᵣ = 81.0

## Grid Spacing Rules

The tool implements proper electromagnetic simulation grid spacing:

- **High Accuracy (λ/20)**: 3.5 cm at 430 MHz
- **Standard Accuracy (λ/10)**: 7 cm at 430 MHz  
- **Recommended (5 cm)**: Balanced accuracy and performance

## Building

### Prerequisites

- CMake 3.16 or later
- C++17 compatible compiler
- No external dependencies required

### Quick Build (Linux/macOS)

```bash
./build.sh
```

### Manual Build

#### Linux/macOS

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

#### Windows

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### macOS (Xcode)

```bash
mkdir build
cd build
cmake .. -G Xcode
cmake --build . --config Release
```

## Usage

### Basic Usage

```bash
./stl-to-eznec
```

The tool will guide you through an interactive process:

1. **STL File Selection**: Choose your 3D model file
2. **Material Selection**: Select the appropriate vehicle material
3. **Frequency Input**: Enter the highest antenna frequency (or 0 for no antenna)
4. **Vehicle Type**: Specify the type of vehicle
5. **Marine Parameters**: For ships/boats, enter waterline height and water type
6. **Scaling**: Optionally scale the model to correct dimensions

### Example Session

```
========================================
    STL to EZ/NEC Converter Tool
    Electromagnetic Vehicle Modeling
========================================

Enter STL filename: my_ship.stl
Loading STL file: my_ship.stl
STL file loaded successfully.
Triangles: 12543
Bounding box: (-2.5, -1.2, 0.0) to (2.5, 1.2, 3.8)
Size: 5.0 x 2.4 x 3.8 m

Enter the actual length of the object in meters: 50.0
Model scaled to 50.0 m length.

=== Material Selection ===
Select the material type for your vehicle:
1. Aluminum (σ = 1.5e+07 S/m, εᵣ = 1.0)
2. Mild Steel (σ = 7.0e+06 S/m, εᵣ = 1.0)
...

Select material (1-6): 2
Enter highest antenna frequency in MHz (0 if no antenna): 430.0
Select vehicle type (1-6): 1
Enter waterline-to-gunwale distance in meters: 2.5
Select water type (1-2): 2

SUCCESS: NEC file generated: my_ship.nec
SUCCESS: EZ file generated: my_ship.ez
```

## Output Files

The tool generates two output files:

- **`.nec`**: NEC format for electromagnetic simulation
- **`.ez`**: EZ format for 4NEC2 compatibility

**Important**: The generated files may need checking and final adjustment in a suitable EZNEC program before running simulations.

## Compatible Simulation Software

### Windows
- **4NEC2** (Free) - Most popular NEC-based antenna modeling software
- **EZNEC** (Commercial) - Professional antenna modeling
- **NEC-Win Plus** (Commercial) - Advanced NEC modeling
- **MMANA-GAL** (Free) - Antenna modeling and optimization

### Linux
- **4NEC2** (via Wine) - Run Windows version
- **xnecview** (Free) - NEC file viewer and analyzer
- **nec2c** (Free) - Command-line NEC-2 engine
- **Python NEC** (Free) - Python-based NEC implementation

### macOS
- **4NEC2** (via Wine or CrossOver) - Run Windows version
- **EZNEC** (Commercial) - Native macOS version available
- **Python NEC** (Free) - Python-based NEC implementation
- **xnecview** (Free) - Via Homebrew or MacPorts

## File Format Details

### NEC Format
- Industry standard for electromagnetic simulation
- Compatible with NEC-2, NEC-4, and other NEC-based tools
- Includes proper material properties and segmentation
- Used by professional antenna modeling software

### EZ Format  
- Compatible with 4NEC2 software
- Includes all necessary simulation parameters
- Ready for immediate use in 4NEC2
- Popular among amateur radio operators

## Technical Details

### Antenna Detection

The tool automatically detects thin wire-like objects in the STL model that could be antennas. Detection parameters:

- Maximum wire diameter: 1 cm
- Minimum antenna length: 10 cm
- Maximum antenna length: 10 m

### Frequency Analysis

For proper electromagnetic simulation, the tool calculates:

- Wavelength at operating frequency
- Appropriate grid spacing (λ/20 to λ/10)
- Number of segments for wire modeling
- Frequency band identification

### Marine Modeling

For ships and boats, the tool includes:

- Waterline-to-gunwale distance input
- Water type selection (fresh/salt)
- Proper ground plane modeling with water properties

## Error Handling

The tool exits gracefully in the following situations:

- **File not found**: If the STL file doesn't exist, the tool will prompt for a valid file
- **Invalid input**: If user provides invalid input, the tool will ask for correction
- **No antenna detected**: If no antenna is found, the tool will continue with structure-only modeling
- **Conversion complete**: After successful file generation, the tool exits normally

## Example Workflow

1. **Load STL**: Import your vehicle 3D model
2. **Scale Model**: Enter actual dimensions in meters
3. **Select Material**: Choose appropriate vehicle material
4. **Set Frequency**: Enter highest antenna frequency
5. **Detect Antenna**: Tool automatically finds antenna wires
6. **Generate Files**: Creates NEC and EZ simulation files
7. **Import to EZNEC**: Load the generated files into your preferred simulation software
8. **Verify and Adjust**: Check the model in the simulation software and make final adjustments
9. **Run Simulation**: Perform electromagnetic analysis

## Troubleshooting

### Common Issues

- **STL file not found**: Ensure the file exists and has a `.stl` extension
- **No antenna detected**: Check that your model contains thin wire-like objects
- **Scaling issues**: Verify that you enter the correct actual dimensions
- **Material selection**: Choose the material that best matches your vehicle

### Getting Help

If you encounter issues:

1. Check that your STL file is valid and contains the expected geometry
2. Ensure you're using the correct material properties for your vehicle
3. Verify that the frequency you enter matches your intended use
4. For marine vehicles, ensure waterline height is accurate

## License

This project is open source. See LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Support

For technical support or questions, please open an issue on the project repository.
