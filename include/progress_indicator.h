#pragma once

#include <string>
#include <iostream>
#include <chrono>

namespace stl_to_eznec {

class ProgressIndicator {
public:
    ProgressIndicator(const std::string& taskName, int totalSteps = 100);
    ~ProgressIndicator() = default;
    
    // Update progress
    void update(int currentStep);
    void update(double percentage);
    void setMessage(const std::string& message);
    
    // Complete the progress
    void complete();
    void complete(const std::string& finalMessage);
    
    // Static utility methods
    static void printStep(const std::string& step);
    static void printInfo(const std::string& info);
    static void printSuccess(const std::string& message);
    static void printWarning(const std::string& message);
    static void printError(const std::string& message);
    
    // Progress bar display
    void displayProgressBar();

private:
    std::string taskName_;
    int totalSteps_;
    int currentStep_;
    std::string currentMessage_;
    std::chrono::steady_clock::time_point startTime_;
    bool completed_;
    
    void printProgressBar();
    std::string formatTime(double seconds);
};

} // namespace stl_to_eznec
