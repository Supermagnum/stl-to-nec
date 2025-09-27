#pragma once

#include <string>

namespace stl_to_eznec {

class FrequencyCalculator {
public:
    FrequencyCalculator();
    
    // Set frequency in MHz
    void setFrequency(double frequencyMHz);
    
    // Get wavelength in meters
    double getWavelength() const { return wavelength_; }
    
    // Get wavelength in cm
    double getWavelengthCm() const { return wavelength_ * 100.0; }
    
    // Get high accuracy grid spacing (λ/20) in meters
    double getHighAccuracyGridSpacing() const { return wavelength_ / 20.0; }
    
    // Get standard accuracy grid spacing (λ/10) in meters  
    double getStandardAccuracyGridSpacing() const { return wavelength_ / 10.0; }
    
    // Get recommended grid spacing (5cm for amateur radio) in meters
    double getRecommendedGridSpacing() const { return 0.05; }
    
    // Get grid spacing in cm
    double getHighAccuracyGridSpacingCm() const { return getHighAccuracyGridSpacing() * 100.0; }
    double getStandardAccuracyGridSpacingCm() const { return getStandardAccuracyGridSpacing() * 100.0; }
    double getRecommendedGridSpacingCm() const { return getRecommendedGridSpacing() * 100.0; }
    
    // Calculate number of segments for a wire of given length
    int calculateSegments(double wireLength, double gridSpacing) const;
    
    // Calculate number of segments using recommended spacing
    int calculateSegmentsRecommended(double wireLength) const;
    
    // Get frequency in MHz
    double getFrequencyMHz() const { return frequencyMHz_; }
    
    // Get frequency in Hz
    double getFrequencyHz() const { return frequencyMHz_ * 1e6; }
    
    // Check if frequency is valid
    bool isValidFrequency() const { return frequencyMHz_ > 0; }
    
    // Print frequency information
    void printFrequencyInfo() const;
    
    // Get frequency band name
    std::string getFrequencyBand() const;

private:
    double frequencyMHz_;
    double wavelength_;
    static constexpr double SPEED_OF_LIGHT = 299792458.0; // m/s
    
    void calculateWavelength();
    std::string getBandName(double frequencyMHz) const;
};

} // namespace stl_to_eznec
