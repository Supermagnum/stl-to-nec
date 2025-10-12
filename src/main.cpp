#include <iostream>
#include <fstream>
#include <string>
#include "stl_parser.h"
#include "material_database.h"
#include "frequency_calculator.h"
#include "antenna_detector.h"
#include "nec_generator.h"
#include "ez_generator.h"
#include "user_interface.h"

using namespace stl_to_eznec;

int main() {
    try {
        // Initialize components
        UserInterface ui;
        STLParser parser;
        AntennaDetector detector;
        FrequencyCalculator frequency;
        NECGenerator necGen;
        EZGenerator ezGen;
        
        // Get user input
        UserInput input = ui.getUserInput();
        
        // Load and parse STL file
        std::cout << "Loading STL file: " << input.stlFilename << "\n";
        if (!parser.loadFile(input.stlFilename)) {
            ui.printError("Failed to load STL file: " + parser.getErrorMessage());
            std::cout << "\nPlease ensure the file exists and is a valid STL file.\n";
            std::cout << "The program will now exit.\n";
            return 1;
        }
        
        // Get triangles and scale if needed
        auto triangles = parser.getTriangles();
        BoundingBox bbox = parser.getBoundingBox();
        
        std::cout << "STL file loaded successfully.\n";
        std::cout << "Triangles: " << triangles.size() << "\n";
        std::cout << "Bounding box: (" << bbox.min.x << ", " << bbox.min.y << ", " << bbox.min.z << ") to (";
        std::cout << bbox.max.x << ", " << bbox.max.y << ", " << bbox.max.z << ")\n";
        std::cout << "Size: " << bbox.size().x << " x " << bbox.size().y << " x " << bbox.size().z << " m\n\n";
        
        // Ask user for scaling if needed
        std::cout << "Enter the actual length of the object in meters (or press Enter to keep current scale): ";
        std::string scaleInput;
        std::getline(std::cin, scaleInput);
        
        if (!scaleInput.empty()) {
            double targetLength = std::stod(scaleInput);
            parser.scaleToLength(targetLength);
            triangles = parser.getTriangles();
            bbox = parser.getBoundingBox();
            
            std::cout << "Model scaled to " << targetLength << " m length.\n";
            std::cout << "New size: " << bbox.size().x << " x " << bbox.size().y << " x " << bbox.size().z << " m\n\n";
        }
        
        // Set frequency
        if (input.frequencyMHz > 0) {
            frequency.setFrequency(input.frequencyMHz);
            frequency.printFrequencyInfo();
        }
        
        // Detect antenna (if enabled)
        AntennaWire antenna;
        bool hasAntenna = false;
        
        if (input.enableAntennaDetection) {
            antenna = detector.detectAntenna(triangles);
            ui.printAntennaDetectionResult(antenna);
            
            // Confirm antenna detection
            if (antenna.isDetected) {
                hasAntenna = ui.getAntennaConfirmation(true);
            } else {
                hasAntenna = ui.getAntennaConfirmation(false);
            }
        } else {
            std::cout << "Antenna detection disabled. Processing structure only.\n";
            antenna = AntennaWire(); // Empty antenna
            hasAntenna = false;
        }
        
        // Generate NEC file
        std::cout << "Generating NEC file: " << input.outputNECFilename << "\n";
        std::string necContent;
        
        if (hasAntenna && antenna.isDetected) {
            necContent = necGen.generateNEC(
                triangles, input.material, frequency, antenna,
                input.modelName, true, input.waterlineHeight, input.waterProperties
            );
        } else {
            necContent = necGen.generateNECStructureOnly(triangles, input.material, input.modelName);
        }
        
        // Write NEC file
        std::ofstream necFile(input.outputNECFilename);
        if (necFile.is_open()) {
            necFile << necContent;
            necFile.close();
            ui.printSuccess("NEC file generated: " + input.outputNECFilename);
        } else {
            ui.printError("Failed to write NEC file: " + input.outputNECFilename);
        }
        
        // Generate EZ file
        std::cout << "Generating EZ file: " << input.outputEZFilename << "\n";
        std::string ezContent;
        
        if (hasAntenna && antenna.isDetected) {
            ezContent = ezGen.generateEZ(
                triangles, input.material, frequency, antenna,
                input.modelName, true, input.waterlineHeight, input.waterProperties
            );
        } else {
            ezContent = ezGen.generateEZStructureOnly(triangles, input.material, input.modelName);
        }
        
        // Write EZ file
        std::ofstream ezFile(input.outputEZFilename);
        if (ezFile.is_open()) {
            ezFile << ezContent;
            ezFile.close();
            ui.printSuccess("EZ file generated: " + input.outputEZFilename);
        } else {
            ui.printError("Failed to write EZ file: " + input.outputEZFilename);
        }
        
        // Print summary
        std::cout << "\n=== Conversion Complete ===\n";
        std::cout << "Input: " << input.stlFilename << "\n";
        std::cout << "Output: " << input.outputNECFilename << ", " << input.outputEZFilename << "\n";
        std::cout << "Material: " << input.material.name << "\n";
        if (input.frequencyMHz > 0) {
            std::cout << "Frequency: " << input.frequencyMHz << " MHz\n";
        }
        if (hasAntenna && antenna.isDetected) {
            std::cout << "Antenna: " << antenna.length << " m length, " << antenna.radius << " m radius\n";
        } else {
            std::cout << "Antenna: None detected\n";
        }
        
        std::cout << "\nFiles are ready for electromagnetic simulation!\n";
        std::cout << "Note: The generated files may need checking and final adjustment\n";
        std::cout << "in a suitable EZNEC program before running simulations.\n";
        std::cout << "\nConversion completed successfully. Program exiting.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Program will now exit.\n";
        return 1;
    }
    
    return 0;
}
