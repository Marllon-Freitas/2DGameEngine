#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_SUCCESS,
	LOG_ERROR,
	LOG_DEFAULT
};

struct LogEntry {
	LogType type;
	std::string message;
};

class Logger {
	public:
		static std::vector<LogEntry> messages;
		static void Log(const std::string& message);
		static void Info(const std::string& message);
		static void Success(const std::string& message);
		static void Error(const std::string& message);
		static void Warning(const std::string& message);
};

#endif