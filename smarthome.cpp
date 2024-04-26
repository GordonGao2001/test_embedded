#include "mbed.h"
#include "board_freedom.h"
#include <cstdint>
#include "adc.h"
 
  //smart-home
// main() runs in its own thread in the OS
int main()
{
    board_init();


    DigitalOut Garden_Lamp(PTB18);
    DigitalOut Garage_Door (PTB11);
    DigitalIn  Car_Sensor(PTE25);
    float voltage;
    // unsigned int pwm_min=580;


    
    pwm0.write (0.5);
    pwm0.period_ms(10);
    pwm1.write (0.5);
    pwm1.period_ms(10);  
    pwm2.write (0.5);
    pwm2.period_ms(10);
    Garden_Lamp = 0;


    while (true)
    {
        uint16_t analog_in_lampoutside = adc_read(1);
        voltage = analog_in_lampoutside* 3 / 65535.0 ;
        printf("volt is  %f \n\t", voltage);   
        if(voltage<0.5){
            Garden_Lamp = 1;
        }else{Garden_Lamp=0;}
        if (Car_Sensor == 0) {
            ThisThread::sleep_for(1s);
            Garage_Door = 1;
            ThisThread::sleep_for(5s);
            Garage_Door = 0; 
        }
    }


   
}
