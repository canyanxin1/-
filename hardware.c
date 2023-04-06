
#include "common.h"


//*****************************************************************
void uart_putchar ( unsigned char c)
{
  
    while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
		USART1->DR = ( unsigned char) c;
}
 /*
****************************************************************
*  串口初始化 默认是串口1 波特率9600 
*****************************************************************
*/
void uart_putstr(char ch[])
{
  unsigned char ptr=0;
  while(ch[ptr]){
      uart_putchar((unsigned char)ch[ptr++]);
  } 
}

 /*
****************************************************************
*  配合串口示波器的校验协议
*****************************************************************
*/
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
/*
****************************************************************
*    配合串口示波器的输出协议，float 字长设为16位！！！注意工程
*    设置有的是32位!
*  1  可以输出4路信息，每路数据长度是16位。
*  2  每次输出字节总数10个，最后两个字节为校验
*****************************************************************
*/
void OutPut_Data(float OutData[4])
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
   {
    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];
    
   }
   
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  CRC16 = CRC_CHECK(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
    uart_putchar(databuf[i]);
}







