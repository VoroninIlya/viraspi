#pragma once

#include "webapihandler.hpp"
#include "viduplexqueue.hpp"
#include "mongoose.h"
#include <memory>
#include <string>
#include <deque>
#include <mutex>
#include <map>

using Sessions = std::map<uint64_t, apiCtx>;
using Queue = viasyncworker::ViDuplexQueue<std::string, 
std::function<void(const std::string&)>>;

class ConnectionHandler {

  static Sessions m_sessions;

  ConnectionHandler();

  void taskHandler();
  void taskCallback();
  void webSocketHandler(apiCtx& ctx);
  void HttpHandler(apiCtx& ctx);
  void sessionsGrabageCollect();

  static void eventsHandler(struct mg_connection *c, int ev, void *evData);

  static void evOpen(apiCtx& ctx);
  static void evClose(apiCtx& ctx);
  static void evHttpMsg(apiCtx& ctx);
  static void evPoll(apiCtx& ctx);
  static void handleStates(apiCtx& ctx,
    std::function<void(apiCtx& ctx)> EvOpen = nullptr,
    std::function<void(apiCtx& ctx)> EvClose = nullptr,
    std::function<void(apiCtx& ctx)> EvHttpMsg = nullptr,
    std::function<void(apiCtx& ctx)> EvPoll = nullptr,
    std::function<void(apiCtx& ctx)> EvError = nullptr,
    std::function<void(apiCtx& ctx)> EvResolve = nullptr,
    std::function<void(apiCtx& ctx)> EvConnect = nullptr,
    std::function<void(apiCtx& ctx)> EvTlsHs = nullptr,
    std::function<void(apiCtx& ctx)> EvRead = nullptr,
    std::function<void(apiCtx& ctx)> EvWrite = nullptr,
    std::function<void(apiCtx& ctx)> EvHttpHdrs = nullptr,
    std::function<void(apiCtx& ctx)> EvWsCtl = nullptr,
    std::function<void(apiCtx& ctx)> EvMqttCmd = nullptr,
    std::function<void(apiCtx& ctx)> EvMqttMsg = nullptr,
    std::function<void(apiCtx& ctx)> EvMqttOpen = nullptr,
    std::function<void(apiCtx& ctx)> EvSntpTime = nullptr,
    std::function<void(apiCtx& ctx)> EvWakeup = nullptr,
    std::function<void(apiCtx& ctx)> EvUser = nullptr,
    std::function<void(apiCtx& ctx)> EvAccept = nullptr, 
    std::function<void(apiCtx& ctx)> EvWsOpen = nullptr,
    std::function<void(apiCtx& ctx)> EvWsMsg = nullptr
  );

public:

  std::unique_ptr<Queue> wsQueue;
  std::unique_ptr<Queue> httpQueue;

  ConnectionHandler(const ConnectionHandler&) = delete;
  
  static ConnectionHandler& getInstance(std::unique_ptr<Queue> wsQueue = nullptr, std::unique_ptr<Queue>  httpQueue = nullptr) {
      static ConnectionHandler obj;

      if (obj.wsQueue == nullptr) {
        if (nullptr != wsQueue) {
          obj.wsQueue = std::move(wsQueue);
        } else {
          obj.wsQueue = std::make_unique<Queue>();
        }
      }

      if (obj.httpQueue == nullptr) {
        if (nullptr != httpQueue) {
          obj.httpQueue = std::move(httpQueue);
        } else {
          obj.httpQueue = std::make_unique<Queue>();
        }
      }

      return obj;
  }

};