/*
 * Below is the given application for lab4_2.
 * The goal of this app is to control the car via Bluetooth. 
 */

#include "user_lib.h"
#include "util/types.h"
void delay(unsigned int time){
  unsigned int a = 0xfffff ,b = time;
  volatile unsigned int i,j;
  for(i = 0; i < a; ++i){
    for(j = 0; j < b; ++j){
      ;
    }
  }
}
int main(void) {
  printu("Hello world!\n");
  int i;
  int pid = fork();
  if(pid == 0)
  {
    while (1)
    {
      delay(3);
      printu("waiting for you!\n");
    }
    
  }
  else
  {
    for (;;) {
      char temp = (char)uartgetchar();
      printu("%c\n", temp);
      switch (temp)
      {
        case '1' : gpio_reg_write(0x2e); break; //前进
        case '2' : gpio_reg_write(0xd1); break; //后退
        case '3' : gpio_reg_write(0x63); break; //左转
        case '4' : gpio_reg_write(0x9c); break; //右转
        case 'q' : exit(0);              break;
        default : gpio_reg_write(0x00); break;  //停止
      }
    }
  }
  

  exit(0);

  return 0;
}
