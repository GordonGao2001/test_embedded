#include "mbed.h"
#include "board_freedom.h"
#include "adc.h"
#include "oled_ssd1322.h"
#include <cstdint>
#include <cstdio>


#define MESSAGE_MAX_SIZE 50


// main() runs in its own thread in the OS
int main()
{
    // Preamble
    board_init();
    u8g2_ClearBuffer(&oled);
    u8g2_SetFont(&oled, u8g2_font_6x12_mr);
    u8g2_SendBuffer(&oled);

    PwmOut heater_power(PTC2);
    heater_power = 0;


    // Ready a single reusable buffer for writing text to.
    char message[MESSAGE_MAX_SIZE + 1];
    message[MESSAGE_MAX_SIZE] = '\0';

    DigitalOut redLed(PTB2);
    DigitalOut greenLed(PTB3);

    redLed = 0;
    greenLed = 0;


    while (true)
    {
        // Read analog and convert
        uint16_t analog_in_value = adc_read(0);
        
        float voltage = (analog_in_value  / 65535.0) * 3.0;
        float temperature = (voltage * 1000 - 400) / 19.5;
        if(analog_in_value < 0.01 || analog_in_value > 3.0)
        {
            snprintf(message, MESSAGE_MAX_SIZE, "Missing Analog input value, shutting down.");
        }
        else{
            if(temperature - 30 < 0.001)
            {
                //turn on heater
                redLed = 1;
                heater_power = 1;
                greenLed = 0;
            }
            else if(temperature - 35 > 0.001)
            {
                //turn off heater
                redLed = 0;
                greenLed = 0;
                heater_power = 0;
                snprintf(message, MESSAGE_MAX_SIZE, "Overheated, sleeping for 10 seconds");
                ThisThread::sleep_for(10000ms);
            }
            else
            {
                greenLed = 1;
                redLed =1;
                heater_power =1;
            }
        
        // Construct message
        snprintf(message, MESSAGE_MAX_SIZE, "Value is %-5d, temperature is %5.02f", analog_in_value , temperature);


        // Clear screen and write a message.
        u8g2_ClearBuffer(&oled);
        u8g2_DrawUTF8(&oled, 10, 10, message);
        u8g2_SendBuffer(&oled);
        
        // Also write the message over USB to the serial monitor
        printf("%s\n", message);

        }
        // Wait a bit
        ThisThread::sleep_for(100ms);
    }
}
