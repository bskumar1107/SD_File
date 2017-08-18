#include<lpc214x.h>
 void dela()
	{
	unsigned int i,j;
	for(i=0;i<0xff;i++)
	for(j=0;j<0xfff;j++);
	}
	
		 
	
      	
unsigned char keypad(void)
                     {

                        IO1DIR|=0x0F00<<16;  
 
			IO1PIN =(IO1PIN&0xF0FFFFFF)|0x0E00<<16; // First Scan Line
			if(( IO1PIN & 0xF000<<16 )!= 0xF000<<16) //Check if any key is pressed in
			//4th row
			{
			switch(IO1PIN & 0xF000<<16) //Check which one of the key is pressed
			{
			case 0x0007000<<16 : dela();return 'c' ;
			case 0x000B000<<16 : dela();return 'd' ;
			case 0x000D000<<16 : dela();return 'e' ;
			case 0x000E000<<16 : dela();return 'f' ;
			}
			}
			IO1PIN =(IO1PIN&0xF0FFFFFF)|0x0D00<<16;  //Move second data to scan line
			if(( IO1PIN & 0xF000<<16)!= 0xF000<<16) //Check if any key is pressed in 3rd row.

			{
			switch(IO1PIN & 0xF000<<16) //check which one of the key is pressed
			{
      case 0x0007000<<16 : dela();return '9' ;
			case 0x000B000<<16 : dela();return '0' ;
			case 0x000D000<<16 : dela();return 'a' ;
			case 0x000E000<<16 : dela();return 'b' ;
			}
			}
			IO1PIN =(IO1PIN&0xF0FFFFFF)|0x0B00<<16;  //Move 3rd scan data to port line
			if(( IO1PIN & 0xF000<<16 )!= 0xF000<<16)//Scan any key is pressed in 2nd row
			{
			switch(IO1PIN & 0xF000<<16) //Check which one of the key is
			//pressed in 2nd row
			{
			case 0x0007000<<16 : dela();return '5' ;
			case 0x000B000<<16 : dela();return '6' ;
			case 0x000D000<<16 : dela();return '7' ;
			case 0x000E000<<16 : dela();return '8' ;
			}
			}
			IO1PIN =(IO1PIN&0xF0FFFFFF)|0x0700<<16; //Move 4th scan data to port line
			if(( IO1PIN & 0xF000<<16 )!= 0xF000<<16) //Check any key is pressed in 1st
			// row
			{
			switch(IO1PIN & 0xF000<<16) //Check which one of the key is
			//pressed in 1st row
			{
			case 0x0007000<<16 : dela();return '1' ;
			case 0x000B000<<16 : dela();return '2' ;
			case 0x000D000<<16 : dela();return '3' ;
			case 0x000E000<<16 : dela();return '4' ;
			}
			}
			return 0xFF;
                        }

unsigned char key_wa(void)
			{
          unsigned char Read;
         while((Read=keypad())==0xFF);
         return Read;
			}				
		