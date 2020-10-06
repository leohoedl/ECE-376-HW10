#include <pic18.h>
#include <math.h>
#include <stdio.h>

// Global Variables
 unsigned long int TIME, TIME0, TIME1,Final_TIME;

// Interrupt Service Routine

void interrupt IntServe(void) 
{
   if (TMR1IF) {
      TIME = TIME + 0x10000;
      TMR1IF = 0;
      }   
   if (CCP1IF) {  // rising edge
      TIME0 = TIME + CCPR1;
      CCP1IF = 0;
      }   
   if (CCP2IF) {  // falling edge
      TIME1 = TIME + CCPR2;
      CCP2IF = 0;
      }   
   }

// Subroutines
#include        "lcd_portd.c"


// Main Routine

void main(void)
{
   float HEIGHT_m;
   float HEIGHT_f,TEMP;
   unsigned int i;
   TRISA = 0;
   TRISB = 0xFF;
   TRISC = 0x04;	// capture every rising edge
   TRISD = 0;
   ADCON1 = 0x0F;
 
// set up Timer1 for PS = 1
   TMR1CS = 0;
   T1CON = 0x81;
   TMR1ON = 1;
   TMR1IE = 1;
   TMR1IP = 1;
   PEIE = 1;
// set up Capture1 for rising edges
   TRISC2 = 1;
   CCP1CON = 0x05;
   CCP1IE = 1;
   PEIE = 1;
// set up Capture2 for falling edges
   TRISC1 = 1;
   CCP2CON = 0x04;
   CCP2IE = 1;
   PEIE = 1;

   LCD_Init();           
   Wait_ms(1000);

   TIME = 0;

// turn on all interrupts
   GIE = 1;
   
   while(1) {
			Final_TIME = TIME1-TIME0;
			TEMP = Final_TIME / (20000000);
			if(RB1){
				HEIGHT_m = 4.9 * (TEMP)*(TEMP);
				HEIGHT_f = HEIGHT_m * 3.28;
				char array[21];
				sprintf(array, 21 ,"%f", HEIGHT_f);
            	LCD_Move(0,0);  LCD_Out(Final_TIME, 9, 7);
				//LCD_Move(1,0);  LCD_Out(HEIGHT_m,9,7);
				LCD_Move(1,0);	for (i=0;i<20;i++) LCD_Write(array[i]);
			}
      }
   }
