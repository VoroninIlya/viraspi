#include <stdio.h>

int main(void){
  printf("Hallo world\r\n");
  return 0;
}

__attribute__((weak)) int Reset_Handler() {

}