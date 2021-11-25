#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <wiring.c>

// defines variables
long duration;
int distance;
int safetyDistance;
int dutyCyc = 400;

//led 8 OUTPUT
//trig 9
//echo 10
//buzz 3 OUTPUT
//pButton 2 INPUT
//servo 11 OUTPUT

int main (){
  Serial.begin(9600);
  // SETUP INPUT OUTPUT FOR LED TRIG ECHO
  DDRB = B11111011;
  // LED AS OUT, SERVO AS OUT
  PORTB = B10011110;

  // SETUP PBUTTON BIT KE 3 DAN BUZZER BIT KE 4
  DDRD = 0xF0;
  PORTD = 0x0F;

  // SETUP PWM TIMER 2
  TCCR2A = (1 << COM2A1) | (1 << WGM20) | (1 << WGM21);
  TIMSK2 = (1 << TOIE2);
  OCR2A = 250;
  TCCR2B = (1 << CS20);
  
  // SETUP EXTERNAL INTERRUPT INT0 TEPI NAIK
  EICRA = 1<<ISC01 | 1<<ISC00;
  EIMSK = 1 << INT0;

  sei();

  while (1){
    // TRIG DI LOWKAN
    PORTB &= B11111101;
    _delay_ms(2);
    // TRIG DI HIGH KAN
    PORTB |= B00000010;
    _delay_ms(10);
    // TRIG DI LOW KAN
    PORTB &= B11111101;

    duration = pulseIn(B1010, HIGH);
    distance= duration*0.034/2;
    safetyDistance = distance;

    if (safetyDistance <= 25){
      // TURN ON LED & BUZZ
      PORTB |= B00001001;
      PORTD = 0xFF;
    }
    else{
      // TURN OFF
      PORTB &= B11111110; 
      PORTD = 0x0F;
    }
    
    dutyCyc = 400;
    _delay_ms(250);
    OCR2A = 1999 - dutyCyc;

    _delay_ms(1);
    if(dutyCyc >= 180){
      dutyCyc = 0;
    }
  }
}

ISR(TIMER2_OVF_vect){
    OCR2A = 250;
    OCR2A = 3999 + dutyCyc;
}

ISR(INT0_vect){
  // INTERRUPT TURN ON LED BUZZ
   PORTB |= B00001001; 
   PORTD = 0xFF;
   _delay_ms(500);
   // TURN OFF
   PORTB &= B11111110;
   PORTD = 0x0F;
   _delay_ms(500);
   // TURN ON
   PORTB |= B00001001; 
   PORTD = 0xFF;
   _delay_ms(500);
   // TURN OFF
   PORTB &= B11111110;
   PORTD = 0x0F;
}
