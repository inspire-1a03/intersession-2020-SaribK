/*
 * Thermistor working with RGB LED
 * Sarib Kashif
 * May 21 2020
 * Program changes the color of the rgb led depending on the temperature reading from the thermistor
 * Cold reading means blue, hot reading means red
 */

int redPin = 11; 
int greenPin = 10;
int bluePin = 9;

#include <math.h>

#define ThermistorPIN 0                 // Analog Pin 0

float vcc = 4.91;                       // only used for display purposes, if used
                                        // set to the measured Vcc.
float pad = 9850;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermr = 10000;                   // thermistor nominal resistance

float Thermistor(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=pad*((1024.0 / RawADC) - 1);
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  return Temp;                                      // Return the Temperature
}

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  Serial.begin(115200);
}

void loop() {
  float temp;
  float outputValue = 0;
  temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
  Serial.print(temp,1);
  Serial.print(" Celsius: ");
  //Serial.print(temp,1);                             // display Celsius
  //temp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  //Serial.print(", Fahrenheit: ");
  //Serial.print(temp,1);                             // display  Fahrenheit
  Serial.println("");
  outputValue = map(temp, 26, 30, 0, 255);            //temperature mainly ranges from 26 to 30 degrees for my room                               
  changeColor (outputValue); //call this function to decide the color of the light
}

void changeColor(float mappedTemp)
{                      //chooses color depending on the mapped temperature 
  if (mappedTemp < 80) //lower number means cooler color like blue, higher number is hotter like red
  {
    //blue
    setColor (0, 0, mappedTemp * 3); //multiplied by 3 because the value would be less than 80 otherwise
  }
  else if (mappedTemp > 180)
  {
    //red
    setColor (mappedTemp, 0, 0);
  }
  else
  {
    //yellowish
    setColor (mappedTemp, mappedTemp / 2, 0); 
  }
}

void setColor(int red, int green, int blue) //sets the color of rgb light according to given values for each color
{
  analogWrite(redPin, red); 
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
