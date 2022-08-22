/**
....................................................................................................
....................................................................................................
............             ..      ...                 ...             ..-..            .-............
..........-..=##########... %###-.. =###-.%#########@.. :###*...####*... .%###...%###.. ............
..........-. %##########+=. :###=#..####=*%#########@@: :###+#.=####+#.   @###=+ %###+= .-..........
..........-..%###+#%@###+=. .@###+=*###+#...#+###+##@=. :###+#-#####+#.   @###=+.%###+= .-..........
..........-..%###+= %###+=.  :###+@@###=+.   :###+#.    :###+%@#####+#.   @###=+.%###+= .-..........
.........-. .%###+= %###+=.  .@###-###+#..  .:###+#.  ..:###+*######+#. ..@###+**####+= .-..........
............#####+= %####*. .%#########:..  @######.  .%#############@. -#############*. .-.........
.........-...%###+= %###+#@. ..=#####+##=.  .:###+##*.  :######%:###+##: .@###=+=####+#%..-.........
..........-..%###+= %###+=.    .#####+=. .-. :###+#  .. :######++###+#.   @###=%.%###+= ............
..........-..%###+= %###+=.   ..+###=#. .... :###+#. -. :#####+#:###+#.   @###=+.%###+= .-..........
..........-..%###+= %###+=.  .+#####=+..-... :###+#. -. :####%@::###+#.   @###=+.%###+= .-..........
..........-..%###== =###+=.  .+###%==....... :###+#..-. :####+=.:###+#.  .@###=+.%###+= .-..........
............  .###-  .###-..-.  +%*. ......-  .=##+..--. .=##=.  .=##%. ....###:  .###- ............
............-..........  ..-..-................. ......--. ..............-. .  .....  ..-...........
............................................###*:.###.. ............................................
........................................... +#######*#- ............................................
.................-..............-.......--.  ..*+*+#%.  -...-.................    ..-...............
...........-+++-  ..*++- .-***.  ..***-  ..*+*. ..:***. .... .:****..........*%@@=-...-.............
............####*+.####*+.:###%%-.=###+= .*###+= *####+%..-. :#####+%. -...=########*...-...........
.........-. :###@*%###+#...@###+%.####+= .*###+#.#####+#. ...=#####@@: ...+###=+:####*- ............
.............+#######%@:  .:###+#:###+#. .*###+#=#####+#.   .#######+=.  .=###+@.%###+@..-..........
............ .%######=+.  ..@###*%###+=. .*###++######+#.   :#######+#.  .=###+@.%###+#.............
...........- ..#####*+.   ..=###=###@+. ..%###+@######%+...:####+####+- .-@###+@.%###=*. ...........
...........- .-#####*%=.  ..+#######=+%..-@#######%###@*%-.*####+%###+=+.:####+@ %###%+=............
............ .%#####@=+.     .#####=#+.  .*######++###+#-..:###=#+###+#.  =###+@ %###+#. ...........
..........-..+#######=#-     .*####+@  ...*#####%@+###+#. .=###+#-###@%: .=###+@.%###+#.............
.........-. :####*%###+#..  .-=###%@-..-..*#####+=:###+#. .####+@.@###+= .+###@*+####+@..-..........
............####+%.####+%.  :#####=+ .--..*####+#.:###+#..:###%@+.=###+#.  =########*#- ............
...........-===*#...+==*@+. .+=++#: .......==%*#:..===*#..-===*#- .===:#:.  .:%@%++#%...-...........
..............--.................. .-...... .--......--. .. .--. ....-.....-...  ... ...............
.................-..-.....-..-----.......................--......-.......-....--.-.-................
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "tim.h"
#include "gpio.h"
#include "adc.h"
#ifdef USE_USB_LOG
#include "usb_device.h"
#include "usbd_cdc_if.h"
#endif



#define THRESHOLD_UP  ( 3900 )
#define THRESHOLD_DN  ( 3700 )

#define WAVE          ( 60 )

#define DATA_SIZE     ( 4 )              // 4, 8, 16, 32
#define DATA_MASK     ( DATA_SIZE - 1 )

uint8_t full_buff = 0;

uint16_t now_data  = 0;
uint16_t last_data = 0;

uint16_t raw_data[DATA_SIZE];
uint8_t  raw_data_idx = 0;

uint16_t dif_data[DATA_SIZE];
uint8_t  dif_data_idx = 0;




/* System Clock Configuration */
void SystemClock_Config( void )
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /**Initializes the CPU, AHB and APB busses clocks  */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State          = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


  /**Initializes the CPU, AHB and APB busses clocks  */
  RCC_ClkInitStruct.ClockType       = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV2; //4
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV2;   //1
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  PeriphClkInit.PeriphClockSelection    = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection       = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  /**Configure the Systick interrupt time  */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  /**Configure the Systick   */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* set gift in drone */
void set_gift( void )
{
    LED_RED_On();
    MX_TIM2_Init();
    TIM2->CCR4 = 200;
    HAL_Delay( 3000 );
    MX_TIM2_DeInit();
    LED_RED_Off();
}

/* dropping gift moskaljam */
void dropping_gift( void )
{
    LED_RED_On();
    MX_TIM2_Init();
    TIM2->CCR4 = 200;
    HAL_Delay( 1000 );
    MX_TIM2_DeInit();
    HAL_Delay( 1000 );
    LED_RED_Off();
}

/* check ON/OFF led */
uint8_t check_led( void )
{
    last_data = now_data;

    // Read ADC value
    HAL_ADC_Start( &hadc );
    HAL_Delay( 2 );
    now_data = HAL_ADC_GetValue( &hadc );
    HAL_ADC_Stop( &hadc );

#ifdef USE_USB_LOG
    // Send debud in USB
    static char pack[10];
    __itoa( now_data, pack, 10 );
    uint8_t len = strlen( pack );
    pack[len] = 0x0D;
    pack[len + 1] = 0x0A;
    CDC_Transmit_FS( (uint8_t*)pack, len + 2 );
#endif

    if( now_data > THRESHOLD_DN )
    {
        // Checking data
        raw_data[raw_data_idx] = now_data;
        raw_data_idx++;
        raw_data_idx &= DATA_MASK;

        dif_data[dif_data_idx] = abs( now_data - last_data );
        dif_data_idx++;
        if( dif_data_idx == DATA_SIZE ) full_buff = 1;
        dif_data_idx &= DATA_MASK;

        // Analize data
        if( full_buff )
        {
            for( uint8_t i = 0; i < DATA_SIZE; i++ )
            {
                if( raw_data[i] < THRESHOLD_UP ) return 0;
                else
                if( dif_data[i] > WAVE ) return 0;
            }
            return 1;    // Led ON!!!
        }
    }
    else
    {
        full_buff    = 0;
        raw_data_idx = 0;
        dif_data_idx = 0;
    }

    return 0;  // Led OFF
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main( void )
{
  /* Init board */
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC_Init();
#ifdef USE_USB_LOG
  MX_USB_DEVICE_Init();
#endif  


  HAL_Delay( 1000 );
  set_gift();

  while( 1 )
  {
     HAL_Delay( 100 );
     if( check_led() ) dropping_gift();
  }

  return 0;
}

/**
  * @brief  This function recive data USB
  * @param  uint8_t* Buf, uint32_t* Len
  * @retval None
  */
void receive_usb_data( uint8_t* Buf, uint32_t* Len )
{
    (void)Buf;
    (void)Len;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
    /* User can add his own implementation to report the HAL error return state */
    while(1) {}
    (void)file;
    (void)line;
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

