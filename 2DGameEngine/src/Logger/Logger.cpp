#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include "Logger.h"

std::vector<LogEntry> Logger::messages;

std::string CurrentDateTimeToString() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime;
    localtime_s(&localTime, &now);
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", &localTime);
    return output;
}

void Logger::Info(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "INFO: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[34m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::Success(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_SUCCESS;
    logEntry.message = "SUCCESS: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::Error(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERROR: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[31m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::Warning(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_WARNING;
    logEntry.message = "WARNING: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[33m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::Log(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_DEFAULT;
    logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[37m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}