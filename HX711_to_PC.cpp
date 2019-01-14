#include "mbed.h"
#include "HX711.h"
#include "uLCD_4DGL.h"
#include <cmath>

//uLCD_4DGL uLCD(p9, p10, p11); tx, rx, pin leaving this out for now
Serial pc(USBTX, USBRX); // tx rx
HX711 scale(p15, p16); // p16 - clk, p15 - dat
DigitalIn pb1(p20);
DigitalIn pb2(p22);
DigitalIn pb3(p21);

//AnalogIn scaleRaw(p18); leaving this out too lol

float calibration_factor = -190500; // I do not know why, but the only program runs with this, don't remove
int averageSamples = 100;

int main(void)
{
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    
    pc.printf("Beginning Program . . .\r\n");
    pc.printf("Load-Cell to HX711 Weight Calibrator\r\n");
    pc.printf("Remove all weight from parallel bars\r\n");
    pc.printf("Place known weight on bars\r\n");
    pc.printf("Press + or a to increase calibration factor\r\n");
    pc.printf("Press - or z to decrease calibration factor\r\n\r\n");
    
      
    scale.setScale(0);
    scale.setScale(0); //Reset the scale to 0 should be .tare
    
    long zero_factor = scale.averageValue(averageSamples); //Get a baseline reading
    pc.printf("Zero factor: %.4f\r\n" , zero_factor); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    
    wait(1);
    //state = BUTTON_NOT_PRESSED
    state = INIT;
    
    pc.printf("Press the Spacebar to begin the program.\r\nAnd press space to end the program.\r\n\r\n");
    
	int mover;
	mover = 0;
	
    while (true && mover == 0)   
    {
		char starter = pc.getchar();
		while(starter == 32 || starter == ' ')
		{
			scale.setScale(calibration_factor); //Adjust to this calibration factor
			float weight = scale.getGram();
			pc.printf("Calibration Factor: %.2f\r\n", calibration_factor);
			//float raw = scaleRaw.read();
			if (weight < 1000)
			{
				pc.printf("Reading: %.2f (g)\r\n", abs(weight));
			}
			else
			{
				pc.printf("Reading: %.2f (kg)\r\n", (abs(weight)/1000));
			}   
			//pc.printf("Reading: %.2f\n", weight); no
			//pc.printf("Raw Value: %.7f\n", raw); no haha
			 
			pc.printf("Reading: %.2f (lbs)\r\n", abs(weight)/453.59237);
			pc.printf("Percentage of Bar Usage: %.2f %%\r\n\r\n\r\n", (abs(weight)/10000)*100);
				
			//int prcnt;
			//prcnt = ( (abs(weight)/10000)*100);
			//wait_ms(10);
			// pc.printf("$%d;", prcnt); >>FIX

			if(pc.readable()) 
			{
				char temp = pc.getc();
				if(temp == '+' || temp == 'a')
				{
				   calibration_factor += 500;
				}
				else if(temp == '-' || temp == 'z')
				{
					calibration_factor -= 500;
				}
			}
			
			char ender = pc.getc();
			if(ender == 32 || ender == ' ')
			{
				mover++;
			}
			
		   wait(2); 
		}
    }
}