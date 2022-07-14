
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"



/**
  * Initializes the Global MSP.
  */
void HAL_MspInit( void )
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_REMAP_PIN_ENABLE( HAL_REMAP_PA11_PA12 );
}

