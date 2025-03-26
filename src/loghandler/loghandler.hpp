#pragma once

#include "vilogger.hpp"
#include <memory>
#include <map>

class LogHandler {

  std::map<std::string, vilogger::ViLogger*> m_listOfLoggers;
  
  std::shared_ptr<vilogger::ViLogger> m_rootLogger;

  LogHandler();

  void print(const std::string& s);
  std::function<void(const std::string&)> m_print;
public:
   
  static LogHandler& getInstance() {
    static LogHandler ret;
    return ret;
  }

  void setPrint(std::function<void(const std::string&)>);
  void log(const std::string&, vilogger::LogLevel, const std::string&);
  std::map<std::string, vilogger::LogLevel> getLogs() const;
  void setLogLevel(const std::string&, vilogger::LogLevel);
  
};