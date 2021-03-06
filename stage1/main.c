/**
  ******************************************************************************
  * @file    stage1/main.c
  * @author  Daniel Fitzmaurice
  * @date    10-January-2015
  * @brief   Prac 1 Template C main file - BCD timer and press counter.
  *			 NOTE: THIS CODE IS PSEUDOCODE AND DOES NOT COMPILE.
  *				   GUIDELINES ARE GIVEN AS COMMENTS.
  *			 REFERENCES: ex1_led, ex2_gpio, ex3_gpio, ex11_character
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <board.h>
#include <stm32f4xx_hal_conf.h>
#include <debug_printf.h>
#include "s4396122_hal_ledbar.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t counter_value = 0x03; //! Some comment here
unsigned char left_bool = 1;
uint16_t press_counter_val = 0;
unsigned int last_press = 0;
unsigned char rising_val = 0;
uint16_t delayFactor = 10000;

/* Private function prototypes -----------------------------------------------*/
void Hardware_init(void);
void exti_a2_interrupt_handler(void);

/**
  * @brief  Main program - timer and press counter.
  * @param  None
  * @retval None
  */
void main(void) {

	BRD_init();	//Initalise NP2
	Hardware_init();	//Initalise hardware modules

	/* Main processing loop */
  	while (1) {

		s4396122_hal_ledbar_write(counter_value);
		debug_printf("0x%04x\n", counter_value);

		if (left_bool) {
			counter_value = counter_value << 1;
		} else {
			counter_value = counter_value >> 1;
		}

		if (counter_value == 3) {
			left_bool = 1;
			counter_value = 3;
		} else if (counter_value == 768) {
			left_bool = 0;
			counter_value = 768;
		}

		BRD_LEDToggle();
    	HAL_Delay(delayFactor);		//Delay for 1s (1000ms)

	}
}

/**
  * @brief  Initialise Hardware
  * @param  None
  * @retval None
  */
void Hardware_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	BRD_LEDInit();		//Initialise Blue LED
	BRD_LEDOff();		//Turn off Blue LED

	s4396122_hal_ledbar_init();

	__BRD_A2_GPIO_CLK();
	HAL_NVIC_SetPriority(BRD_A2_EXTI_IRQ, 10, 0);
	NVIC_SetVector(BRD_A2_EXTI_IRQ, (uint32_t) &exti_a2_interrupt_handler);
	NVIC_EnableIRQ(BRD_A2_EXTI_IRQ);
	GPIO_InitStructure.Pin = BRD_A2_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(BRD_A2_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  exti_a2 GPIO Interrupt handler
  * @param  None.
  * @retval None
  */
void exti_a2_interrupt_handler(void) {

	HAL_GPIO_EXTI_IRQHandler(BRD_A2_PIN);				//Clear A2 pin external interrupt flag

	if (HAL_GetTick() > (last_press + 300)) {
		if (rising_val) {
			delayFactor *= 2;
			if (delayFactor >= 1000) {
				delayFactor = 1000;
				rising_val = 0;
			}
		} else {
			delayFactor /= 2;
			if (delayFactor <= 100) {
				delayFactor = 100;
				rising_val = 1;
			}
		}
	}

	last_press = HAL_GetTick();
}
