
#include "common.h"


//*****************************************************************
void uart_putchar ( unsigned char c)
{
  
    while((USART1->SR & 0X40) == 0); //ѭ������,ֱ���������
		USART1->DR = ( unsigned char) c;
}
 /*
****************************************************************
*  ���ڳ�ʼ�� Ĭ���Ǵ���1 ������9600 
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
*  ��ϴ���ʾ������У��Э��
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
*    ��ϴ���ʾ���������Э�飬float �ֳ���Ϊ16λ������ע�⹤��
*    �����е���32λ!
*  1  �������4·��Ϣ��ÿ·���ݳ�����16λ��
*  2  ÿ������ֽ�����10������������ֽ�ΪУ��
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







