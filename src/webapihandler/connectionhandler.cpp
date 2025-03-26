#include "connectionhandler.hpp"
#include "webapicontext.hpp"
#include <thread>
#include <future>
#include <functional>
#include <iostream>

Sessions ConnectionHandler::m_sessions;
std::deque<std::string> ConnectionHandler::m_incomingHttpMessages;
std::deque<std::string> ConnectionHandler::m_incomingWsMessages;
std::deque<std::string> ConnectionHandler::m_outgoingHttpMessages;
std::deque<std::string> ConnectionHandler::m_outgoingWsMessages;
std::mutex ConnectionHandler::m_incomingHttpMutex;
std::mutex ConnectionHandler::m_incomingWsMutex;
std::mutex ConnectionHandler::m_outgoingHttpMutex;
std::mutex ConnectionHandler::m_outgoingWsMutex;

ConnectionHandler::ConnectionHandler() {

    std::promise<std::function<void()>> prom;
    std::future<std::function<void()>> fut = prom.get_future();

    std::thread t(std::bind(&ConnectionHandler::taskHandler, this), std::move(fut));

    prom.set_value(std::bind(&ConnectionHandler::taskCallback, this));

    t.detach();
}

void ConnectionHandler::sessionsGrabageCollect() {
  // remove session if connection is NULL
  for(auto it = m_sessions.begin(); it != m_sessions.end(); it++) {
    if(it->second.connection == NULL)
      m_sessions.erase((it--)->first);
  }
}

void ConnectionHandler::pushOutgoingMessageToWs(const std::string& s) {
  std::lock_guard lock{m_outgoingWsMutex};
  m_outgoingWsMessages.push_back(s);
}

void ConnectionHandler::pushOutgoingMessageToHttp(const std::string& s) {
  std::lock_guard lock{m_outgoingHttpMutex};
  m_outgoingHttpMessages.push_back(s);
}

void ConnectionHandler::pushIncomingMessageToWs(const std::string& s) {
  std::lock_guard lock{m_incomingWsMutex};
  m_incomingWsMessages.push_back(s);
}

void ConnectionHandler::pushIncomingMessageToHttp(const std::string& s) {
  std::lock_guard lock{m_incomingHttpMutex};
  m_incomingHttpMessages.push_back(s);
}

bool ConnectionHandler::hasIncomingWsMessages() {
  std::lock_guard lock{m_incomingWsMutex};
  return !m_incomingWsMessages.empty();
}

bool ConnectionHandler::hasIncomingHttpMessages() {
  std::lock_guard lock{m_incomingHttpMutex};
  return !m_incomingHttpMessages.empty();
}

bool ConnectionHandler::hasOutgoingWsMessages() {
  std::lock_guard lock{m_outgoingWsMutex};
  return !m_outgoingWsMessages.empty();
}

bool ConnectionHandler::hasOutgoingHttpMessages() {
  std::lock_guard lock{m_outgoingHttpMutex};
  return !m_outgoingHttpMessages.empty();
}

std::string ConnectionHandler::popOutgoingMessageFromWs() {
  if(!hasOutgoingWsMessages())
    return "";

  std::lock_guard lock{m_outgoingWsMutex};
  std::string s = m_outgoingWsMessages.back();
  m_outgoingWsMessages.pop_back();
  return s;
}

std::string ConnectionHandler::popOutgoingMessageFromHttp() {
  if(!hasOutgoingHttpMessages())
    return "";

  std::lock_guard lock{m_outgoingHttpMutex};
  std::string s = m_outgoingHttpMessages.back();
  m_outgoingHttpMessages.pop_back();
  return s;
}

std::string ConnectionHandler::popIncomingMessageFromWs() {
  if(!hasIncomingWsMessages())
    return "";

  std::lock_guard lock{m_incomingWsMutex};
  std::string s = m_incomingWsMessages.back();
  m_incomingWsMessages.pop_back();
  return s;
}

std::string ConnectionHandler::popIncomingMessageFromHttp() {
  if(!hasIncomingHttpMessages())
    return "";

  std::lock_guard lock{m_incomingHttpMutex};
  std::string s = m_incomingHttpMessages.back();
  m_incomingHttpMessages.pop_back();
  return s;
}

void ConnectionHandler::subscribeOnWsMessages(std::unique_ptr<Subscriber> sbc) {
  m_wsMessagesSubscribers.push_back(std::move(sbc));
}

void ConnectionHandler::subscribeOnHttpMessages(std::unique_ptr<Subscriber> sbc) {
  m_httpMessagesSubscribers.push_back(std::move(sbc));
}

void ConnectionHandler::unSubscribeOnWsMessages(std::unique_ptr<Subscriber> sbc) {
  auto el = std::find(m_wsMessagesSubscribers.begin(),
    m_wsMessagesSubscribers.end(), sbc);
  if(m_wsMessagesSubscribers.end() != el)
    m_wsMessagesSubscribers.erase(el);
}

void ConnectionHandler::unSubscribeOnHttpMessages(std::unique_ptr<Subscriber> sbc) {
  auto el = std::find(m_httpMessagesSubscribers.begin(),
    m_httpMessagesSubscribers.end(), sbc);
  if(m_httpMessagesSubscribers.end() != el)
    m_httpMessagesSubscribers.erase(el);  
}

void ConnectionHandler::taskHandler() {

  struct mg_mgr mgr;  // Declare event manager
  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&mgr);  // Initialise event manager

  mg_http_listen(&mgr, "http://0.0.0.0:8000", ConnectionHandler::eventsHandler, nullptr);  // Setup listener

  while (true) {
      mg_mgr_poll(&mgr, 1000);
  }
}

void ConnectionHandler::taskCallback() {

}

void ConnectionHandler::webSocketHandler(apiCtx& ctx) {
  if(NULL != ctx.connection && ctx.connection->is_websocket){
    handleStates(ctx, evOpen, evClose, evHttpMsg, evPoll);
  }
}

void ConnectionHandler::HttpHandler(apiCtx& ctx) {
  if(NULL != ctx.connection && !ctx.connection->is_websocket){
    handleStates(ctx, evOpen, evClose, evHttpMsg);
  }
}

void ConnectionHandler::eventsHandler(struct mg_connection *c, int ev, void *evData) {
  m_sessions[c->id] = apiCtx{c, ev, evData};

  ConnectionHandler::HttpHandler(m_sessions[c->id]);
  ConnectionHandler::webSocketHandler(m_sessions[c->id]);
}

void ConnectionHandler::handleStates(apiCtx& ctx,
    std::function<void(apiCtx& ctx)> EvOpen,
    std::function<void(apiCtx& ctx)> EvClose,
    std::function<void(apiCtx& ctx)> EvHttpMsg,
    std::function<void(apiCtx& ctx)> EvPoll,
    std::function<void(apiCtx& ctx)> EvError,
    std::function<void(apiCtx& ctx)> EvResolve,
    std::function<void(apiCtx& ctx)> EvConnect,
    std::function<void(apiCtx& ctx)> EvTlsHs,
    std::function<void(apiCtx& ctx)> EvRead,
    std::function<void(apiCtx& ctx)> EvWrite,
    std::function<void(apiCtx& ctx)> EvHttpHdrs,
    std::function<void(apiCtx& ctx)> EvWsCtl,
    std::function<void(apiCtx& ctx)> EvMqttCmd,
    std::function<void(apiCtx& ctx)> EvMqttMsg,
    std::function<void(apiCtx& ctx)> EvMqttOpen,
    std::function<void(apiCtx& ctx)> EvSntpTime,
    std::function<void(apiCtx& ctx)> EvWakeup,
    std::function<void(apiCtx& ctx)> EvUser,
    std::function<void(apiCtx& ctx)> EvAccept,
    std::function<void(apiCtx& ctx)> EvWsOpen,     
    std::function<void(apiCtx& ctx)> EvWsMsg) {

  switch(ctx.event) {
    case MG_EV_ERROR:
      std::cout << "MG_EV_ERROR " << ctx.connection->id << std::endl;
      if(EvError){
        EvError(ctx);
      }
      break;
    case MG_EV_OPEN:
      std::cout << "MG_EV_OPEN " << ctx.connection->id << std::endl;
      if(EvOpen) {
        EvOpen(ctx);
      }
      break;
    case MG_EV_POLL:
    //  std::cout << "MG_EV_POLL" << std::endl;
      if(EvPoll) {
        EvPoll(ctx);
      }
      break;
    case MG_EV_RESOLVE:
      std::cout << "MG_EV_RESOLVE " << ctx.connection->id << std::endl;
      if(EvResolve) {
        EvResolve(ctx);
      }
      break;
    case MG_EV_CONNECT:
      std::cout << "MG_EV_CONNECT " << ctx.connection->id << std::endl;
      if(EvConnect) {
        EvConnect(ctx);
      }
      break;
    case MG_EV_TLS_HS:
      std::cout << "MG_EV_TLS_HS " << ctx.connection->id << std::endl;
      if(EvTlsHs) {
        EvTlsHs(ctx);
      }
      break;
    case MG_EV_READ:
      std::cout << "MG_EV_READ " << ctx.connection->id << std::endl;
      if(EvRead) {
        EvRead(ctx);
      }
      break;
    case MG_EV_WRITE:
      std::cout << "MG_EV_WRITE " << ctx.connection->id << std::endl;
      if(EvWrite) {
        EvWrite(ctx);
      }
      break;
    case MG_EV_CLOSE:
      std::cout << "MG_EV_CLOSE " << ctx.connection->id << std::endl;
      if(EvClose) {
        EvClose(ctx);
      }
      break;
    case MG_EV_HTTP_HDRS:
      std::cout << "MG_EV_HTTP_HDRS " << ctx.connection->id << std::endl;
      if(EvHttpHdrs) {
        EvHttpHdrs(ctx);
      }
      break;
    case MG_EV_WS_CTL:
      std::cout << "MG_EV_WS_CTL " << ctx.connection->id << std::endl;
      if(EvWsCtl) {
        EvWsCtl(ctx);
      }
      break;
    case MG_EV_MQTT_CMD:
      std::cout << "MG_EV_MQTT_CMD " << ctx.connection->id << std::endl;
      if(EvMqttCmd) {
        EvMqttCmd(ctx);
      }
      break;
    case MG_EV_MQTT_MSG:
      std::cout << "MG_EV_MQTT_MSG " << ctx.connection->id << std::endl;
      if(EvMqttMsg) {
        EvMqttMsg(ctx);
      }
      break;
    case MG_EV_MQTT_OPEN:
      std::cout << "MG_EV_MQTT_OPEN " << ctx.connection->id << std::endl;
      if(EvMqttOpen) {
        EvMqttOpen(ctx);
      }
      break;
    case MG_EV_SNTP_TIME:
      std::cout << "MG_EV_SNTP_TIME " << ctx.connection->id << std::endl;
      if(EvSntpTime) {
        EvSntpTime(ctx);
      }
      break;
    case MG_EV_WAKEUP:
      std::cout << "MG_EV_WAKEUP " << ctx.connection->id << std::endl;
      if(EvWakeup) {
        EvWakeup(ctx);
      }
      break;
    case MG_EV_USER:
      std::cout << "MG_EV_USER " << ctx.connection->id << std::endl;
      if(EvUser) {
        EvUser(ctx);
      }
      break;
    case MG_EV_ACCEPT:
      std::cout << "MG_EV_ACCEPT " << ctx.connection->id << std::endl;
      if(EvAccept) {
        EvAccept(ctx);
      }
      break;  
    case MG_EV_HTTP_MSG:
      std::cout << "MG_EV_HTTP_MSG " << ctx.connection->id << std::endl;
      if(EvHttpMsg) {
        EvHttpMsg(ctx);
      }
      break;  
    case MG_EV_WS_OPEN:
      std::cout << "MG_EV_WS_OPEN " << ctx.connection->id << std::endl;
      if(EvWsOpen) {
        EvWsOpen(ctx);
      }      
      break;
    case MG_EV_WS_MSG:
      std::cout << "MG_EV_WS_MSG " << ctx.connection->id << std::endl;
      if(EvWsMsg) {
        EvWsMsg(ctx);
      }
      break;
  }
} 

void ConnectionHandler::evOpen (apiCtx& ctx) {
  sessionsGrabageCollect();
  m_sessions[ctx.connection->id] = ctx;
};

void ConnectionHandler::evClose (apiCtx& ctx) {
  if(m_sessions.contains(ctx.connection->id)){
    m_sessions.erase(ctx.connection->id);
  }
  sessionsGrabageCollect();
};

void ConnectionHandler::evHttpMsg (apiCtx& ctx) {
  auto hm = static_cast<struct mg_http_message*>(ctx.eventData);
  auto c = ctx.connection;
  api::getInstance().runtime(
    [hm](const std::string& s) -> bool {
      return mg_match(hm->uri, mg_str(s.data()), NULL);
    },
    [c]() {
      // Serve a 404 page for any undefined routes
        mg_http_reply(c, 404, "Content-Type: text/html\r\n",
          "<html><body><h1>404 Not Found</h1>"
          "<p>The page you are looking for does not exist.</p>"
          "<p><a href=\"/\">Go back to homepage</a></p>"
          "</body></html>");
    },
    ctx
  ); 
}

void ConnectionHandler::evPoll(apiCtx& ctx) {
  std::string msg = popOutgoingMessageFromWs();
  if(!msg.empty()) {
    mg_ws_send(ctx.connection, msg.c_str(), msg.length(), WEBSOCKET_OP_TEXT);
  }
}