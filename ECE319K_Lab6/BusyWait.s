// BusyWait.s
// Student names: Alice Di Mauro and Ravi Shah
// Last modification date: change this to the last modification date or look very silly

// Note: these functions do not actually output to SPI or Port A. 
// They are called by the grader to see if the functions would work

// As part of Lab 6, students need to implement these two functions

      .global   SPIOutCommand
      .global   SPIOutData
      .text
      .align 2

// ***********SPIOutCommand*****************
// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: R0 = 32-bit command (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI tx data register address
//         R3 = 32-bit GPIOA->DOUTCLR31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the clear register to clear will make it friendly
SPIOutCommand:
// --UUU-- Code to write a command to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
//3) Clear D/C (GPIO PA13) to zero, be friendly (R3 has address of GPIOA->DOUTCLR31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTCLR31_0
//4) Write the command to the SPI data register (R2 has address of SPI1->TXDATA)
//5) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
    PUSH {R4-R5, LR}

step1command:
    LDR R4, [R1] // R1 has the address of SPI1 -> STAT
    LDR R5, =(1<<4) // Load bit mask
    ANDS R4, R4, R5 // != 0 if bit 4 is high
    BNE step1command

    LDR R4, =0x2000
    STR R4, [R3] // Write 0x2000 to GPIOA->DOUTCLR31_0
    
    STR R0, [R2] // Write command to SPI data register
step5command:
    LDR R4, [R1] // Read SPI data register and check bit 4
    LDR R5, =(1<<4) // Load bit mask
    ANDS R4, R4, R5 // != 0 if bit 4 is high
    BNE step5command

    POP {R4-R5, PC}    //   return



// ***********SPIOutData*****************
// This is a helper function that sends an 8-bit data to the LCD.
// Inputs: R0 = 32-bit data (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI data register address
//         R3 = 32-bit GPIOA->DOUTSET31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the set register to clear will make it friendly
SPIOutData:
// --UUU-- Code to write data to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 1,
//2) If bit 1 is low, loop back to step 1 (wait for TNF bit to be high)
//3) Set D/C (GPIO PA13) to one, be friendly (R3 has address of GPIOA->DOUTSET31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTSET31_0
//4) Write the data to the SPI data register (R2 has address of SPI1->TXDATA)
    PUSH {R4-R5, LR}
step1data: 
    LDR R4, [R1] // Read SPI status register
    LDR R5, =(1<<1) // Mask for bit 1
    ANDS R4, R4, R5 // Check bit 1
    BEQ step1data

    LDR R4, =0x2000
    STR R4, [R3]

    STR R0, [R2] // Write data to SPI -> TXDATA

    POP {R4-R5, PC}   // return
//****************************************************

    .end
