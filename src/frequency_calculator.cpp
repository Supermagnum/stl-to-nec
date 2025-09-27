#include "frequency_calculator.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace stl_to_eznec {

FrequencyCalculator::FrequencyCalculator() 
    : frequencyMHz_(0.0), wavelength_(0.0) {
}

void FrequencyCalculator::setFrequency(double frequencyMHz) {
    frequencyMHz_ = frequencyMHz;
    calculateWavelength();
}

void FrequencyCalculator::calculateWavelength() {
    if (frequencyMHz_ > 0) {
        wavelength_ = SPEED_OF_LIGHT / (frequencyMHz_ * 1e6);
    } else {
        wavelength_ = 0.0;
    }
}

int FrequencyCalculator::calculateSegments(double wireLength, double gridSpacing) const {
    if (gridSpacing <= 0) return 1;
    return static_cast<int>(std::ceil(wireLength / gridSpacing));
}

int FrequencyCalculator::calculateSegmentsRecommended(double wireLength) const {
    return calculateSegments(wireLength, getRecommendedGridSpacing());
}

void FrequencyCalculator::printFrequencyInfo() const {
    std::cout << "\n=== Frequency Information ===\n";
    std::cout << "Frequency: " << std::fixed << std::setprecision(1) << frequencyMHz_ << " MHz\n";
    std::cout << "Wavelength: " << std::fixed << std::setprecision(3) << wavelength_ << " m (";
    std::cout << std::fixed << std::setprecision(1) << wavelength_ * 100.0 << " cm)\n";
    std::cout << "Band: " << getFrequencyBand() << "\n\n";
    
    std::cout << "Grid Spacing Options:\n";
    std::cout << "  High Accuracy (λ/20): " << std::fixed << std::setprecision(1) << getHighAccuracyGridSpacingCm() << " cm\n";
    std::cout << "  Standard Accuracy (λ/10): " << std::fixed << std::setprecision(1) << getStandardAccuracyGridSpacingCm() << " cm\n";
    std::cout << "  Recommended (5cm): " << std::fixed << std::setprecision(1) << getRecommendedGridSpacingCm() << " cm\n\n";
    
    std::cout << "The highest frequency of antennas is needed to get the wire mesh spacing correct.\n";
    std::cout << "This ensures proper electromagnetic simulation accuracy.\n\n";
}

std::string FrequencyCalculator::getFrequencyBand() const {
    return getBandName(frequencyMHz_);
}

std::string FrequencyCalculator::getBandName(double frequencyMHz) const {
    if (frequencyMHz >= 3.0 && frequencyMHz <= 30.0) {
        return "HF (3-30 MHz)";
    } else if (frequencyMHz >= 30.0 && frequencyMHz <= 300.0) {
        return "VHF (30-300 MHz)";
    } else if (frequencyMHz >= 300.0 && frequencyMHz <= 3000.0) {
        return "UHF (300-3000 MHz)";
    } else if (frequencyMHz >= 3000.0 && frequencyMHz <= 30000.0) {
        return "SHF (3-30 GHz)";
    } else if (frequencyMHz >= 0.1 && frequencyMHz < 3.0) {
        return "MF (0.1-3 MHz)";
    } else if (frequencyMHz >= 0.01 && frequencyMHz < 0.1) {
        return "LF (0.01-0.1 MHz)";
    } else if (frequencyMHz >= 0.001 && frequencyMHz < 0.01) {
        return "VLF (0.001-0.01 MHz)";
    } else {
        return "Unknown Band";
    }
}

} // namespace stl_to_eznec
