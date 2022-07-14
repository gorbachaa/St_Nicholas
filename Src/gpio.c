/**
 *****************************************************
 * GPIO module
 *****************************************************
 **/

#include "gpio.h"

GPIO_InitTypeDef GPIO_InitStruct;


void MX_GPIO_Init( void )
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( GPIOA, CH_3_Pin | CH_1_Pin, GPIO_PIN_RESET );

    /*Configure GPIO pins : LED_1_Pin LED_2_Pin */
    GPIO_InitStruct.Pin  = LED_1_Pin | LED_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );

    /*Configure GPIO pins : LED_1_Pin LED_2_Pin */
    GPIO_InitStruct.Pin  = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    /*Configure GPIO pins : CH_4_Pin CH_3_Pin CH_1_Pin CH_2_Pin */
    GPIO_InitStruct.Pin  = CH_3_Pin | CH_1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

    HAL_GPIO_WritePin( GPIOA, CH_3_Pin | CH_1_Pin, GPIO_PIN_SET );
}

void LED_RED_On( void )
{
    GPIO_InitStruct.Pin  = LED_1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
}

void LED_RED_Off( void )
{
    GPIO_InitStruct.Pin  = LED_1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
}

void LED_GREEN_On( void )
{
    GPIO_InitStruct.Pin  = LED_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
}

void LED_GREEN_Off( void )
{
    GPIO_InitStruct.Pin  = LED_2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
}

void CH_1_On( void )
{
    HAL_GPIO_WritePin( GPIOA, CH_1_Pin, GPIO_PIN_RESET );
}

void CH_1_Off( void )
{
    HAL_GPIO_WritePin( GPIOA, CH_1_Pin, GPIO_PIN_SET );
}

void CH_3_On( void )
{
    HAL_GPIO_WritePin( GPIOA, CH_3_Pin, GPIO_PIN_RESET );
}

void CH_3_Off( void )
{
    HAL_GPIO_WritePin( GPIOA, CH_3_Pin, GPIO_PIN_SET );
}


