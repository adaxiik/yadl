#pragma once
// copied from my older header-only logger, with some modifications
// https://github.com/adaxiik/c-logger
#ifdef __cplusplus
    #include <string>
    #include <sstream>
    #include <iostream>
    #include <cstdio>
#else
    #include <stdio.h>
#endif

#define YADL_LOGGER_LEVEL_DEBUG 0
#define YADL_LOGGER_LEVEL_INFO 1
#define YADL_LOGGER_LEVEL_WARNING 2
#define YADL_LOGGER_LEVEL_ERROR 3
#define YADL_LOGGER_LEVEL_FATAL 4

#define YADL_LOGGER_COLORS_OFF 0
#define YADL_LOGGER_COLORS_ON 1

#define YADL_LOGGER_COLOR_RESET "\033[0m"
#define YADL_LOGGER_COLOR_RED "\033[31m"
#define YADL_LOGGER_COLOR_GREEN "\033[32m"
#define YADL_LOGGER_COLOR_YELLOW "\033[33m"
#define YADL_LOGGER_COLOR_BLUE "\033[34m"

#ifdef YADL_DEBUG
    [[maybe_unused]] static const char* g_levelStrings[] = {
        "[DEBUG]",
        "[INFO]",
        "[WARNING]",
        "[ERROR]",
        "[FATAL]"
    };

    [[maybe_unused]] static const char* g_levelColors[] = {
        YADL_LOGGER_COLOR_BLUE,
        YADL_LOGGER_COLOR_GREEN,
        YADL_LOGGER_COLOR_YELLOW,
        YADL_LOGGER_COLOR_RED,
        YADL_LOGGER_COLOR_RED
    };

#endif

#ifndef YADL_LOGGER_OUTPUT_STDOUT
    #define YADL_LOGGER_OUTPUT_STDOUT stdout
#endif

#ifndef YADL_LOGGER_OUTPUT_STDERR
    #define YADL_LOGGER_OUTPUT_STDERR stderr
#endif


#ifndef YADL_LOGGER_SETTINGS_LEVEL
    #define YADL_LOGGER_SETTINGS_LEVEL YADL_LOGGER_LEVEL_DEBUG
#endif

#ifndef YADL_LOGGER_SETTINGS_COLORS
    #define YADL_LOGGER_SETTINGS_COLORS YADL_LOGGER_COLORS_ON
#endif


#ifdef YADL_DEBUG
    #define YADL_LOGGER_LOG(file,level,fmt,...) do { \
        if (level >= YADL_LOGGER_SETTINGS_LEVEL) { \
            fprintf(file, "%s%s%s: ", YADL_LOGGER_SETTINGS_COLORS ? g_levelColors[level] : "", g_levelStrings[level], YADL_LOGGER_SETTINGS_COLORS ? YADL_LOGGER_COLOR_RESET : ""); \
            fprintf(file, fmt, ##__VA_ARGS__); \
            fprintf(file, "\n"); \
        } \
        } while (0)
#else
    #define YADL_LOGGER_LOG(file,level,fmt,...) {}
#endif

#define YADL_LOG_DEBUG(fmt,...) YADL_LOGGER_LOG(YADL_LOGGER_OUTPUT_STDOUT, YADL_LOGGER_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define YADL_LOG_INFO(fmt,...) YADL_LOGGER_LOG(YADL_LOGGER_OUTPUT_STDOUT, YADL_LOGGER_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define YADL_LOG_WARN(fmt,...) YADL_LOGGER_LOG(YADL_LOGGER_OUTPUT_STDOUT, YADL_LOGGER_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define YADL_LOG_ERROR(fmt,...) YADL_LOGGER_LOG(YADL_LOGGER_OUTPUT_STDERR, YADL_LOGGER_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define YADL_LOG_FATAL(fmt,...) YADL_LOGGER_LOG(YADL_LOGGER_OUTPUT_STDERR, YADL_LOGGER_LEVEL_FATAL, fmt, ##__VA_ARGS__); exit(1)

#define YADL_LOG_FATAL_IF(condition, fmt, ...) do { \
    if (condition) { \
        YADL_LOG_FATAL(fmt, ##__VA_ARGS__); \
    } \
    } while (0)


#ifdef __cplusplus
    #define YADL_WATCH(variable) do { \
        YADL_LOG_DEBUG(#variable " = %s (%s:%d)", std::to_string(variable).c_str(), __FILE__, __LINE__); \
        } while (0)
#else
    #define YADL_LOG_DEBUG(variable) {}
#endif


#ifdef __cplusplus
#include <chrono>
#define YADL_PERF_COLOR YADL_LOGGER_COLOR_BLUE

#ifdef YADL_PERF
    #define YADL_FUNCTION_PERF(stream) yadl::debug::FunctionProfiler __function_profiler__(stream, __FUNCTION__)


    namespace yadl {
        namespace debug {
            class FunctionProfiler {
                public:
                    FunctionProfiler(std::ostream& stream, const char* functionName) 
                    : m_stream(stream)
                    , m_startTime(std::chrono::high_resolution_clock::now())
                    , m_functionName(functionName)
                    {
                        // m_stream << YADL_PERF_COLOR << "[PERF START] "<< YADL_LOGGER_COLOR_RESET << m_functionName  << std::endl;
                    }

                    ~FunctionProfiler()
                    {
                        auto endTime = std::chrono::high_resolution_clock::now();
                        double duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime).count() / 1000.0;
                        m_stream << YADL_PERF_COLOR << "[PERF] "<< YADL_LOGGER_COLOR_RESET  << m_functionName << ": " << duration << "ms" << std::endl;
                    }

                private:
                    std::ostream& m_stream;
                    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
                    std::string m_functionName;
            };

        }
    }

    #define YADL_PERF_START(name) auto __perf_start_##name = std::chrono::high_resolution_clock::now()
    #define YADL_PERF_END(name) auto __perf_end_##name = std::chrono::high_resolution_clock::now(); \
        double __perf_duration_##name = std::chrono::duration_cast<std::chrono::microseconds>(__perf_end_##name - __perf_start_##name).count() / 1000.0; \
        std::cout << YADL_PERF_COLOR << "[PERF] "<< YADL_LOGGER_COLOR_RESET  << #name << ": " << __perf_duration_##name << "ms" << std::endl
    #define YADL_PERF_END_FILE(name,stream) auto __perf_end_##name = std::chrono::high_resolution_clock::now(); \
        double __perf_duration_##name = std::chrono::duration_cast<std::chrono::microseconds>(__perf_end_##name - __perf_start_##name).count() / 1000.0; \
        stream << YADL_PERF_COLOR << "[PERF] "<< YADL_LOGGER_COLOR_RESET << #name << ": " << __perf_duration_##name << "ms"  << std::endl
    #else
        #define YADL_FUNCTION_PERF(stream) {}
        #define YADL_PERF_START(name) {}
        #define YADL_PERF_END(name) {}
        #define YADL_PERF_END_FILE(name,stream) {}
    #endif
#endif