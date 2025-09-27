#include "user_interface.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <cstdlib>

namespace stl_to_eznec {

UserInterface::UserInterface() : materialDB_(), waterDB_() {
}

UserInput UserInterface::getUserInput() {
    UserInput input;
    
    printWelcome();
    
    // Get STL filename
    input.stlFilename = getSTLFilename();
    
    // Get material selection
    input.material = getMaterialSelection();
    
    // Get frequency input
    input.frequencyMHz = getFrequencyInput();
    
    // Get vehicle type
    input.vehicleType = getVehicleType();
    
    // Get waterline height and water type for marine vehicles
    if (input.vehicleType == VehicleType::SHIP || input.vehicleType == VehicleType::BOAT) {
        input.waterlineHeight = getWaterlineHeight();
        input.waterProperties = getWaterType();
    }
    
    // Get output filenames
    input.outputNECFilename = getOutputNECFilename(input.stlFilename);
    input.outputEZFilename = getOutputEZFilename(input.stlFilename);
    input.modelName = getModelName(input.stlFilename);
    
    // Print conversion summary
    printConversionSummary(input);
    
    return input;
}

void UserInterface::printWelcome() {
    std::cout << "========================================\n";
    std::cout << "    STL to EZ/NEC Converter Tool\n";
    std::cout << "    Electromagnetic Vehicle Modeling\n";
    std::cout << "========================================\n\n";
    
    std::cout << "This tool converts STL files to EZ and NEC formats for\n";
    std::cout << "electromagnetic simulation of vehicles with antennas.\n\n";
    
    std::cout << "Supported vehicle types:\n";
    std::cout << "- Ships and boats (with waterline modeling)\n";
    std::cout << "- Airplanes and helicopters\n";
    std::cout << "- Cars and land vehicles\n\n";
}

std::string UserInterface::getSTLFilename() {
    std::string filename;
    
    while (true) {
        std::cout << "Enter STL filename (or 'quit' to exit): ";
        std::getline(std::cin, filename);
        
        // Check if user wants to quit
        if (filename == "quit" || filename == "exit" || filename == "q") {
            std::cout << "Exiting program.\n";
            exit(0);
        }
        
        if (validateSTLFilename(filename)) {
            break;
        }
        
        printError("Invalid STL file. Please enter a valid .stl filename or 'quit' to exit.");
    }
    
    return filename;
}

MaterialProperties UserInterface::getMaterialSelection() {
    materialDB_.printMaterialList();
    
    int selection;
    while (true) {
        std::cout << "Select material (1-" << materialDB_.getMaterialCount() << "): ";
        std::cin >> selection;
        
        if (materialDB_.isValidMaterialIndex(selection)) {
            break;
        }
        
        printError("Invalid selection. Please enter a number between 1 and " + 
                  std::to_string(materialDB_.getMaterialCount()));
    }
    
    std::cin.ignore(); // Clear newline
    
    return *materialDB_.getMaterial(selection - 1);
}

double UserInterface::getFrequencyInput() {
    printFrequencyHelp();
    
    double frequency;
    while (true) {
        std::cout << "Enter highest antenna frequency in MHz (0 if no antenna): ";
        std::cin >> frequency;
        
        if (validateFrequency(frequency)) {
            break;
        }
        
        printError("Invalid frequency. Please enter a positive number or 0 for no antenna.");
    }
    
    std::cin.ignore(); // Clear newline
    return frequency;
}

VehicleType UserInterface::getVehicleType() {
    printVehicleTypeHelp();
    
    int selection;
    while (true) {
        std::cout << "Select vehicle type (1-6): ";
        std::cin >> selection;
        
        if (selection >= 1 && selection <= 6) {
            break;
        }
        
        printError("Invalid selection. Please enter a number between 1 and 6.");
    }
    
    std::cin.ignore(); // Clear newline
    
    switch (selection) {
        case 1: return VehicleType::SHIP;
        case 2: return VehicleType::BOAT;
        case 3: return VehicleType::AIRPLANE;
        case 4: return VehicleType::HELICOPTER;
        case 5: return VehicleType::CAR;
        case 6: return VehicleType::LAND_VEHICLE;
        default: return VehicleType::UNKNOWN;
    }
}

double UserInterface::getWaterlineHeight() {
    printWaterlineHelp();
    
    double height;
    while (true) {
        std::cout << "Enter waterline-to-gunwale distance in meters: ";
        std::cin >> height;
        
        if (validateWaterlineHeight(height)) {
            break;
        }
        
        printError("Invalid height. Please enter a positive number.");
    }
    
    std::cin.ignore(); // Clear newline
    return height;
}

WaterProperties* UserInterface::getWaterType() {
    waterDB_.printWaterTypes();
    
    int selection;
    while (true) {
        std::cout << "Select water type (1-2): ";
        std::cin >> selection;
        
        if (selection == 1) {
            std::cin.ignore(); // Clear newline
            return const_cast<WaterProperties*>(waterDB_.getFreshWater());
        } else if (selection == 2) {
            std::cin.ignore(); // Clear newline
            return const_cast<WaterProperties*>(waterDB_.getSaltWater());
        }
        
        printError("Invalid selection. Please enter 1 or 2.");
    }
}

std::string UserInterface::getOutputNECFilename(const std::string& stlFilename) {
    std::string baseName = getBaseFilename(stlFilename);
    return baseName + ".nec";
}

std::string UserInterface::getOutputEZFilename(const std::string& stlFilename) {
    std::string baseName = getBaseFilename(stlFilename);
    return baseName + ".ez";
}

std::string UserInterface::getModelName(const std::string& stlFilename) {
    return getBaseFilename(stlFilename);
}

void UserInterface::printMaterialHelp() {
    std::cout << "\nMaterial properties are needed to set the material's electric properties\n";
    std::cout << "correctly for accurate electromagnetic simulation.\n\n";
}

void UserInterface::printFrequencyHelp() {
    std::cout << "\nThe highest frequency of antennas is needed to get the wire mesh spacing correct.\n";
    std::cout << "This ensures proper electromagnetic simulation accuracy.\n\n";
}

void UserInterface::printVehicleTypeHelp() {
    std::cout << "\n=== Vehicle Type Selection ===\n";
    std::cout << "1. Ship\n";
    std::cout << "2. Boat\n";
    std::cout << "3. Airplane\n";
    std::cout << "4. Helicopter\n";
    std::cout << "5. Car\n";
    std::cout << "6. Land Vehicle\n\n";
}

void UserInterface::printWaterlineHelp() {
    std::cout << "\n=== Waterline Information ===\n";
    std::cout << "For ships and boats, enter the distance from waterline to gunwale.\n";
    std::cout << "Use the distance for a fully loaded ship/boat for best results.\n\n";
}

void UserInterface::printAntennaDetectionResult(const AntennaWire& antenna) {
    if (antenna.isDetected) {
        printInfo("Antenna wire detected in the model!");
        std::cout << "Length: " << std::fixed << std::setprecision(3) << antenna.length << " m\n";
        std::cout << "Radius: " << std::fixed << std::setprecision(3) << antenna.radius << " m\n";
    } else {
        printInfo("No antenna wire detected in the model.");
    }
}

bool UserInterface::getAntennaConfirmation(bool antennaDetected) {
    if (antennaDetected) {
        std::cout << "\nAntenna wire detected! Use this antenna? (y/n): ";
    } else {
        std::cout << "\nNo antenna detected. Continue without antenna? (y/n): ";
    }
    
    std::string response;
    std::getline(std::cin, response);
    
    return (response == "y" || response == "Y" || response == "yes" || response == "YES");
}

void UserInterface::printConversionSummary(const UserInput& input) {
    std::cout << "\n=== Conversion Summary ===\n";
    std::cout << "STL file: " << input.stlFilename << "\n";
    std::cout << "Material: " << input.material.name << "\n";
    std::cout << "Frequency: " << std::fixed << std::setprecision(1) << input.frequencyMHz << " MHz\n";
    std::cout << "Vehicle type: " << static_cast<int>(input.vehicleType) << "\n";
    
    if (input.vehicleType == VehicleType::SHIP || input.vehicleType == VehicleType::BOAT) {
        std::cout << "Waterline height: " << std::fixed << std::setprecision(2) << input.waterlineHeight << " m\n";
        std::cout << "Water type: " << input.waterProperties->type << "\n";
    }
    
    std::cout << "Output files:\n";
    std::cout << "  NEC: " << input.outputNECFilename << "\n";
    std::cout << "  EZ: " << input.outputEZFilename << "\n\n";
}

void UserInterface::printError(const std::string& message) {
    std::cout << "ERROR: " << message << "\n";
}

void UserInterface::printSuccess(const std::string& message) {
    std::cout << "SUCCESS: " << message << "\n";
}

void UserInterface::printInfo(const std::string& message) {
    std::cout << "INFO: " << message << "\n";
}

bool UserInterface::validateSTLFilename(const std::string& filename) {
    if (filename.empty()) return false;
    
    std::string ext = getFileExtension(filename);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return (ext == ".stl" && fileExists(filename));
}

bool UserInterface::validateFrequency(double frequency) {
    return frequency >= 0.0;
}

bool UserInterface::validateWaterlineHeight(double height) {
    return height > 0.0;
}

std::string UserInterface::getFileExtension(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return "";
    return filename.substr(dotPos);
}

std::string UserInterface::getBaseFilename(const std::string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) return filename;
    return filename.substr(0, dotPos);
}

std::string UserInterface::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool UserInterface::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

} // namespace stl_to_eznec
