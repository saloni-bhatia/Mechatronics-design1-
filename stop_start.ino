#define F_CU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
/*defined the macros to be used throughout the code */
#define set(A,B) A|=(1<<B)
#define clr(A,B) A &=~(1<<B)
#define toggle(A,B) A^=(1<<B)

volatile int sensor[4]={0,0,0,0};


int setup_int(){
   /*setting up required pins for the motor controls */ 
  set(DDRB, 7);
  set(DDRD,0); 
  /*Setting up the timers
     Set Timer A and B  */
  TCCR0B |= (1 << 2);
   /* Clear on Compare match
      Clear OCOA and OCOB on Compare Match
      Set fast PWM*/
  TCCR0A |= (1 << 7) | (1 << 5) | (1 << 1) | (1 << 0);
  ADC_set();
}
int ADC_set(){
  ADMUX |= (1<<6)|(1<<5)|(1<<2)|(1<<1) ;   //5V reference, ADC6
  ADCSRA |= (1<<7)|(1<<2)|(1<<3)|(1<<1)|1;    //Configuration - Auto-trigger off
  ADCSRB = 0;
  ADCSRA |= (1<<6); 
  //Start first conversions
}

ISR(ADC_vect){
  uint8_t mux = ADMUX & 0b00011111;
  uint8_t carry = ADMUX & 0b11100000;
  switch(mux){
    
    case 0b0110:    
    ADMUX= carry|0b0111;
    sensor[0] = ADCH;
    break; 
    case 0b0111:
    ADMUX= carry|0b0001;
    sensor[1] = ADCH;
    break;
    case 0b0001:
    ADMUX = carry|0b0100;
    sensor[3]=ADCH;
    break;
    case 0b0100:
    ADMUX = carry|0b0110;
    sensor[4]=ADCH;
    break;    
    
 }
set (ADCSRA,6);
}



/*sensor[0] - motor0 - OCROB senor[1] - motor 1 - OCROA*/
int straight() 
{
   set(DDRB,3);
   set(PORTB,3);

   /*set the duty cycle - which sets the speed of the motor*/
  OCR0A = 170; /*m0*/
  OCR0B = 170;/*m1*/
  
}
int right()
{
 clr(DDRB,3);
 clr(PORTB,3);
  OCR0A = 100; /*m0*/
  OCR0B = 10;/*m1*/
}
int left()
{
  clr(DDRB,3);
   clr(PORTB,3);
 
  OCR0A = 100; /*m0*/
  OCR0B = 70;/*m1*/
}
void stop_moving()
{
  clr(DDRB,3);
 clr(PORTB,3);
  OCR0A=0;
  OCR0B=0;
}


int main() {
  
  setup_int();
  sei();
  
  while (1) /*loops lets the motors keep running till switched off manually*/
  
{
     if(sensor[0]<200)
      {
       
        right();
        
      }
      else if (sensor[1]<200)
      {
        
        left();
        clr(DDRB,3);
        clr(PORTB,3);
      }
    
      else 
        {
          
          
          if((sensor[3]<200))
          {
            _delay_ms(100);
             stop_moving();
            _delay_ms(2000);
            if(sensor[0]<200)
          { 
       
            right();
            
        }
        else if (sensor[1]<200)
        {
          
          left();
         
        }
           
              
           
         
         } 
            straight();
           
        }
     
        
 
}
}
  
