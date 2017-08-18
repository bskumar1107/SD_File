#include<LPC214x.h>
#define rs 8
#define rw 9
#define en 10
 
int k;

//-----------------------------COMMAND---------------------------------/	 
 void co(unsigned char x)
  {
   IO1PIN=(IO1PIN&0xFF00FFFF)|x<<16;
   IO0CLR=1<<rs;
   IO0CLR=1<<rw;
   IO0SET=1<<en;
   for(k=0;k<100000;k++);
   IO0CLR=1<<en;
  }
//-------------------------------DATA----------------------------------/
 void da(unsigned char y)
  {
   IO1PIN=(IO1PIN&0xFF00FFFF)|y<<16;
   IO0SET=1<<rs;
   IO0CLR=1<<rw;
   IO0SET=1<<en;
   for(k=0;k<100000;k++);
   IO0CLR=1<<en;
  }
	//----------------------------LCD INITIALIZATION------------------------/
void LCD(void)
  {
   unsigned char i[]={0x38,0x0f,0x01,0x06},j;
	 IO1DIR|=0x00FF0000;
	 IO0DIR|=0x00000700;
	 for(k=0;k<1000;k++);
   for(j=0;j<4;j++)
   {
    co(i[j]);
		for(k=0;k<200000;k++);
   }
   }
//-------------------------------LCDSTRING------------------------------/
  void lcds(unsigned char* p)
  {
   for(;*p!=0;p++)
    {
      da(*p);
			for(k=0;k<200000;k++);
		}
	}		
	//-------------------------------TIMER--------------------------------/
void tim(int x)
  {	
     T0CTCR=0x00;
	   T0PR=14999999;
	 //T0TCR=0x02;
     T0TCR=0x02;
	   T0TCR=0x01;
	   while(T0TC<x);
	   T0TCR=0x00;
	   T0TC=0;
   }
	 //--------------------------------END-----------------------------------/	
	
	 
	 