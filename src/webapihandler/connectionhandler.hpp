#pragma once

#include "webapihandler.hpp"
#include "subscriber.hpp"
#include "mongoose.h"
#include <string>
#include <deque>
#include <mutex>
#include <map>

using Sessions = std::map<uint64_t, apiCtx>;

class ConnectionHandler {

  static Sessions m_sessions;
  static std::deque<std::string> m_incomingHttpMessages;
  static std::deque<std::string> m_incomingWsMessages;
  static std::deque<std::string> m_outgoingHttpMessages;
  static std::deque<std::string> m_outgoingWsMessages;
  static std::mutex m_incomingHttpMutex;
  static std::mutex m_incomingWsMutex;
  static std::mutex m_outgoingHttpMutex;
  static std::mutex m_outgoingWsMutex;

  void taskHandler();
  void taskCallback();

  static void eventsHandler(struct mg_connection *c, int ev, void *evData);
  static void webSocketHandler(apiCtx& ctx);
  static void HttpHandler(apiCtx& ctx);

  static void sessionsGrabageCollect();

  ConnectionHandler();

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

  static void pushIncomingMessageToWs(const std::string& s);
  static void pushIncomingMessageToHttp(const std::string& s);
  static std::string popOutgoingMessageFromWs();
  static std::string popOutgoingMessageFromHttp();
  static bool hasOutgoingWsMessages();
  static bool hasOutgoingHttpMessages();

  std::vector<std::unique_ptr<Subscriber>> m_httpMessagesSubscribers;
  std::vector<std::unique_ptr<Subscriber>> m_wsMessagesSubscribers;

public:

  ConnectionHandler(const ConnectionHandler& ) = delete;
  
  static ConnectionHandler& getInstance() {
      static ConnectionHandler obj;
      return obj;
  }

  void pushOutgoingMessageToWs(const std::string& s);
  void pushOutgoingMessageToHttp(const std::string& s);
  bool hasIncomingWsMessages();
  bool hasIncomingHttpMessages();
  std::string popIncomingMessageFromWs();
  std::string popIncomingMessageFromHttp();

  void subscribeOnWsMessages(std::unique_ptr<Subscriber>);
  void subscribeOnHttpMessages(std::unique_ptr<Subscriber>);
  void unSubscribeOnWsMessages(std::unique_ptr<Subscriber>);
  void unSubscribeOnHttpMessages(std::unique_ptr<Subscriber>);

  // 
  
};