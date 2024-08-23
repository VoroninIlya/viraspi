#include "hcsr04callbacks.h"
#include "pigpio.h"
#include <chrono>
#include <thread>

namespace vihcsr04 {

  uint64_t Hcsr04Callbacks::PulseIn(const void* gpio, uint16_t port, 
    uint8_t state, uint64_t timeoutUs, const void* context) {
  
    Hcsr04Callbacks* hcsr04Cb = (Hcsr04Callbacks*)context;
    hcsr04Cb->m_startTick = 0;
    hcsr04Cb->m_pulseDuration = 0;  
    // Set the callback for state change
    gpioSetAlertFuncEx(port, Hcsr04Callbacks::AlertCb, (void*)hcsr04Cb);  
    auto start_time = std::chrono::high_resolution_clock::now();  
    // Wait for the pulse to be measured or timeout
    while (hcsr04Cb->m_pulseDuration == 0) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = current_time - start_time;
        auto count = elapsed.count();
        if (count > timeoutUs) {
            gpioSetAlertFunc(port, NULL); // Disable callback  
            return 0; // Timeout occurred
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }    
    return hcsr04Cb->m_pulseDuration; // Return duration in microseconds  
  }
  
  void Hcsr04Callbacks::TriggerPort(const void* gpio, uint16_t port, 
     uint8_t state, uint64_t pulseDuration, const void* context) {
  
     gpioTrigger(port, pulseDuration, state);
  }
  
  void Hcsr04Callbacks::Distance(float distance) {
     printf("Distance: %f\r\n", distance);
  }
  
  void Hcsr04Callbacks::AlertCb(int gpio, int level, unsigned int tick, void* context)
  {
    static bool measuring = false;

    Hcsr04Callbacks* hcsr04Cb = static_cast<Hcsr04Callbacks*>(context);
  
    if (level == 1 && !measuring) {
        // Rising edge, start measuring
        hcsr04Cb->m_startTick = tick;
        measuring = true;
    } else if (level == 0 && measuring) {
        // Falling edge, stop measuring
        hcsr04Cb->m_pulseDuration = tick - hcsr04Cb->m_startTick;
        measuring = false;
        gpioSetAlertFunc(gpio, NULL); // Disable callback
    }
  }
}