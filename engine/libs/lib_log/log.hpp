#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <filesystem>
#include <functional>
#include <fstream>

namespace log {

#ifdef LOGGING

#define LOG(level, message) (log::global_logger.log(level, message))
#define LOG_DEBUG(message) LOG(log::Level::DEBUG, message)
#define LOG_INFO(message) LOG(log::Level::INFO, message)
#define LOG_WARNING(message) LOG(log::Level::WARNING, message)
#define LOG_ERROR(message) LOG(log::Level::ERROR, message)
#define LOG_CRITICAL(message) LOG(log::Level::CRITICAL, message)
#define LOG_SET_FILE(file_path) (log::global_logger.openLogFile(file_path))
#define LOG_SET_LEVEL(level) (log::global_logger.setLogLevel(level))
#define LOG_SET_STREAM(stream) (log::global_logger.setLogStream(&(stream)))
#define LOG_SET_PREFIX(prefix_creator) (log::global_logger.setLogPrefix(prefix_creator))

#else

#define LOG(level, message)
#define LOG_DEBUG(message)
#define LOG_INFO(message)
#define LOG_WARNING(message)
#define LOG_ERROR(message)
#define LOG_CRITICAL(message)
#define LOG_SET_FILE(file_path)
#define LOG_SET_LEVEL(level)
#define LOG_SET_STREAM(stream)
#define LOG_SET_PREFIX(prefix_creator)

#endif

enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

std::string logLevelToStr(Level level);

class Logger {
public:
    using PrefixCreator = std::function<std::string(Level)>;

    Logger();
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(Logger &&) = delete;

    void log(Level level, const std::string &message);

    [[nodiscard]] Level getLogLevel() const;
    void setLogLevel(Level level);

    [[nodiscard]] std::ostream *getLogStream() const;
    void setLogStream(std::ostream *stream);
    void resetLogStream();
    static std::ostream *getDefaultLogStream();

    [[nodiscard]] PrefixCreator getLogPrefix() const;
    void setLogPrefix(PrefixCreator prefixCreator);
    void resetLogPrefix();
    static std::string createDefaultPrefix(Level level);

    bool openLogFile(const std::filesystem::path &filePath);
    void closeLogFile();
    [[nodiscard]] bool logFileIsOpen() const;

private:
    Level log_level;
    PrefixCreator prefix_creator;
    std::ostream *log_stream;
    std::filesystem::path log_file_path;
    std::ofstream log_file;
};

static Logger global_logger;

}