#pragma once

#include <string>
#include <vector>
#include "material_database.h"
#include "frequency_calculator.h"
#include "antenna_detector.h"

namespace stl_to_eznec {

enum class VehicleType {
    UNKNOWN,
    SHIP,
    BOAT,
    AIRPLANE,
    HELICOPTER,
    CAR,
    LAND_VEHICLE
};

struct UserInput {
    std::string stlFilename;
    MaterialProperties material;
    double frequencyMHz;
    bool hasAntenna;
    VehicleType vehicleType;
    double waterlineHeight;  // For ships/boats only
    WaterProperties* waterProperties;  // For ships/boats only
    std::string outputNECFilename;
    std::string outputEZFilename;
    std::string modelName;
    
    UserInput() : frequencyMHz(0), hasAntenna(false), vehicleType(VehicleType::UNKNOWN), 
                  waterlineHeight(0), waterProperties(nullptr) {}
};

class UserInterface {
public:
    UserInterface();
    
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
    
    // Print welcome message
    void printWelcome();
    
    // Print material selection help
    void printMaterialHelp();
    
    // Print frequency help
    void printFrequencyHelp();
    
    // Print vehicle type help
    void printVehicleTypeHelp();
    
    // Print waterline help
    void printWaterlineHelp();
    
    // Print antenna detection result
    void printAntennaDetectionResult(const AntennaWire& antenna);
    
    // Print conversion summary
    void printConversionSummary(const UserInput& input);
    
    // Print error message
    void printError(const std::string& message);
    
    // Print success message
    void printSuccess(const std::string& message);
    
    // Print information message
    void printInfo(const std::string& message);

private:
    MaterialDatabase materialDB_;
    WaterDatabase waterDB_;
    
    // Input validation
    bool validateSTLFilename(const std::string& filename);
    bool validateFrequency(double frequency);
    bool validateWaterlineHeight(double height);
    
    // Helper functions
    std::string getFileExtension(const std::string& filename);
    std::string getBaseFilename(const std::string& filename);
    std::string toLowerCase(const std::string& str);
    bool fileExists(const std::string& filename);
    
    // User input helpers
    std::string getStringInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);
    bool getYesNoInput(const std::string& prompt);
    int getMenuSelection(const std::string& prompt, int min, int max);
};

} // namespace stl_to_eznec
