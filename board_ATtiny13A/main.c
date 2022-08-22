#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <bits_macros.h>


//19200
//#define T1 0x37
//#define T0 0xFB
//----------------
//9600
#define T1 0x73
#define T0 0xDC
//---------------

uint8_t temp;
volatile uint8_t count;
volatile uint8_t start;
volatile uint8_t c;
volatile uint8_t Rece_bit;
volatile uint8_t rec;
volatile uint8_t usart_r;


// Прерывание чисто для определения стартового бита при приеме,
// используется редко, можно сюда повесить что-либо еще
ISR( PCINT0_vect )
{
  rec = 1;
}

ISR(TIM0_COMPA_vect){

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

void lov (uint8_t data2){

 if(count >= 8){
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

 if(c == 1){
    if(start == 0){
        temp = 0x80;
        start = 1;
        count--;
        goto lab;
       }

 temp = data2;
 temp = temp >> count;
 temp = temp << 7;

 lab:
 switch(temp){
       case 0x80 : PORTB &= ~(1 << 3);  break;
       case 0x00 : PORTB |= (1 << 3);   break;
      }
 count++;
 c = 0;

   }
}



void UART_trans(uint8_t data){
uint8_t f;

 data = ~data;
 TIMSK0 = 0x04;
 TCCR0B = 0x02; //!!!

 for(f = 0; f < 10; f++){
     while(c == 0);
     lov(data);
     }
 start = 0; temp = 0; c = 0; count = 0;
 TIMSK0 = 0; TCCR0B = 0; OCR0A = 0;
}

uint8_t UART_receiv(void){
uint8_t a, res = 0;

 GIMSK = 0x20;
 MCUCR = 0x00;
 PCMSK = 0x10;
 GIFR  = 0x20;

 while(rec == 0); // Ждать, пока не случится стартовый бит
       MCUCR = 0x00; // INT0 Interrupt
       GIMSK = 0x00; // INT0 Interrupt
       PCMSK = 0x00;
       GIFR  = 0x00;

       TIMSK0 = 0x04;
       TCCR0B = 0x02;
       rec = 0;
       TCNT0 = T0;
       for(a = 0; a < 9; a++){
           while(Rece_bit == 0);

           if(bit_is_set(PINB,4)) res |= (1 << 7);
              else                res &= ~(1 << 7);

           res = res >> 1;
           Rece_bit = 0;
          }
       return res;
}

int main( void )
{
    uint8_t R_byte;
    uint8_t R_BUF[11];
    uint8_t i = 0;


 DDRB &= ~(1<<4); //RX
 DDRB |= (1<<3);  //TX

 asm("sei");

      while(1)
      {  


     R_byte = UART_receiv();                 // получаем байт из UART
     if((R_byte != 157) & (R_byte != 158)){  // если не ошибка и не повтор - обрабатываем
 //        UART_trans(R_byte);
         R_BUF[i] = R_byte;
         if((R_byte == 0x0D)/*&(i == 3)*/){
              if(R_BUF[0] == 'M'){
                if(R_BUF[1] == '1'){ SetBit(DDRB, 0);   SetBit(PORTB, 0);  };
                if(R_BUF[1] == '0'){ ClearBit(DDRB, 0); ClearBit(PORTB, 0);};

                if(R_BUF[2] == '1'){ SetBit(DDRB, 2);   SetBit(PORTB, 2);  };
                if(R_BUF[2] == '0'){ ClearBit(DDRB, 2); ClearBit(PORTB, 2);};
                }
             i = 0; R_BUF[0] = 0; R_BUF[1] = 0; R_BUF[2] = 0;
           }
           else{ i++; if(i >= 10) i = 0; }
     };


     }
}
