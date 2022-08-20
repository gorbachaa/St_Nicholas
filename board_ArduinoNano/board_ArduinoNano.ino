//----------------------------------------
// St_Nicholas
// Dropping Gifts - board ArduinoNano

#include <Servo.h>

// set debug to serial
#define DEBUG    ( 1 )
// set angle of rotation
#define ANGLE    ( 70 )
// set pin PWM for SG90
#define PWM      ( 12 )
// set empty pin
#define EMPTY    ( 11 )
// set sensor pin
#define SENSOR   ( A0 )

Servo servo;


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
}

/* check ON/OFF led */
int check_led()
{
  long sensorValue = analogRead( SENSOR );
  if( DEBUG ) Serial.println( sensorValue );

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
