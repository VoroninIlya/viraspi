#include "webapihandler.hpp"
#include "viasyncworker.hpp"
#include "viasyncworker.hpp"
#include "../time/time.hpp"
#include <sstream>
#include <fstream>

static std::string getCpuLoad() {
  std::ifstream file("/proc/stat");
  std::string line;
  getline(file, line);
  std::istringstream iss(line);
  std::string cpu;
  long user, nice, system, idle;
  iss >> cpu >> user >> nice >> system >> idle;
  long total = user + nice + system + idle;
  return std::to_string((total - idle) * 100 / total) + "%";
}

WebApiHandler::WebApiHandler() {

  viwebapi::api_element<apiCtx&> elem1 {std::string("/"), 
    std::make_unique<node>([](apiCtx& a) {

      viasyncworker::ViAsyncWorker<void, int&> w(nullptr);
      w.startWork([a](std::future<std::function<void(int&)>> f) {

        std::stringstream ss;
        ss << "<html><body><h1>Home</h1>" << 
              "<p>This is the home page: conn = " 
              << a.connection->id << "</p>" 
              << "</body></html>";

        mg_http_reply(a.connection, 200, "Content-Type: text/html\r\n",
          ss.str().c_str());

        //logAsync(3, "Send response:" + ss.str());

        int ret = 1;
        auto cb = f.get();
        if(cb)
          cb(ret);

      });

      return "";
    })
  };
  api::getInstance() += elem1;

  viwebapi::api_element<apiCtx&> elem2 {std::string("/ws"), 
    std::make_unique<node>([](apiCtx& a) {
      mg_ws_upgrade(a.connection, static_cast<mg_http_message*>(a.eventData), NULL);
      return "";
    })
  };
  api::getInstance() += elem2;

  viwebapi::api_element<apiCtx&> elem3 {std::string("/api/status"), 
    std::make_unique<node>([](apiCtx& a) {
      viasyncworker::ViAsyncWorker<void, int&> w(nullptr);
      w.startWork([a](std::future<std::function<void(int&)>> f) {

        std::string response = "{\"time\": \"" + getTime() + "\", \"cpuLoad\": \"" + getCpuLoad() + "\"}";
        //logAsync(3, "Send response:" + response);
        mg_http_reply(a.connection, 200, 
          "Access-Control-Allow-Origin: *\r\n"
          "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
          "Access-Control-Allow-Headers: Content-Type\r\n",
          "%s", response.c_str());
        int ret = 2;
        auto cb = f.get();
        if(cb)
          cb(ret);
      });
      return "";
    })
  };
  api::getInstance() += elem3;

}