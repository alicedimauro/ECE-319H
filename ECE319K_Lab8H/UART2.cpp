/* UART2.cpp
 * Your name
 * Data:
 * PA22 UART2 Rx from other microcontroller PA8 IR output<br>
 */

#include "UART2.h"
#include "../inc/Clock.h"
#include "../inc/FIFO2.h"
#include "../inc/LaunchPad.h"
#include <ti/devices/msp/msp.h>


uint32_t LostData;
Queue FIFO2;

// power Domain PD0
// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 2375 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void) {
  // RSTCLR to GPIOA and UART2 peripherals
  // write this
  // 1. Enable UART2 clock and reset
  LostData = 0;
  UART2->GPRCM.RSTCTL = 0xB1000003; // Reset UART2
  UART2->GPRCM.PWREN = 0x26000001;  // Power enable UART2
  Clock_Delay(24);                  // Delay for activation

  // 2. Configure PA22 for UART2 RX (Receiver)  (Important: Check PA22 index)
  // bit 18 INENA input enable
  // bit 7 PC connected
  // bits 5-0=2 for UART2_RX
  IOMUX->SECCFG.PINCM[PA22INDEX] = 0x00040082; // Configure PA22 as UART2_RX

  // 3. Con figure the UART clock source and divider for 2375 bps
  UART2->CLKSEL = 0x08;     // Bus clock
  UART2->CLKDIV = 0x00;     // No division
  UART2->CTL0 &= ~0x01;     // Disable UART2
  UART2->CTL0 = 0x00020018; // Enable FIFO, TXE, RXE

  // 4. Disable UART2 before configuration

  // 5. Set the baud rate (2375 bps)
  // UART clock frequency: 40 MHz
  // Baud rate: 2375 bps
  // UART clock divider = 40,000,000 / (16 * 2375) = 1052.614  f
  // Integer part: 2105
  // Fractional part: 0.614 * 64 = ~40
  UART2->IBRD = 1052; // Integer part
  UART2->FBRD = 40;   // Fractional part

  // 6. Configure UART LCRH register (8-bit data, no parity, 1 stop bit)
  UART2->LCRH = 0x00000030; // 8-bit, 1 stop, no parity

  // 7. Configure UART CTL0 register (enable UART, TXE, RXE)
  UART2->CPU_INT.IMASK = 0x0C01;
  UART2->IFLS = 0x0400;
  NVIC->ICPR[0] = 1 << 15;
  NVIC->ISER[0] = 1 << 14;
  NVIC->IP[3] = (NVIC->IP[3] & (~0x00C00000)) | (0 << 22);
  UART2->CTL0 |= 0x01; // Enable UART2
}
//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void) {
  char out;
  // write this
  // return 0;
  if (FIFO2.IsEmpty()) {
    return 0; // Return 0 if the FIFO is empty
  } else {
    FIFO2.Get(&out); // Get data from the FIFO
    return out;      // Return the data
  }
}
extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void) {
  uint32_t status;
  char letter;
  uint32_t RxCounter = 0;
  status = UART2->CPU_INT.IIDX; // reading clears bit in RTOUT
  if (status == 0x01) {         // 0x01 receive timeout
    GPIOB->DOUTTGL31_0 = BLUE;  // toggle PB22 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = BLUE;  // toggle PB22 (minimally intrusive debugging)
                                // read all data, putting in FIFO
                                // finish writing this
    // Read all available data from UART2 and put it in the FIFO
    while ((UART2->STAT & 0x04) == 0) { // Check if RX FIFO is not empty
      letter = UART2->RXDATA;           // Read data
      if (!FIFO2.Put(letter)) {
        LostData++;
      }
      RxCounter++;
    }

    GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
  }
}
