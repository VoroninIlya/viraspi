#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

namespace vihcsr04 {

  class Hcsr04Callbacks {
  public:
    static uint64_t PulseIn(const void* gpio, uint16_t port, 
  uint8_t state, uint64_t timeoutUs, const void* context);

    static void TriggerPort(const void* gpio, uint16_t port, 
       uint8_t state, uint64_t pulseDuration, const void* context);

    static void Distance(float distance);

  private:
    uint32_t m_startTick;
    uint32_t m_pulseDuration;

    static void AlertCb(int gpio, int level, unsigned int tick, void* context);

  };

}