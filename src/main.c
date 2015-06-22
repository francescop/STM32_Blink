#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f4xx_conf.h"
#include "utils.h"
#include "delay.h"

unsigned int btnPressed = 0;

void setup();
void loop();
void rotate();

int main(void) {
	setup();
  while (1) {
    loop();
  }
	return 0;
}

void loop(){
  //uint8_t in = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
  uint16_t in = GPIO_ReadInputData(GPIOA);

  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

  if(in&0x0001) {
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
  } else {
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    delay(500);
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);
    delay(500);
  }

}

void setup() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
 * Dummy function to avoid compiler error
 */
void _init() {
}

