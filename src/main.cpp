
#include <stdio.h>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>
#include <thread>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <functional>
#include <memory>
#include <ctime>
#include <sstream>
#include "pigpio.h"
#include "rotary_encoder.hpp"

#include "vihcsr04.h"
#include "vihcsr04.hpp"
#include "viwebapinode.hpp"
#include "viwebapi.hpp"
#include "viasyncworker.hpp"
#include "hcsr04callbacks/hcsr04callbacks.h"
#include "webapihandler/webapihandler.hpp"
#include "webapihandler/webapihandler.hpp"

#include "mongoose.h"

using namespace viwebapi;

using apiCtx = IWebApiContext; // 
using api = ViWebApi<std::string, apiCtx&>; //
using node = ViWebApiNode<std::string, apiCtx&>; //

static float measuredDistance = 0.0f;

static struct mg_connection *conn = nullptr;

static void Distance(float distance) {
  measuredDistance = distance;
  printf("Distance: %f\r\n", distance);
}

// Функция получения загрузки процессора
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

// Функция получения текущего времени
std::string getTime() {
  std::time_t now = std::time(nullptr);
  char buf[80];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  return std::string(buf);
}

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
      struct mg_http_message *hm = (struct mg_http_message *) ev_data;

      apiCtx ctx{c};
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

      //struct mg_http_serve_opts opts = { .root_dir = "./web_root/" };
      //mg_http_serve_dir(c, hm, &opts);
    }
}

static void EncoderCb(int v){
   printf("%d\r\n", v);
}

static void DelayMs(uint32_t ms);

int main(void) {
  printf("Hallo world\r\n");

  //if (gpioInitialise() < 0) {
  //  printf("Error by gpio initialization\r\n");
  //  return 0;
  //}
//
  //re_decoder dec{22,23,&EncoderCb};
//
  //gpioSetMode(6, PI_OUTPUT);
  //gpioSetPullUpDown(6, PI_PUD_DOWN);
  //gpioSetMode(5, PI_INPUT);
  //gpioSetPullUpDown(6, PI_PUD_DOWN);

  //vihcsr04::Hcsr04Callbacks hcsr04Cb{};

  int i = 0;

  auto testCb = [](int& ret) -> void {
    printf("%d\n", ret);
  };

  viwebapi::api_element<apiCtx&> elem1 {std::string("/"), 
    std::make_unique<node>([testCb](apiCtx& a) {

      viasyncworker::ViAsyncWorker<void, int&> w(testCb);
      w.startWork([a](std::future<std::function<void(int&)>> f) {

        int ret = 1;

        std::stringstream ss;
        ss << "<html><body><h1>Home</h1>" << 
              "<p>This is the home page: conn = " 
              << a.con->id << "</p>" 
              << "</body></html>";

        mg_http_reply(a.con, 200, "Content-Type: text/html\r\n",
          ss.str().c_str());
        
        auto cb = f.get();
        if(cb)
          cb(ret);

      });

      return "";
    })
  };

  viwebapi::api_element<apiCtx&> elem2 {std::string("/api/status"), 
    std::make_unique<node>([testCb](apiCtx& a) {

      viasyncworker::ViAsyncWorker<void, int&> w(testCb);

      w.startWork([a](std::future<std::function<void(int&)>> f){
        
        int ret = 2;

        std::string response = "{\"time\": \"" + getTime() + "\", \"cpuLoad\": \"" + getCpuLoad() + "\"}";
        mg_http_reply(a.con, 200, "Content-Type: application/json\r\n", "%s", response.c_str());

        auto cb = f.get();
        if(cb)
          cb(ret);

      });

      return "";
    })
  };

  (api::getInstance() += elem1) += elem2;

  struct mg_mgr mgr;  // Declare event manager
  mg_mgr_init(&mgr);  // Initialise event manager
  mg_http_listen(&mgr, "http://0.0.0.0:8000", ev_handler, nullptr);  // Setup listener

  // c usage
  VIHCSR04_SetPrintfCb((VIHCSR04_Printf_t)printf);
  VIHCSR04_SetDebugLvl(VIHCSR04_DEBUG_INFO);
  VIHCSR04_Init(vihcsr04::Hcsr04Callbacks::PulseIn, vihcsr04::Hcsr04Callbacks::TriggerPort);
  VIHCSR04_Create("HC-SR04 1", nullptr, 6, nullptr, 5);

  //VIHCSR04_MeasureDistanceAsync("HC-SR04 1", VIHCSR04_CONTINUOUS_MEASURE, 
  // 31.307f, 400, (VIHCSR04_Distance_t)Distance, &hcsr04Cb);

  // c++ usage
  //vihcsr04::Hcsr04Sensor sensor{vihcsr04::Hcsr04Callbacks::PulseIn, vihcsr04::Hcsr04Callbacks::TriggerPort};   
  //sensor.SetDebugLvl(vihcsr04::DebugLvl_t::DEBUG_INFO);
  //sensor.SetPrintfCb((vihcsr04::Printf_t)printf);
  //sensor.AddSensor("HC-SR04 2", nullptr, 6, nullptr, 5);

  //sensor.MeasureDistanceAsync("HC-SR04 2", vihcsr04::MeasureMode_t::CONTINUOUS_MEASURE, 
  //31.307f, 400, (vihcsr04::Distance_t)Distance, &hcsr04Cb);

  while(1) {

    //VIHCSR04_Runtime(); // c runtime
    //DelayMs(100);

    //sensor.Runtime(); // c++ runtime
    //DelayMs(100);

    //printf("Synchronous result: %f\r\n",
    //  VIHCSR04_MeasureDistance("HC-SR04 1", 19.307f, 400));
    //DelayMs(1000);

    //printf("Synchronous result: %f\r\n",
    //  sensor.MeasureDistance("HC-SR04 2", 19.307f, 400));
    //DelayMs(100);

    mg_mgr_poll(&mgr, 1000);
  }

  gpioTerminate();

  return 0;
}

__attribute__((weak)) int Reset_Handler() {
   return 0;
}

void DelayMs(uint32_t ms) {
   struct timespec ts;
    ts.tv_sec = ms / 1000; 
    ts.tv_nsec = ms % 1000 * 1000000L;   

   nanosleep(&ts, nullptr);  // Sleep for 1 millisecond
}