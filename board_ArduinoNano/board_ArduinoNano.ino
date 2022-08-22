//----------------------------------------
// St_Nicholas
// Dropping Gifts - board ArduinoNano

#include <Servo.h>

// set debug to serial
#define DEBUG    ( 0 )

// set angle of rotation
#define ANGLE    ( 70 )
// set pin PWM for SG90
#define PWM      ( 12 )
// set empty pin
#define EMPTY    ( 11 )
// set sensor pin
#define SENSOR   ( A0 )

Servo servo;

#define THRESHOLD_UP  ( 980 )
#define THRESHOLD_DN  ( 970 )

#define WAVE          ( 20 )

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
void set_gift()
{
    servo.attach( PWM );
    servo.write( ANGLE );
    delay( 3000 );
    servo.write( 0 );
    delay( 500 );
    servo.attach( EMPTY );
}

/* dropping gift moskaljam */
void dropping_gift()
{
    servo.attach( PWM );
    servo.write( ANGLE );
    delay( 1000 );
    servo.write( 0 );
    delay( 500 );
    servo.attach( EMPTY );
    delay( 1000 );
}

/* check ON/OFF led */
uint8_t check_led()
{
    last_data = now_data;

    // Read ADC value
    now_data = analogRead( SENSOR );
    if( DEBUG ) Serial.println( now_data );

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

    return 0;
}

/* init board */
void setup()
{
    if( DEBUG ) Serial.begin( 115200 );
    delay( 1000 );
    set_gift();
}

/* main cycle */
void loop() 
{ 
    delay( 100 );
    if( check_led() ) dropping_gift();
}
