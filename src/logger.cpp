#include "logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace stl_to_eznec {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() 
    : currentLevel_(LogLevel::INFO), consoleOutput_(true), fileOutput_(false) {
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex_);
    currentLevel_ = level;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex_);
    logFilename_ = filename;
    if (fileOutput_) {
        logFile_ = std::make_unique<std::ofstream>(filename, std::ios::app);
    }
}

void Logger::enableConsoleOutput(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex_);
    consoleOutput_ = enable;
}

void Logger::enableFileOutput(bool enable) {
    std::lock_guard<std::mutex> lock(logMutex_);
    fileOutput_ = enable;
    if (enable && !logFilename_.empty()) {
        logFile_ = std::make_unique<std::ofstream>(logFilename_, std::ios::app);
    } else {
        logFile_.reset();
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level >= currentLevel_) {
        writeLog(level, message);
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LogLevel::CRITICAL, message);
}

void Logger::logStep(const std::string& step, const std::string& details) {
    std::string message = "STEP: " + step;
    if (!details.empty()) {
        message += " - " + details;
    }
    info(message);
}

void Logger::logProgress(const std::string& task, int current, int total) {
    double percentage = static_cast<double>(current) / total * 100.0;
    std::string message = "PROGRESS: " + task + " (" + std::to_string(current) + 
                         "/" + std::to_string(total) + ", " + 
                         std::to_string(static_cast<int>(percentage)) + "%)";
    info(message);
}

void Logger::logError(const std::string& operation, const std::string& error) {
    std::string message = "ERROR in " + operation + ": " + error;
    this->error(message);
}

void Logger::logSuccess(const std::string& operation, const std::string& details) {
    std::string message = "SUCCESS: " + operation;
    if (!details.empty()) {
        message += " - " + details;
    }
    info(message);
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::formatLogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(logMutex_);
    if (logFile_ && logFile_->is_open()) {
        logFile_->flush();
    }
}

void Logger::writeLog(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);
    
    std::string formattedMessage = formatMessage(level, message);
    
    if (consoleOutput_) {
        std::cout << formattedMessage << std::endl;
    }
    
    if (fileOutput_ && logFile_ && logFile_->is_open()) {
        *logFile_ << formattedMessage << std::endl;
        logFile_->flush();
    }
}

std::string Logger::formatMessage(LogLevel level, const std::string& message) {
    std::stringstream ss;
    ss << "[" << getTimestamp() << "] ";
    ss << "[" << formatLogLevel(level) << "] ";
    ss << message;
    return ss.str();
}

} // namespace stl_to_eznec
