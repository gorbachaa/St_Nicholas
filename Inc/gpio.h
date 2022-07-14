
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"


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

