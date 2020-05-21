/*
 * Inputs ADC Value from Thermistor and outputs Temperature in Celsius
 *  requires: include <math.h>
 * Utilizes the Steinhart-Hart Thermistor Equation:
 *    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]3}
 *    where A = 0.001129148, B = 0.000234125 and C = 8.76741E-08
 *
 * These coefficients seem to work fairly universally, which is a bit of a
 * surprise.
 *
 * Schematic:
 *   [Ground] -- [10k-pad-resistor] -- | -- [thermistor] --[Vcc (5 or 3.3v)]
 *                                               |
 *                                          Analog Pin 0
 *
 * In case it isn't obvious (as it wasn't to me until I thought about it), the analog ports
 * measure the voltage between 0v -> Vcc which for an Arduino is a nominal 5v, but for (say)
 * a JeeNode, is a nominal 3.3v.
 *
 * The resistance calculation uses the ratio of the two resistors, so the voltage
 * specified above is really only required for the debugging that is commented out below
 *
 * Resistance = PadResistor * (1024/ADC -1)  
 *
 * I have used this successfully with some CH Pipe Sensors (https://www.atcsemitec.co.uk/pdfdocs/ch.pdf)
 * which be obtained from https://www.rapidonline.co.uk.
 *
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
  Serial.print("Celsius: ");
  //Serial.print(temp,1);                             // display Celsius
  //temp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  //Serial.print(", Fahrenheit: ");
  //Serial.print(temp,1);                             // display  Fahrenheit
  Serial.println("");
  outputValue = map(temp, 26, 30, 0, 255);            //temperature mainly ranges from 26 to 30 degrees for my room
  Serial.print(outputValue,1);                               
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
