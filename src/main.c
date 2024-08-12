
#include <stdio.h>
#include "pigpio.h"

int main(void){
  printf("Hallo world\r\n");

  if (gpioInitialise() < 0) {
    printf("Error by gpio initialization\r\n");
  }

  return 0;
}

__attribute__((weak)) int Reset_Handler() {

}