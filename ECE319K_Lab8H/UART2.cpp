/* UART2.cpp
 * Your name
 * Data:
 * PA22 UART2 Rx from other microcontroller PA8 IR output<br>
 */


#include <ti/devices/msp/msp.h>
#include "UART2.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/FIFO2.h"

uint32_t LostData;
Queue FIFO2;

// power Domain PD0
// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 2375 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void){
    // RSTCLR to GPIOA and UART2 peripherals
   // write this
// 1. Enable UART2 clock and reset
    UART2->GPRCM.RSTCTL = 0xB1000003;  // Reset UART2
    UART2->GPRCM.PWREN  = 0x26000001;  // Power enable UART2
    Clock_Delay(24);                    // Delay for activation

    // 2. Configure PA22 for UART2 RX (Receiver)  (Important: Check PA22 index)
    IOMUX->SECCFG.PINCM[PA22INDEX] = 0x00040082; // Configure PA22 as UART2_RX
    // bit 18 INENA input enable
    // bit 7 PC connected
    // bits 5-0=2 for UART2_Rx

    // 3. Configure the UART clock source and divider for 2375 bps
    UART2->CLKSEL = 0x08;              // Bus clock
    UART2->CLKDIV = 0x00;              // No division

    // 4. Disable UART2 before configuration
    UART2->CTL0 &= ~0x01;              // Disable UART2

    // 5. Set the baud rate (2375 bps)
    // UART clock frequency: 48 MHz
    // Baud rate: 2375 bps
    // UART clock divider = 48,000,000 / (16 * 2375) = 1263.15789
    // Integer part: 1263
    // Fractional part: 0.15789 * 64 = ~10
    UART2->IBRD = 1263;                 // Integer part
    UART2->FBRD = 10;                  // Fractional part

    // 6. Configure UART LCRH register (8-bit data, no parity, 1 stop bit)
    UART2->LCRH = 0x00000030;          // 8-bit, 1 stop, no parity

    // 7. Configure UART CTL0 register (enable UART, TXE, RXE)
    UART2->CTL0 = 0x00020018;           // Enable FIFO, TXE, RXE
    UART2->CTL0 |= 0x01;              // Enable UART2

}
//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void){char out;
// write this
  // return 0;
  while((UART0->STAT&0x04) == 0x04){}; // wait while not input
   return((char)(UART0->RXDATA));
}

extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ uint32_t status; char letter;
  status = UART2->CPU_INT.IIDX; // reading clears bit in RTOUT
  if(status == 0x01){   // 0x01 receive timeout
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    // read all data, putting in FIFO
    // finish writing this

    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
  }
}
