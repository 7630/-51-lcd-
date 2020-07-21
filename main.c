/*******************************************************************************
注意事项：
根据自己使用的LCD1602是否带有转接板，如果带有转接板的即为4位，需在LCD.H头文件中
将宏#define LCD1602_4PINS打开，我们这里使用的LCD1602是8位，所以默认将该宏注释。

接线说明：（开发攻略每个实验章节内的实验现象都有对应的接线图说明）
LCD1602接线参考LCD1602液晶显示实验接线图
DS18B20传感器接线参考DS18B20温度传感器实验接线图



实验操作：
LCD1602上显示采集的温度，同时
打开串口调试助手，将串口波特率选择4800，打开串口，此时在串口上就会显示采集的温度，串口截图参考文件夹下。
*******************************************************************************/

#include<reg51.h>
#include"lcd.h"
#include"temp.h"
#include"bluetooth.h"
uchar CNCHAR[6] = "摄氏度";
void LcdDisplay(int);
void UsartConfiguration();
uchar recv_data ;
void lightControl();
/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void main()
{
	UsartConfiguration();
	LcdInit();			 //初始化LCD1602
	//LcdWriteData('C'); 
	while(1){
		string(0x80,"temperature");
		LcdWriteCom(0xc7);
		LcdWriteData('C'); 
		LcdDisplay(Ds18b20ReadTemp());
		Delay1ms(500);
		  if(RI)
    {
        RI = 0;
        //add your code here!			
			  recv_data = SBUF;			  
				LcdDisplay(Ds18b20ReadTemp());
			  lightControl();
				Delay1ms(500);	
    }
    else
       TI = 0;
	};
}

/*******************************************************************************
* 函数名         : LcdDisplay()
* 函数功能		   : LCD显示读取到的温度
* 输入           : v
* 输出         	 : 无
*******************************************************************************/

void LcdDisplay(int temp) 	 //lcd显示
{
    
  	unsigned char i, datas[] = {0, 0, 0, 0, 0}; //定义数组
	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
	  	LcdWriteCom(0xc0);		//写地址 80表示初始地址
		SBUF='-';//将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
	    LcdWriteData('-');  		//显示负
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
 
  	}
 	else
  	{			
	  	LcdWriteCom(0xc0);		//写地址 80表示初始地址
	    LcdWriteData('+'); 		//显示正
		SBUF='+';//将接收到的数据放入到发送寄存器
		while(!TI);			         //等待发送数据完成
		TI=0;						 //清除发送完成标志位
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	datas[0] = temp / 10000;
	datas[1] = temp % 10000 / 1000;
	datas[2] = temp % 1000 / 100;
	datas[3] = temp % 100 / 10;
	datas[4] = temp % 10;

	LcdWriteCom(0xc1);		  //写地址 80表示初始地址
	LcdWriteData('0'+datas[0]); //百位 
	SBUF = '0'+datas[0];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;
	
	LcdWriteCom(0xc2);		 //写地址 80表示初始地址
	LcdWriteData('0'+datas[1]); //十位
	SBUF = '0'+datas[1];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

	LcdWriteCom(0xc3);		//写地址 80表示初始地址
	LcdWriteData('0'+datas[2]); //个位 
	SBUF = '0'+datas[2];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

	LcdWriteCom(0xc4);		//写地址 80表示初始地址
	LcdWriteData('.'); 		//显示 ‘.’
	SBUF = '.';//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

	LcdWriteCom(0xc5);		 //写地址 80表示初始地址
	LcdWriteData('0'+datas[3]); //显示小数点  
	SBUF = '0'+datas[3];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;

	LcdWriteCom(0xc6);		 //写地址 80表示初始地址
	LcdWriteData('0'+datas[4]); //显示小数点 
	SBUF = '0'+datas[4];//将接收到的数据放入到发送寄存器
	while (!TI);			         //等待发送数据完成
	TI = 0;
	for(i=0; i<6; i++)
	{
	 	SBUF = CNCHAR[i];//将接收到的数据放入到发送寄存器
		while (!TI);			         //等待发送数据完成
		TI = 0;
	}

	 
}
/*******************************************************************************
* 函 数 名         :UsartConfiguration()
* 函数功能		   :设置串口
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void UsartConfiguration()
{
	 TMOD = 0x20;//设置计数器工作方式2
    SCON = 0x50;//设置为工作方式1
    TH1 = 0xFD;
    TL1 = TH1; //计数器初始值设置，注意波特率是9600的
    PCON = 0x00;//波特率不加倍
    EA = 1;	//打开总中断
    ES = 1;//打开接收中断
    TR1 = 1;	//打开计数器
}
/*小灯控制装置*/
void lightControl()
{
		if (recv_data == 'A')
		{
			led2=0;
		}
	   else if(recv_data == 'B'){
			led2=1;
		}
}
