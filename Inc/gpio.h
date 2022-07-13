/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for
  *                      the gpio
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/


#define LED_1_Pin           GPIO_PIN_0
#define LED_1_GPIO_Port     GPIOF
#define LED_2_Pin           GPIO_PIN_1
#define LED_2_GPIO_Port     GPIOF


#define CH_3_Pin            GPIO_PIN_1
#define CH_3_GPIO_Port      GPIOA
#define CH_1_Pin            GPIO_PIN_2
#define CH_1_GPIO_Port      GPIOA






void MX_GPIO_Init( void );

void LED_RED_On( void );
void LED_RED_Off( void );

void LED_GREEN_On( void );
void LED_GREEN_Off( void );

void CH_1_On( void );
void CH_1_Off( void );

void CH_3_On( void );
void CH_3_Off( void );





#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
