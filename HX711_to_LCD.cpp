#include "mbed.h"
#include "HX711.h"
#include "uLCD_4DGL.h"
#include <cmath>

uLCD_4DGL uLCD(p9, p10, p11); //tx, rx, pin 
HX711 scale(p15, p16); // p16 - clk, p15 - dat

//Define the push buttons
DigitalIn pb1(p20); // 1
DigitalIn pb2(p22); // 1
DigitalIn pb3(p21); // 1

//AnalogIn scaleRaw(p18); leaving this out too 

float calibration_factor = -190500; // This is a mechanically determined calibration factor. Dependent on our personal circuit schemaric.
int averageSamples = 100;
int state;
enum buttonstate {INIT = 0, BUTTON_PRESSED_ONCE = 1, BUTTON_PRESSED_TWICE =2}; // this creates an ON and OFF button for the device.

int main(void)
{
    uLCD.background_color(BLUE);
    uLCD.textbackground_color(BLUE);
    uLCD.color(WHITE);
    uLCD.cls();
    
    
    //Turn the buttons on
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    
    uLCD.printf("\nWelcome to the\n");
    uLCD.printf("HX711 Calibrator\n\n");
    uLCD.printf("Remove all weight!\n\n");
    uLCD.printf("Place known weight.\n\n");
    uLCD.printf("Start: B1\n");
    uLCD.printf("B2:(+)\nB3:(-)\n\n");
    wait(10);
    uLCD.cls();
    
      
    scale.setScale(0);
    scale.setScale(0); //Reset the scale to 0 should be .tare
    
    //long zero_factor = scale.averageValue(averageSamples); //Get a baseline reading
    //uLCD.printf("0-Factor: %.4f\r\n" , zero_factor); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    
   
    state = INIT;
    
    uLCD.printf("\nPush B #1 to begin.\n\n\n\n");
    
	int i;
    i = 0;
    
	while (i <= 1)   
    {
        //check if button was pushed, save variable (state) --> press button again to exit this while loop enum (state of button not pressed button pressed once button pressed twice)

        if(pb1 == 0)
        {
            i++;
			state++;
            uLCD.cls();
        }
        while(state == BUTTON_PRESSED_ONCE)
        {
            scale.setScale(calibration_factor); //Adjust to this calibration factor
            float weight = scale.getGram();
            uLCD.printf("\nC.F.: %.2f\n\n", calibration_factor);

            uLCD.printf("\nWeight: %.2f (lbs)\n\n", abs(weight)/453.59237);
            uLCD.printf("Prcnt Used: %.2f %%\n\n\n", ((abs(weight)/10000)*100));
            

           int holder;
           holder = 6;

           if(holder == 6) 
            {
                
                if(pb2 == 0)
                {
                   calibration_factor += 500;
                }
                else if(pb3 == 0)
                {
                    calibration_factor -= 500;
                }
                    
                     
            }
		 if(pb1 == 0)
        {
            i++;
			state++;
            uLCD.cls();
        }
       wait(2);
       uLCD.cls();

        }
    }
	uLCD.cls();
	uLCD.printf("\n\nProgram complete.\n\n\nThank You.");
}
