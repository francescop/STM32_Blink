/*
 * utils.c
 *
 *  Created on: 11 jul 2012
 *      Author: benjamin
 */
#include "utils.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>

/*
 * Can be used with printf when double values are not
 * supported (because hardfloat is used)
 */
char* ftostr(float value, int places) {
	static char buffer[100];
	uint32_t whole;
	uint32_t fraction;
	char sign[2] = "";

	if (value < 0) {
		value = -value;
		sign[0] = '-';
		sign[1] = '\0';
	}

	whole = (uint32_t) value;
	fraction = (uint32_t) ((value - floorf(value)) * powf(10.0f, (float)places) + 0.5f);
	sprintf(buffer, "%s%lu.%*.*lu", sign, whole, places, places, fraction);

	return buffer;
}

int fputc(int c, FILE *stream) {
   return(ITM_SendChar(c));
}

void Print(const char *msg) {
 // Manual semi-hosting, because the GCC ARM Embedded's semihosting wasn't working.
 for (; *msg; ++msg) {
  // Moves a pointer to msg into r1, sets r0 to 0x03,
  // and then performs a special breakpoint that OpenOCD sees as
  // the semihosting call. r0 tells OpenOCD which semihosting
  // function we're calling. In this case WRITEC, which writes
  // a single char pointed to by r1 to the console.
  __asm__ ("mov r1,%0; mov r0,$3; BKPT 0xAB" :
                                             : "r" (msg)
                                             : "r0", "r1"
  );
 }
}
