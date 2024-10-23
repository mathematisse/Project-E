#include "log.hpp"
#include <iostream>
#include <utility>

namespace log {

std::string logLevelToStr(Level level)
{
    switch (level) {
    case Level::DEBUG:
        return "DEBUG";
    case Level::INFO:
        return "INFO";
    case Level::WARNING:
        return "WARNING";
    case Level::ERROR:
        return "ERROR";
    case Level::CRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

Logger::Logger():
    log_level(Level::INFO),
    prefix_creator(createDefaultPrefix),
    log_stream(getDefaultLogStream()),
    max_log_size(DEFAULT_MAX_LOG_SIZE)
{
}

Logger::~Logger() { closeLogFile(); }

void Logger::log(Level level, const std::string &message)
{
    if (level < log_level) {
        return;
    }
    std::string log_line = prefix_creator(level) + message + "\n";
    if (log_stream != nullptr) {
        *log_stream << log_line;
    }
    if (logFileIsOpen()) {
        if (log_file.tellp() > max_log_size) {
            log_file.close();
            log_file.open(log_file_path, std::ios::out | std::ios::ate);
        }
        log_file << log_line;
        log_file.flush();
    }
}

log::Level Logger::getLogLevel() const { return log_level; }

void Logger::setLogLevel(log::Level level) { log_level = level; }

std::ostream *Logger::getLogStream() const { return log_stream; }

void Logger::setLogStream(std::ostream *stream) { log_stream = stream; }

void Logger::resetLogStream() { log_stream = getDefaultLogStream(); }

std::ostream *Logger::getDefaultLogStream() { return &std::cout; }

log::Logger::PrefixCreator Logger::getLogPrefix() const { return prefix_creator; }

void Logger::setLogPrefix(log::Logger::PrefixCreator prefixCreator)
{
    prefix_creator = std::move(prefixCreator);
}

void Logger::resetLogPrefix() { prefix_creator = createDefaultPrefix; }

std::string Logger::createDefaultPrefix(Level level) { return "[" + logLevelToStr(level) + "] "; }

void Logger::openLogFile(const std::filesystem::path &filePath)
{
    if (logFileIsOpen()) {
        if (log_file_path == filePath) {
            return;
        }
        closeLogFile();
    }
    log_file.open(filePath, std::ios::out | std::ios::app | std::ios::ate);
    log_file_path = filePath;
}

void Logger::closeLogFile()
{
    if (logFileIsOpen()) {
        log_file.close();
    }
}

bool Logger::logFileIsOpen() const { return log_file.is_open(); }

void Logger::setMaxLogSize(std::size_t size) { max_log_size = size; }

std::size_t Logger::getMaxLogSize() const { return max_log_size; }

}