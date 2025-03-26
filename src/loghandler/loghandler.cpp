#include "loghandler.hpp"
#include "viloglevel.hpp"
#include "../time/time.hpp"

LogHandler::LogHandler(){

  m_rootLogger = std::make_shared<vilogger::ViLogger>("main", std::make_shared<vilogger::ViLogLevel>(),
    std::bind(&LogHandler::print, this, std::placeholders::_1),
    getHighResolutionTime);

  m_listOfLoggers["main"] = m_rootLogger.get();
  m_rootLogger->setAllowedLogLevel(vilogger::LogLevel::Debug);
  m_rootLogger->addSubLogger("HC-SR04-1");
  m_rootLogger->addSubLogger("webapi");
  m_listOfLoggers["main.webapi"] = static_cast<vilogger::ViLogger*>(&m_rootLogger->getSubLogger("webapi"));
  m_rootLogger->getSubLogger("webapi").setAllowedLogLevel(vilogger::LogLevel::Trace);
  m_rootLogger->addSubLogger("async");
  m_listOfLoggers["main.async"] = static_cast<vilogger::ViLogger*>(&m_rootLogger->getSubLogger("async"));
  m_rootLogger->getSubLogger("async").setAllowedLogLevel(vilogger::LogLevel::Trace);

}

void LogHandler::print(const std::string& s) {
  if(m_print)
    m_print(s);
  else
    printf("%s", s.c_str());
}

void LogHandler::setPrint(std::function<void(const std::string&)> p) {
  m_print = p;
}

void LogHandler::log(const std::string& name, vilogger::LogLevel level, const std::string& msg) {
  if(m_listOfLoggers.count(name)) {
    m_listOfLoggers[name]->level(level).message(msg);
  }
}

std::map<std::string, vilogger::LogLevel> LogHandler::getLogs() const {
  std::map<std::string, vilogger::LogLevel> res;
  for(auto&a : m_listOfLoggers) {
    res[a.first] = a.second->getAllowedLogLevel();
  }
  return res;
}

void LogHandler::setLogLevel(const std::string& name, vilogger::LogLevel level) {
  m_listOfLoggers[name]->setAllowedLogLevel(level);
}
