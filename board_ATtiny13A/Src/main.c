#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>




#ifdef USE_UART_LOG

//19200
//#define T1 0x37
//#define T0 0xFB
//----------------
//9600
#define T1 0x73
#define T0 0xDC
//---------------

volatile uint8_t temp;
volatile uint8_t count;
volatile uint8_t start;
volatile uint8_t c;
volatile uint8_t Rece_bit;

ISR( TIM0_COMPA_vect )
{

  TIMSK0 = 0x00;
  TCCR0B = 0x00;   // Единственный Таймер, используется для формирования четких промежуток
  OCR0A  = 0x00;   // между битами, как при приеме так и при передачи
  c      = 0x01;
  TCNT0  = 0x00;
  TIMSK0 = 0x04;
  TCCR0B = 0x02;   // Пределитель
                   // Значение "сброс при совпадении" загружается каждый раз из переменной
  OCR0A  = T1;     // Можно быстро менять скорости UART
  Rece_bit = 1;
}

void lov( uint8_t data2 )
{
  if( count >= 8 )
  {
    PORTB |= (1 << 3);
    start = 0;
    temp = 0;
    c = 0;
    count = 0;
    TIMSK0 = 0;
    TCCR0B = 0;
    OCR0A = 0;
    return;
  }

  if( c == 1 )
  {
    if( start == 0 )
    {
      temp = 0x80;
      start = 1;
      count--;
      goto lab;
    }

    temp = data2;
    temp = temp >> count;
    temp = temp << 7;

    lab:
    switch( temp )
    {
      case 0x80 : PORTB &= ~( 1 << 3 );  break;
      case 0x00 : PORTB |= ( 1 << 3 );   break;
    }
    count++;
    c = 0;
  }
}

void UART_trans( uint8_t data )
{
  data = ~data;
  TIMSK0 = 0x04;
  TCCR0B = 0x02; //!!!

  for( uint8_t f = 0; f < 10; f++ )
  {
    while( c == 0 );
    lov( data );
  }
  start = 0; temp = 0; c = 0; count = 0;
  TIMSK0 = 0; TCCR0B = 0; OCR0A = 0;
}

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


/* set gift in drone */
void set_gift( void )
{

}

/* dropping gift moskaljam */
void dropping_gift( void )
{

}

/* check ON/OFF led */
uint8_t check_led( void )
{
    last_data = now_data;

    // Read ADC value
    //HAL_ADC_Start( &hadc );
    //HAL_Delay( 2 );
    //now_data = HAL_ADC_GetValue( &hadc );
    //HAL_ADC_Stop( &hadc );

#ifdef USE_UART_LOG
    // Send debud in UART
    //UART_trans( x );
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



int main( void )
{
    DDRB |= ( 1 << 3 );  //TX

    _delay_ms( 1000 );
    set_gift();

    while( 1 )
    {
      _delay_ms( 100 );
      if( check_led() ) dropping_gift();              
    }
}
