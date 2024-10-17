#pragma once

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

// #include <boost/log/core.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/trivial.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>
// #include <boost/log/utility/setup/console.hpp>
// #include <boost/log/utility/setup/file.hpp>

// namespace logs {

// // Helper function to convert any argument to a string
// template<typename T>
// std::string toString(T &&arg)
// {
//     std::ostringstream oss; // oss117
//     oss << std::forward<T>(arg);
//     return oss.str();
// }

// template<typename... Args>
// std::string concatenate(Args... args)
// {
//     return (toString(args) + ...);
// }

// inline boost::log::trivial::severity_level getSeverityLevelFromEnv()
// {
//     static std::mutex env_mutex;
//     std::string logLevelStr;
//     std::lock_guard<std::mutex> lock(env_mutex);
//     {
//         const char *logLevelEnv = std::getenv("LOG_LEVEL");
//         if (logLevelEnv == nullptr) {
//             // Default to 'info' if LOG_LEVEL is not set
//             return boost::log::trivial::info;
//         }
//         logLevelStr = logLevelEnv;
//         std::transform(logLevelStr.begin(), logLevelStr.end(), logLevelStr.begin(), ::tolower);
//     }

//     if (logLevelStr == "trace") {
//         return boost::log::trivial::trace;
//     }
//     if (logLevelStr == "debug") {
//         return boost::log::trivial::debug;
//     }
//     if (logLevelStr == "info") {
//         return boost::log::trivial::info;
//     }
//     if (logLevelStr == "warning") {
//         return boost::log::trivial::warning;
//     }
//     if (logLevelStr == "error") {
//         return boost::log::trivial::error;
//     }
//     if (logLevelStr == "fatal") {
//         return boost::log::trivial::fatal;
//     }

//     std::cerr << "Unknown LOG_LEVEL: " << logLevelStr << ". Defaulting to 'info'." << std::endl;
//     // Fallback to 'info' if an unknown level is provided
//     return boost::log::trivial::info;
// }

// constexpr uint32_t MAX_LOG_FILE_SIZE = 10 * 1024 * 1024; // 10 MB

// inline void init()
// {
//     boost::log::add_common_attributes();

//     boost::log::add_console_log(
//         std::cout, boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%"
//     );

//     boost::log::add_file_log(
//         boost::log::keywords::file_name = "app_%N.log",
//         boost::log::keywords::rotation_size = MAX_LOG_FILE_SIZE,
//         boost::log::keywords::time_based_rotation =
//             boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
//         boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%"
//     );

//     boost::log::trivial::severity_level logLevel = getSeverityLevelFromEnv();
//     boost::log::core::get()->set_filter(boost::log::trivial::severity >= logLevel);
// }

// template<typename... Args>
// void trace(Args... args)
// {
//     BOOST_LOG_TRIVIAL(trace) << concatenate(args...);
// }

// template<typename... Args>
// void debug(Args... args)
// {
//     BOOST_LOG_TRIVIAL(debug) << concatenate(args...);
// }

// template<typename... Args>
// void info(Args... args)
// {
//     BOOST_LOG_TRIVIAL(info) << concatenate(args...);
// }

// template<typename... Args>
// void warn(Args... args)
// {
//     BOOST_LOG_TRIVIAL(warning) << concatenate(args...);
// }

// template<typename... Args>
// void error(Args... args)
// {
//     BOOST_LOG_TRIVIAL(error) << concatenate(args...);
// }

// template<typename... Args>
// void fatal(Args... args)
// {
//     BOOST_LOG_TRIVIAL(fatal) << concatenate(args...);
// }

// // Static initialization block to call init once
// struct LoggerInitializer {
//     LoggerInitializer()
//     {
//         try {
//             init();
//         } catch (const std::exception &e) {
//             std::cerr << "Failed to initialize logging: " << e.what() << std::endl;
//             std::terminate();
//         } catch (...) {
//             std::cerr << "Failed to initialize logging due to an unknown error." << std::endl;
//             std::terminate();
//         }
//     }
// };

// static const LoggerInitializer loggerInitializer;
// } // namespace logs
