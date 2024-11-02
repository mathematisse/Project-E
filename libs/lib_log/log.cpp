#ifdef LOGGING

#include <iostream>
#include <utility>
#include "log.hpp"

logging::Logger logging::global_logger;

std::string logging::levelToStr(Level level)
{
    switch (level) {
    case Level::LDEBUG:
        return "DEBUG";
    case Level::LINFO:
        return "INFO";
    case Level::LWARNING:
        return "WARNING";
    case Level::LERROR:
        return "ERROR";
    case Level::LCRITICAL:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

std::string logging::levelToColor(Level level)
{
    switch (level) {
    case Level::LDEBUG:
        return LOG_PINK;
    case Level::LINFO:
        return LOG_GREEN;
    case Level::LWARNING:
        return LOG_YELLOW;
    case Level::LERROR:
        return LOG_RED;
    case Level::LCRITICAL:
        return LOG_RED LOG_BOLD;
    default:
        return LOG_COLOR_RESET;
    }
}
logging::Logger::Logger():
    log_level(Level::LINFO),
    prefix_creator(createDefaultPrefix),
    log_stream(getDefaultLogStream()),
    max_log_size(DEFAULT_MAX_LOG_SIZE)
{
}

logging::Logger::~Logger() { closeLogFile(); }

std::string
logging::Logger::formatMessage(Level level, const std::string &message, bool add_color) const
{
    std::string message_copy = message;
    std::string prefix = add_color ? levelToColor(level) + prefix_creator(level) + LOG_COLOR_RESET
                                   : prefix_creator(level);

    message_copy.erase(message_copy.find_last_not_of(" \n\r\t") + 1);
    std::size_t pos = 0;
    while ((pos = message_copy.find('\n', pos)) != std::string::npos &&
           pos < message_copy.size() - 2) {
        message_copy.replace(pos, 1, "\n" + prefix);
        pos += prefix.size() + 1;
    }
    return prefix + message_copy;
}

void logging::Logger::log(Level level, const std::string &message)
{
    if (level < log_level) {
        return;
    }
    if (log_stream != nullptr) {
        *log_stream << formatMessage(level, message, true) << std::endl;
    }
    if (logFileIsOpen()) {
        if (static_cast<std::size_t>(log_file.tellp()) > max_log_size) {
            log_file.close();
            log_file.open(log_file_path, std::ios::out | std::ios::ate);
        }
        log_file << formatMessage(level, message, false) << std::endl;
        log_file.flush();
    }
}

logging::Level logging::Logger::getLogLevel() const { return log_level; }

void logging::Logger::setLogLevel(logging::Level level) { log_level = level; }

std::ostream *logging::Logger::getLogStream() const { return log_stream; }

void logging::Logger::setLogStream(std::ostream *stream) { log_stream = stream; }

void logging::Logger::resetLogStream() { log_stream = getDefaultLogStream(); }

std::ostream *logging::Logger::getDefaultLogStream() { return &std::cout; }

logging::Logger::PrefixCreator logging::Logger::getLogPrefix() const { return prefix_creator; }

void logging::Logger::setLogPrefix(logging::Logger::PrefixCreator prefixCreator)
{
    prefix_creator = std::move(prefixCreator);
}

void logging::Logger::resetLogPrefix() { prefix_creator = createDefaultPrefix; }

std::string logging::Logger::createDefaultPrefix(Level level)
{
    return "[" + levelToStr(level) + "] ";
}

void logging::Logger::openLogFile(const std::filesystem::path &filePath)
{
    if (logFileIsOpen()) {
        if (log_file_path == filePath) {
            return;
        }
        closeLogFile();
    }
    log_file.open(filePath, std::ios::out | std::ios::ate);
    log_file_path = filePath;
}

void logging::Logger::closeLogFile()
{
    if (logFileIsOpen()) {
        log_file.close();
    }
}

bool logging::Logger::logFileIsOpen() const { return log_file.is_open(); }

void logging::Logger::setMaxLogSize(std::size_t size) { max_log_size = size; }

std::size_t logging::Logger::getMaxLogSize() const { return max_log_size; }

#endif