#ifndef __BLUETOOTH_H_
#define __BLUETOOTH_H_

#include<reg51.h>
/**********************************
PIN¿Ú¶¨Òå
**********************************/
sbit led2 = P2^0;
sbit RX=P3^0;
sbit TX=P3^1;
void InitUART(void);
void SendOneByte(unsigned char c);

