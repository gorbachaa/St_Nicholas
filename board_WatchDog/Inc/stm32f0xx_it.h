

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0xx_IT_H
#define __STM32F0xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 



void NMI_Handler( void );
void HardFault_Handler( void );
void SVC_Handler( void );
void PendSV_Handler( void );
void SysTick_Handler( void );
void USB_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_IT_H */

