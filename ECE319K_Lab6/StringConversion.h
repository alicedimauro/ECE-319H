// StringConversion.h
// This software has string conversion function
// Runs on any microcontroller
// Program written by: put your names here
// Date Created: 
// Last Modified:  12/11/2024
// Lab number: 6

#ifndef STRINGCONVERSION_H
#define STRINGCONVERSION_H
#include <stdint.h>

// test function for udivby10
void Test_udivby10(void);

//-----------------------OutDec-----------------------
// Convert a 16-bit number into unsigned decimal format
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// OutChar does not do actual output, OutChar does virtual output used by the grader
// Input: x (call by value) 16-bit unsigned number
// Output: none
void OutDec(uint16_t x);



#endif
