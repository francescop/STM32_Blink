#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx_conf.h"
#include "utils.h"
#include "delay.h"

void setup();
void loop();

int main(void) {
	setup();
  while (1) {
    loop();
  }
	return 0;
}

void loop(){
    Print("ciao\n");
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    Delay(1000);

    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    Delay(1000);
    
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    Delay(1000);

    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    Delay(1000);
}

void setup() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	// ------ UART ------ //

	// Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {
}

