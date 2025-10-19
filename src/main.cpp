#include <iostream>
#include <stdio.h>
#include <string>
#include <memory>
#include <iomanip>
#include <thread>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include "pigpio.h"
#include "rotary_encoder.hpp"
#include "vihcsr04.h"
#include "vihcsr04.hpp"
#include "viasyncworker.hpp"
#include "time/time.hpp"
#include "hcsr04callbacks/hcsr04callbacks.h"
#include "webapihandler/webapihandler.hpp"
#include "webapihandler/connectionhandler.hpp"
#include "loghandler/loghandler.hpp"

#include "mongoose.h"

using namespace viwebapi;

using apiCtx = WebApiContext; // 
using api = ViWebApi<std::string, apiCtx&>; //
using node = ViWebApiNode<std::string, apiCtx&>; //

static float measuredDistance = 0.0f;

static int logHcSr04_1(const char *format, ...) {

  char buff[1024];
  va_list args;
  va_start(args, args);
  vsprintf(buff, format, args);
  va_end(args);

  std::string s{buff};

  LogHandler::getInstance()
    .log("main.HC-SR04-1", vilogger::LogLevel::Debug, s);

  return s.length();
}

static void logWebApi(uint8_t lvl, const std::string msg) {
  LogHandler::getInstance()
    .log("main.webapi", static_cast<vilogger::LogLevel>(lvl), msg);
}

static void logAsync(uint8_t lvl, const std::string msg) {
  LogHandler::getInstance()
    .log("main.async", static_cast<vilogger::LogLevel>(lvl), msg);
}

static void Distance(float distance) {
  measuredDistance = distance;
  printf("Distance: %f\r\n", distance);
}

std::vector<struct mg_connection*> clients;

void log(const std::string &message) {
  printf("%s", message.c_str());
}

static void EncoderCb(int v){
   printf("%d\r\n", v);
}

static void DelayMs(uint32_t ms);

int main(void) {
  LogHandler::getInstance().setPrint(log);

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
    stringstream ss;
    ss << "Called calback from assync: " << ret;
    logAsync(3, ss.str());
  };

  api::getInstance().setPrintCb(logWebApi);

  WebApiHandler::getInstance();
  ConnectionHandler::getInstance();

  // c usage
  VIHCSR04_SetPrintfCb((VIHCSR04_Printf_t)logHcSr04_1);
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

  auto message1 = [](){
    auto start = std::chrono::steady_clock::now();
    while(1) {
      auto current = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start);
      
      if (elapsed.count() >= 5)
      {
        start = current;
        std::stringstream ss; 
        ss << std::string("Thread1::Test msg: ") << current.time_since_epoch().count() << std::endl;
        ConnectionHandler::getInstance().wsQueue->pushOutgoing(ss.str());
      }
    }
  };

  auto message2 = [](){
    auto start = std::chrono::steady_clock::now();
    while(1) {
      auto current = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current - start);
      
      if (elapsed.count() >= 5)
      {
        start = current;
        std::stringstream ss; ss << "Thread2::Test msg: " << current.time_since_epoch().count() << std::endl;
        ConnectionHandler::getInstance().wsQueue->pushOutgoing(ss.str());
      }
    }
  };

  std::thread t1(message1);
  std::thread t2(message2);

  t1.detach(); 
  t2.detach(); 
  
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