#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <filesystem>
#include <functional>
#include <fstream>

namespace rlog {

#ifdef LOGGING

#define LOG(level, message) (rlog::global_logger.log(level, message))
#define LOG_DEBUG(message) LOG(rlog::Level::DEBUG, message)
#define LOG_INFO(message) LOG(rlog::Level::INFO, message)
#define LOG_WARNING(message) LOG(rlog::Level::WARNING, message)
#define LOG_ERROR(message) LOG(rlog::Level::ERROR, message)
#define LOG_CRITICAL(message) LOG(rlog::Level::CRITICAL, message)
#define LOG_SET_FILE(file_path) (rlog::global_logger.openLogFile(file_path))
#define LOG_SET_LEVEL(level_name) (rlog::global_logger.setLogLevel(rlog::Level::level_name))
#define LOG_SET_STREAM(stream) (rlog::global_logger.setLogStream(&(stream)))
#define LOG_SET_PREFIX(prefix_creator) (rlog::global_logger.setLogPrefix(prefix_creator))

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

#define LOG_PINK "\033[35m"
#define LOG_GREEN "\033[32m"
#define LOG_YELLOW "\033[33m"
#define LOG_RED "\033[31m"
#define LOG_COLOR_RESET "\033[0m"

#define LOG_BOLD "\033[1m"

enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

std::string logLevelToStr(Level level);
std::string logLevelToColor(Level level);

class Logger {
public:
    using PrefixCreator = std::function<std::string(Level)>;
    constexpr static std::size_t DEFAULT_MAX_LOG_SIZE = 104857600; // 100 MB

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

    void openLogFile(const std::filesystem::path &filePath);
    void closeLogFile();
    [[nodiscard]] bool logFileIsOpen() const;
    void setMaxLogSize(std::size_t size);
    [[nodiscard]] std::size_t getMaxLogSize() const;

private:
    Level log_level;
    PrefixCreator prefix_creator;
    std::ostream *log_stream;
    std::filesystem::path log_file_path;
    std::ofstream log_file;
    std::size_t max_log_size;
};

static Logger global_logger;

}