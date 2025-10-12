#include "progress_indicator.h"
#include <iomanip>
#include <sstream>

namespace stl_to_eznec {

ProgressIndicator::ProgressIndicator(const std::string& taskName, int totalSteps)
    : taskName_(taskName), totalSteps_(totalSteps), currentStep_(0), 
      startTime_(std::chrono::steady_clock::now()), completed_(false) {
    
    std::cout << "\nStarting: " << taskName_ << "\n";
    std::cout << "Progress: [";
    for (int i = 0; i < 50; ++i) {
        std::cout << " ";
    }
    std::cout << "] 0%\r" << std::flush;
}

void ProgressIndicator::update(int currentStep) {
    currentStep_ = currentStep;
    displayProgressBar();
}

void ProgressIndicator::update(double percentage) {
    currentStep_ = static_cast<int>(percentage * totalSteps_ / 100.0);
    displayProgressBar();
}

void ProgressIndicator::setMessage(const std::string& message) {
    currentMessage_ = message;
    displayProgressBar();
}

void ProgressIndicator::complete() {
    completed_ = true;
    currentStep_ = totalSteps_;
    displayProgressBar();
    std::cout << "\n";
}

void ProgressIndicator::complete(const std::string& finalMessage) {
    completed_ = true;
    currentStep_ = totalSteps_;
    displayProgressBar();
    std::cout << "\n";
    printSuccess(finalMessage);
}

void ProgressIndicator::displayProgressBar() {
    if (completed_) return;
    
    double percentage = static_cast<double>(currentStep_) / totalSteps_ * 100.0;
    int barLength = static_cast<int>(percentage / 2.0); // 50 character bar
    
    std::cout << "\rProgress: [";
    for (int i = 0; i < 50; ++i) {
        if (i < barLength) {
            std::cout << "=";
        } else {
            std::cout << " ";
        }
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << percentage << "%";
    
    if (!currentMessage_.empty()) {
        std::cout << " - " << currentMessage_;
    }
    
    // Calculate elapsed time
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime_);
    std::cout << " (" << formatTime(elapsed.count() / 1000.0) << ")";
    
    std::cout << std::flush;
}

void ProgressIndicator::printProgressBar() {
    displayProgressBar();
}

std::string ProgressIndicator::formatTime(double seconds) {
    if (seconds < 60) {
        return std::to_string(static_cast<int>(seconds)) + "s";
    } else if (seconds < 3600) {
        int minutes = static_cast<int>(seconds / 60);
        int secs = static_cast<int>(seconds) % 60;
        return std::to_string(minutes) + "m " + std::to_string(secs) + "s";
    } else {
        int hours = static_cast<int>(seconds / 3600);
        int minutes = static_cast<int>((seconds - hours * 3600) / 60);
        return std::to_string(hours) + "h " + std::to_string(minutes) + "m";
    }
}

void ProgressIndicator::printStep(const std::string& step) {
    std::cout << "  -> " << step << "\n";
}

void ProgressIndicator::printInfo(const std::string& info) {
    std::cout << "  [INFO] " << info << "\n";
}

void ProgressIndicator::printSuccess(const std::string& message) {
    std::cout << "  [SUCCESS] " << message << "\n";
}

void ProgressIndicator::printWarning(const std::string& message) {
    std::cout << "  [WARNING] " << message << "\n";
}

void ProgressIndicator::printError(const std::string& message) {
    std::cout << "  [ERROR] " << message << "\n";
}

} // namespace stl_to_eznec
