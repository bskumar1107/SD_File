/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SD_FILE.C
 *      Purpose: File manipulation example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTL kernel functions & defines      */
#include <stdio.h>                    /* standard I/O .h-file                */
#include <ctype.h>                    /* character functions                 */
#include <string.h>                   /* string and memory functions         */
#include "File_Config.h"
#include "SD_File.h"
#include <lpc214x.h>
/* Command Functions */
static void dot_format (U64 val, char *sp);
static void cmd_cpy (char *fname,char *fnew ,char *fmer);
  extern void co(unsigned char );
  extern void da(unsigned char );
	extern void LCD(void);
	extern void lcds(unsigned char*);
	extern void tim(int);
	extern unsigned char keypad(void);
  void displ(int);
	extern unsigned char key_wa(void);
	void lcds1(unsigned char* );
/* Local constants */
static const char intro[] =
  "\n\n\n\n\n\n\n\n"
  "+-----------------------------------------------------------------------+\n"
  "|                SD/MMC Card File Manipulation example                  |\n";
static const char help[] = 
  "+ command ------------------+ function ---------------------------------+\n"
  "| CAP \"fname\" [/A]          | captures serial data to a file            |\n"
  "|                           |  [/A option appends data to a file]       |\n"
  "| FILL \"fname\" [nnnn]       | create a file filled with text            |\n"
  "|                           |  [nnnn - number of lines, default=1000]   |\n"
  "| TYPE \"fname\"              | displays the content of a text file       |\n"
  "| REN \"fname1\" \"fname2\"     | renames a file 'fname1' to 'fname2'       |\n"
  "| COPY \"fin\" [\"fin2\"] \"fout\"| copies a file 'fin' to 'fout' file        |\n"
  "|                           |  ['fin2' option merges 'fin' and 'fin2']  |\n"
  "| DEL \"fname\"               | deletes a file                            |\n"
  "| DIR \"[mask]\"              | displays a list of files in the directory |\n"
  "| FORMAT [label [/FAT32]]   | formats Flash Memory Card                 |\n"
  "|                           | [/FAT32 option selects FAT32 file system] |\n"
  "| HELP  or  ?               | displays this help                        |\n"
  "+---------------------------+-------------------------------------------+\n";





/*----------------------------------------------------------------------------
 *        Copy a File
 *---------------------------------------------------------------------------*/
static void cmd_cpy (char *fname,char *fnew ,char *fmer) {
  FILE *fin,*fout;
  U32 cnt,total,jk;
  char buf[512];
  BOOL merge;
	

  if (fname == NULL) {
    printf ("\nFilename missing.\n");
		co(0x01);
    return;
  }
  if (fmer == NULL) {
    printf ("\nNew Filename missing.\n");
    return;
  }
  if (fnew != NULL) {
    merge = __TRUE;
  }
  else {
    merge = __FALSE;
    fnew = fmer;
  }
  if ((strcmp (fname,fnew) == 0)        ||
      (merge && strcmp (fmer,fnew) == 0)) {
    printf ("\nNew name is the same.\n");
    return;
  }

  fin = fopen (fname,"r");            /* open the file for reading           */
  if (fin == NULL) {
    printf ("\nFile %s not found!\n",fname);
    return;
  }

  if (merge == __FALSE) {
    printf ("\nCopy file %s to %s\n",fname,fnew);
  }
  else {
    printf ("\nCopy file %s, %s to %s\n",fname,fmer,fnew);
  }
  fout = fopen (fnew,"w");            /* open the file for writing           */
  if (fout == NULL) {
    printf ("\nFailed to open %s for writing!\n",fnew);
    fclose (fin);
    return;
  }

  total = 0;
	co(0x01);
	 co(0x80);
	lcds("COPYING....");
  while ((cnt = fread (&buf, 1, 512, fin)) != 0) {
    fwrite (&buf, 1, cnt, fout);
	
	
    total += cnt;
  }
  fclose (fin);                       /* close input file when done          */

  if (merge == __TRUE) {
    fin = fopen (fmer,"r");           /* open the file for reading           */
    if (fin == NULL) {
      printf ("\nFile %s not found!\n",fmer);
    }
    else {
			co(0x01);
			co(0x80);
				lcds("COPYING....");
      while ((cnt = fread (&buf, 1, 512, fin)) != 0) {
        fwrite (&buf, 1, cnt, fout);
				
      }
      fclose (fin);
    }
  }
  fclose (fout);
  dot_format (total, &buf[0]);
  printf ("\n%s bytes copied.\n", &buf[0]);
	if (strcmp (&buf[0],"0")!=0)
	{
	co(0x01);
	lcds("File copied");
	tim(10);
	}
	else
	{
	co(0x01);
	lcds("File not copied");
	tim(10);
	}
}








/*----------------------------------------------------------------------------
 *        Print size in dotted fomat
 *---------------------------------------------------------------------------*/
static void dot_format (U64 val, char *sp) {

  if (val >= (U64)1e9) {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e9));
    val %= (U64)1e9;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= (U64)1e6) {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= 1000) {
    sprintf (sp,"%d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  sprintf (sp,"%d",(U32)(val));
}
/*----------------------------------------------------------------------------
 *        Initialize a Flash Memory Card
 *---------------------------------------------------------------------------*/
static void init_card (void) {
  U32 retv;
  
  while ((retv = finit (NULL)) != 0) {        /* Wait until the Card is ready*/
    if (retv == 1) {
      printf ("\nSD/MMC Init Failed");
      printf ("\nInsert Memory card and press key...\n");
			co(0x01);
			co(0x80);
			lcds(" FAILED TO INITIATE ");
	
			co(0xc0);
			lcds("INSERT CARD PROPERLY");
      key_wa();
      //getkey ();
    }
    else {
      printf ("\nSD/MMC Card is Unformatted");
			printf("%d",retv);
			continue;
     // strcpy (&in_line[0], "KEIL\r\n");
//      cmd_format (&in_line[0]);
    }
  }
}

/*----------------------------------------------------------------------------
 *       Display 
 *---------------------------------------------------------------------------*/
   void displ(int page )
   { 
		 FINFO iy;
		 int in=0;
		 int fi;
	   unsigned char Re;

	
     char g[30];
		 unsigned char spa[20][20];
		 iy.fileID=0;
		 co(0x01);
			 while(ffind("M0:\*.*",&iy)==0)
	    {
				 
				 in++;
			   strcpy(spa[in],(unsigned char * )iy.name);
				
		if(page==2&&in>8&&in<17)
     {
			  
			 
			 
			
				 if(in==9)
				 co(0x80);
         else if (in==10)
         co(0x8A);
         else if (in==11)
         co(0xc0);
      	 else if (in==12)
         co(0xca);
         else if (in==13)
         co(0x94);
         else if (in==14)
         co(0x9E);
         else if (in==15)
         co(0xD4);
         else if (in==16)
         co(0xDE);					 
				 da((in+48)-8);
				 da('.');
				 lcds1(spa[in]);
				 da(' '); 
							
			
			

			 }
	    }   
//      Re=key_wa();
// 			co(0x01);
// 			fi=(int)Re-48;
// 			da(fi%10+48);
// 			da('.');
// 			
// 			lcds1(spa[fi+8]);
// 			printf("%d",fi);
// 			lcds(" selected");
// 	    Re=key_wa();
// 			if(Re=='c')
// 			{	
// 			strcpy(g,"M1:\\");
// 		  strcat(g,(char *)spa[fi+8]);
//       cmd_cpy((char*)spa[fi+8],g,"");     
// 			}
		}
				
				
				
		    
     
  			 
	
 /*----------------------------------------------------------------------------
 *        String short
 *---------------------------------------------------------------------------*/
	   void lcds1(unsigned char* po)
	  {
			 int len3=0;
			 unsigned char* p1;
			p1=po;
			 while(*po!=0)
			 {
				 len3++;
				 po++;
			 }
			   po=p1 ;
			 if(len3>7)
			 {	 
//    			 for(;*po!=0;po++)
			   
// 					   if(po-len3||po-(len3+1)||po-(len3+2)||(po+len3)-3||(po+len3)-2||(po+len3)-1||(po+len3))
// 						 da(*po);
					 da(*po);
					 da(*(po+1) );
					 da(*(po+2) );
					 da(* ((po+len3)-4) );
					 da(* ((po+len3)-3) );
					 da(* ((po+len3)-2) );
					 da(* ((po+len3)-1) );
					 
				 }
				 else
				 {
					 while(*po!=0)
					 {
						 da(*po);
						 po++;
					 }
				 }
		 }
/*----------------------------------------------------------------------------
 *        Main: 
 *---------------------------------------------------------------------------*/
int main (void) {
  char *sp,*cp,*next;
	 U32 i;
	 int fi;
	 unsigned char Re;
	 unsigned char space[20][20];
	 int cou=0;
   char g[20];
	FINFO in;
	
 
	LCD();

		co(0x01);
		co(0x80);
		lcds("SD CARD DATA TRANSFR");
	  co(0xc0);
	  lcds("     LOADING.....   ");
	 init_comm ();                               /* init communication interface*/
   printf (intro);                             /* display example info        */
   printf (help);
   finit(NULL);	
	 init_card ();
	 while(1)
	 {
		 int lo;
   l:  co(0x01);
		  in.fileID=0;
		  cou=0;
				 co(0x80); 
	  while(ffind("M0:\*.*",&in)==0)
	    {
				 
				 cou++;
				 
				 
			   strcpy(space[cou],(unsigned char * )in.name);
				 
	  		if(cou<9)
				{	
				 if(cou==1)
				 co(0x80);
         else if (cou==2)
         co(0x8A);
         else if (cou==3)
         co(0xc0);
      	 else if (cou==4)
         co(0xca);
         else if (cou==5)
         co(0x94);
         else if (cou==6)
         co(0x9E);
         else if (cou==7)
         co(0xD4);
         else if (cou==8)
         co(0xDE);					 
				 da(cou+48);
				 da('.');
				 lcds1(space[cou]);
				 da(' '); 
				
			 }

				
			}   
			   
		
      Re=key_wa();
			co(0x01);
			fi=(int)Re-48;
			da(fi%10+48);
			if(fi<9)
			{	
			lcds1(space[fi]);
			printf("%d",fi);
			lcds(" selected");
			}
			else if (Re=='b')
			{
				displ(2);
			}				
			else
			goto l;	
			Re=key_wa();
			if(Re=='c')
			{	
			strcpy(g,"M1:\\");
		  strcat(g,(char *)space[fi]);
      cmd_cpy((char*)space[fi],g,"");     
			}
			else if(Re=='b')
			{
				displ(2);
				
      }				
      init_card();     				 
		}  
//   while (1) {
//     printf ("\nCmd> ");                       /* display prompt              */
// 	
// 			 
//            				 
//     fflush (stdout);
//                                              /* get command line input      */
//     if (getline (in_line, sizeof (in_line)) == __FALSE) {
//       continue;
//     }
// 		
//     printf("%s",in_line);
//     sp = get_entry (&in_line[0], &next);
//     if (*sp == 0) {
//       continue;
//     }
//     for (cp = sp; *cp && *cp != ' '; cp++) {
//       *cp = toupper (*cp);                    /* command to upper-case       */
//     }
//     for (i = 0; i < CMD_COUNT; i++) {
//       if (strcmp (sp, (const char *)&cmd[i].val)) {
//         continue;
//       }
//      init_card();
// 			/* check if card is removed    */
//       cmd[i].func (next);                     /* execute command function    */
//       break;
//     }
//     if (i == CMD_COUNT) {
//       printf ("\nCommand error\n");
//     }
//   }
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
