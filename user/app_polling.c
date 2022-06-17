/*
 * Below is the given application for lab4_1.
 * The goal of this app is to control the car via Bluetooth. 
 */

#include "user_lib.h"
#include "util/types.h"

int main(void) {
  printu("please input the instruction through bluetooth!\n");
  while(1)
  {
    char temp = (char)uartgetchar();
    uartputchar(temp);
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
  exit(0);
  return 0;
}
