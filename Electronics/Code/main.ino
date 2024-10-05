#define temp 100

#include "tinysnore.h"
class SystemStatus {
  
  public:
    
    uint8_t pin_batt;

    SystemStatus(uint8_t apin_batt);
    SystemStatus();
    int getVCC();
    int8_t getTemperatureInternal();    
};


SystemStatus::SystemStatus(uint8_t apin_batt) : pin_batt(apin_batt) {

}

SystemStatus::SystemStatus() : pin_batt(255) {

}


int SystemStatus::getVCC() {
  
  ADMUX = _BV(MUX5) | _BV(MUX0); 
  delay(2); 
  ADCSRA |= _BV(ADSC); 
  while (bit_is_set(ADCSRA, ADSC));
  uint8_t low = ADCL;
  unsigned int val = (ADCH << 8) | low;
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  low = ADCL;
  val = (ADCH << 8) | low;
  
  return ((long)1024 * 1100) / val;  
}


int8_t SystemStatus::getTemperatureInternal() {

  ADMUX = (1<<REFS0) | (1<<REFS1) | (1<<MUX3); 
  delay(2); 
  ADCSRA |= bit(ADSC);
  while (ADCSRA & bit(ADSC));
  uint8_t low  = ADCL;
  uint8_t high = ADCH;
  ADCSRA |= bit(ADSC);
  while (ADCSRA & bit(ADSC));
  low  = ADCL;
  high = ADCH;
  int a = (high << 8) | low;
  return a - 272; 
}



int tensione;

void setup()// Initiate pin 2 as an output
{
  int carica;
  DDRA = 0b01101111;
  DDRB = 0b00000111; 
  
  tensione =SystemStatus().getVCC();
  if(tensione>4000){
    carica=5;
  }
  else if (tensione>3900){
    carica=4;
  }
  else if (tensione>3800){
    carica=3;
  }  
  else if (tensione>=3600){
    carica=2;
  }  
  else if (tensione>3450){
    carica=1;
  }  
  else{
    carica=0;
  }
  for (int i = 0; i < carica; i++){
    PORTA = 0b00000000;//ledStatus
    delay(200);
    PORTA = 0b01000000;//ledStatus
    delay(200);
  }
  delay(100);
}  

// the loop function runs over and over again forever
void loop() 
{

      if((analogRead(7)<800)&&(tensione>2800))
      {
        for(int i=0; i<2; i++)
        {
            PORTB = 0b00000100;
            PORTA = 0b00000001; //led1
            delay(temp);
            PORTA = 0b00000010;//led2
            delay(temp);
            PORTA = 0b00100000;//led3
            delay(temp);
            PORTA = 0b00000100;//led4
            delay(temp);
            PORTA = 0b00001000;//led4
            delay(temp);
            PORTA = 0b00000000;//led5
            PORTB = 0b00000101;//led5
            delay(temp);
            PORTB = 0b00000100;//led6
            delay(temp);
            PORTB = 0b00000000;//led6
        }
        PORTB = 0b00000000;
        PORTA = 0b01000000;
        delay(100);
        PORTA = 0b00000000;//ledStatus
        delay(100);
        PORTA = 0b01000000;//ledStatus
        delay(100);
        PORTA = 0b00000000;//ledStatus
        delay(100);
    }
    else
    {
      delay(3000);
      tensione =SystemStatus().getVCC();
      PORTA = 0b01000000;//ledStatus
      delay(100);
      PORTA = 0b00000000;//ledStatus
      delay(100);
    }
}
