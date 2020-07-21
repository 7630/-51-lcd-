#include "bluetooth.h"

void SendOneByte(unsigned char c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

// 发送字符串 
void uart_send_str(unsigned char *s) 
{ while(*s != '\0')// '\0':字符串结束标志 
 { SendOneByte(*s);// 发送 1 个字节数据，1 个字符占 8 位，1 字节 
  s++;// 指向下一个字符 
 } 
} 
