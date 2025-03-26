
#include "time.hpp"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string getTime() {
  std::time_t now = std::time(nullptr);
  char buf[80];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  return std::string(buf);
}

std::string getHighResolutionTime() {
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);
  auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;

  std::tm tm = *std::localtime(&now_time_t);

  std::stringstream ss;
  ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "." << now_us.count();
  return ss.str();
}