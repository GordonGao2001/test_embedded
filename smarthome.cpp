#include "mbed.h"
#include "board_freedom.h"
#include <cstdint>
#include "adc.h"
#include "oled_ssd1322.h"
#define MESSAGE_MAX_SIZE 50


void printmsg(char* message)
{
    // Clear screen and write a message.
    u8g2_ClearBuffer(&oled);
    u8g2_DrawUTF8(&oled, 10, 10, message);
    u8g2_SendBuffer(&oled);
}

// main() runs in its own thread in the OS
int main()
{
    board_init();


    DigitalOut Garden_Lamp(PTB18);
    DigitalOut Garage_Door(PTB11);
    DigitalIn  Car_Sensor(PTE25);
    DigitalOut Fan (PTC12);
    //check pin***************
    DigitalIn Mux_A(PTA8);
    DigitalIn Mux_B(PTA9);
    float volt_lamp;
    float volt_indoor;
    // unsigned int pwm_min=580;

    DigitalOut redLed(PTB2);
    DigitalOut greenLed(PTB3);

    //oled
    u8g2_ClearBuffer(&oled);
    u8g2_SetFont(&oled, u8g2_font_6x12_mr);
    u8g2_SendBuffer(&oled);

    // Ready a single reusable buffer for writing text to.
    char message[MESSAGE_MAX_SIZE + 1];
    message[MESSAGE_MAX_SIZE] = '\0';

    redLed = 1;
    greenLed = 0;

    while (true)
    {
        //Light sensor
        uint16_t analog_in_lampoutside = adc_read(1);
        volt_lamp = analog_in_lampoutside* 3 / 65535.0 ;
        printf("volt is  %f \n\t", volt_lamp);   
        if(volt_lamp<0.5){
            Garden_Lamp = 1;

            // light acutators
            pwm0.write (0.5);
            pwm0.period_ms(10);
            pwm1.write (0.5);
            pwm1.period_ms(10);  
            pwm2.write (0.5);
            pwm2.period_ms(10);
            //fan & heater
            pwm3.write(0.5); //heater on
            pwm3.period_ms(10);
            Fan = 1;
        }else{
            Garden_Lamp=0;
            // light acutators
            pwm0.write (0);
            pwm0.period_ms(10);
            pwm1.write (0);
            pwm1.period_ms(10);  
            pwm2.write (0);
            pwm2.period_ms(10);
            //fan & heater
            pwm3.write(0); //heater on
            pwm3.period_ms(10);
            Fan = 0;
            }
        //servomotor & hall effect
        if (Car_Sensor == 0) {
            ThisThread::sleep_for(1s);
            Garage_Door = 1;
            redLed = 0;
            greenLed = 1;
            ThisThread::sleep_for(5s);
            Garage_Door = 0; 
        }else{
            // bug, remove to make it right
            // redLed = 1;
            // greenLed = 0;
        }
        //temperature sensor: Indoor temperature sensor 
        uint16_t analog_in_indtemp = adc_read(0);
        volt_indoor = (analog_in_indtemp  / 65535.0) * 3.0;
        float temp_indoor = (volt_indoor * 1000 - 400) / 19.5;




        //print
        snprintf(message, MESSAGE_MAX_SIZE, "lig_sen volt: %5.2f,indoor temp: %5.2f", volt_lamp, temp_indoor);
        printmsg(message);
    }


   
}

