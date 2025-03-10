/* UARTbusywaitTestmain.c
 * Jonathan Valvano
 * November 18, 2022
 * Derived from uart_rw_multibyte_fifo_poll_LP_MSPM0G3507_nortos_ticlang
 */


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include <stdio.h>

// PA10 is UART0 Tx    index 20 in IOMUX PINCM table
// PA11 is UART0 Rx    index 21 in IOMUX PINCM table
//  PA0 is red LED1,   index 0 in IOMUX PINCM table
// PB22 is BLUE LED2,  index 49 in IOMUX PINCM table
// PB26 is RED LED2,   index 56 in IOMUX PINCM table
// PB27 is GREEN LED2, index 57 in IOMUX PINCM table
// PA18 is S2 positive logic switch,  index 39 in IOMUX PINCM table
// PB21 is S3 negative logic switch,  index 48 in IOMUX PINCM table
// Insert jumper J25: Connects PA10 to XDS_UART
// Insert jumper J26: Connects PA11 to XDS_UART

int main1(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  UART_Init();
  UART_OutString("\n\rHello world\n\r");
  while(1){
   char letter = UART_InChar();
   UART_OutChar(letter);
   if(letter == LF) UART_OutChar(CR);
   if(letter == CR) UART_OutChar(LF);
   GPIOB->DOUT31_0 ^= (  RED );
  }
}

uint32_t side; // room wall meters
uint32_t area; // size squared meters
// note: sorry, but scanf does not work
int main2(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  UART_InitPrintf();
  printf("\n\rThis program calculates areas of square-shaped rooms\n\r");
  while(1){
    printf("Give room side: ");  // 1) ask for input
    side = UART_InUDec();        // 2) wait for input
    area = side*side;            // 3) calculation
    printf("\n\rside = %u m, area = %u sqr m\n\r", side, area); // 4) out
  }
}
// Program 2.4.1
int main(void){
  Clock_Init40MHz();
  LaunchPad_Init();
  UART_InitPrintf();
  printf("Hello world");
  while(1){
  }
}
char Letter;
int main4(void){
  Clock_Init80MHz(0);
  LaunchPad_Init();
  UART1_Init();
  while(1){
    UART1_OutChar('A');
    Letter = UART1_InChar();
  }
}


