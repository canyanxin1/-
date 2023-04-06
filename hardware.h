#ifndef _HARDWARE_H
#define _HARDWARE_H

//-------------串口--------------------
void uart_putchar (unsigned  char c);
void uart_putstr(char ch[]);
//--------------串口输出---------------
 void OutPut_Data(float OutData[4]);
//外部声明的变量
extern float OutData[4];
#endif 

