
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"



extern ADC_HandleTypeDef hadc;



void MX_ADC_Init( void );


#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

