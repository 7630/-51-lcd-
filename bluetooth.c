#include "bluetooth.h"

void SendOneByte(unsigned char c)
{
    SBUF = c;
    while(!TI);
    TI = 0;
}

// �����ַ��� 
void uart_send_str(unsigned char *s) 
{ while(*s != '\0')// '\0':�ַ���������־ 
 { SendOneByte(*s);// ���� 1 ���ֽ����ݣ�1 ���ַ�ռ 8 λ��1 �ֽ� 
  s++;// ָ����һ���ַ� 
 } 
} 
