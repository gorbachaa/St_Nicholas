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
#include "tim.h"
#include "gpio.h"
#include "adc.h"
#ifdef USE_USB_LOG
#include "usb_device.h"
#include "usbd_cdc_if.h"
#endif

#define TH0   ( 500 )
#define TH1   ( 1600 )
#define TH2   ( 3400 )


uint8_t flag = 0;


/*** System Clock Configuration*/
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

// set gift in drone
void set_gift( void )
{
    LED_RED_On();
    MX_TIM2_Init();
    TIM2->CCR4 = 200;
    HAL_Delay( 3000 );
    MX_TIM2_DeInit();
    LED_RED_Off();
}

// dropping gift moskaljam
void dropping_gift( void )
{
    LED_RED_On();
    MX_TIM2_Init();
    TIM2->CCR4 = 200;
    HAL_Delay( 1000 );
    MX_TIM2_DeInit();
    LED_RED_Off();
}

int check_led( void )
{
    HAL_ADC_Start( &hadc );
    HAL_Delay(2);
    uint16_t adc_tmp = HAL_ADC_GetValue( &hadc );
    HAL_ADC_Stop( &hadc );

#ifdef USE_USB_LOG
    static char pack[10];
    __itoa( adc_tmp, pack, 10 );
    uint8_t len = strlen( pack );
    pack[len] = 0x0D;
    pack[len + 1] = 0x0A;
    CDC_Transmit_FS( (uint8_t*)pack, len + 2 );
#endif

    return 0;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main( void )
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
#ifdef USE_USB_LOG
  /* USB init function */
  MX_USB_DEVICE_Init();
#endif
  /* ADC init function */
  MX_ADC_Init();


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

