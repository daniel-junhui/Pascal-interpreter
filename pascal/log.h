// Copyright 2023 Zhu Junhui

#pragma once

#include <atomic>
#include <cstring>
#include <iostream>

namespace Pascal {
enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

static std::atomic<LogLevel> log_level_ = LogLevel::INFO;

// Logger is a singleton class
class Logger {
 private:
  Logger() = default;

 public:
  static LogLevel log_level() { return log_level_.load(); }
  static void set_log_level(LogLevel level) { log_level_.store(level); }
  static void set_log_level(const char* level) {
    if (strcmp(level, "INFO") == 0) {
      log_level_.store(LogLevel::INFO);
    } else if (strcmp(level, "DEBUG") == 0) {
      log_level_.store(LogLevel::DEBUG);
    } else if (strcmp(level, "WARNING") == 0) {
      log_level_.store(LogLevel::WARNING);
    } else if (strcmp(level, "ERROR") == 0) {
      log_level_.store(LogLevel::ERROR);
    } else {
      throw std::runtime_error("Invalid log level");
    }
  }

  template <typename T>
  Logger& operator<<(const T& msg) {
    const auto level_ = log_level();
    // debug is blue, info is green, warning is yellow, error is red
    switch (level_) {
      case LogLevel::DEBUG:
        std::cout << "\033[34m" << msg << "\033[0m";
        break;
      case LogLevel::INFO:
        std::cout << "\033[32m" << msg << "\033[0m";
        break;
      case LogLevel::WARNING:
        std::cout << "\033[33m" << msg << "\033[0m";
        break;
      case LogLevel::ERROR:
        std::cout << "\033[31m" << msg << "\033[0m";
        break;
      default:
        break;
    }
    return *this;
  }

  static Logger& instance() {
    static Logger logger_;
    return logger_;
  }
};

// at last print a newline
#define LOG(level)                            \
  if (LogLevel::level >= Logger::log_level()) \
  Logger::instance() << #level << ": "

}  // namespace Pascal