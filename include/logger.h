#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <mutex>

namespace stl_to_eznec {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};

class Logger {
public:
    static Logger& getInstance();
    
    // Configuration
    void setLogLevel(LogLevel level);
    void setLogFile(const std::string& filename);
    void enableConsoleOutput(bool enable);
    void enableFileOutput(bool enable);
    
    // Logging methods
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);
    
    // Structured logging
    void logStep(const std::string& step, const std::string& details = "");
    void logProgress(const std::string& task, int current, int total);
    void logError(const std::string& operation, const std::string& error);
    void logSuccess(const std::string& operation, const std::string& details = "");
    
    // Utility methods
    std::string getTimestamp();
    std::string formatLogLevel(LogLevel level);
    void flush();

private:
    Logger();
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    LogLevel currentLevel_;
    std::string logFilename_;
    bool consoleOutput_;
    bool fileOutput_;
    std::unique_ptr<std::ofstream> logFile_;
    std::mutex logMutex_;
    
    void writeLog(LogLevel level, const std::string& message);
    std::string formatMessage(LogLevel level, const std::string& message);
};

// Convenience macros
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
#define LOG_CRITICAL(msg) Logger::getInstance().critical(msg)

#define LOG_STEP(step, details) Logger::getInstance().logStep(step, details)
#define LOG_PROGRESS(task, current, total) Logger::getInstance().logProgress(task, current, total)
#define LOG_ERROR_DETAIL(operation, error) Logger::getInstance().logError(operation, error)
#define LOG_SUCCESS(operation, details) Logger::getInstance().logSuccess(operation, details)

} // namespace stl_to_eznec
