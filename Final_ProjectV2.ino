/*
 * Motion Sensing Alarm
 * By Sarib Kashif
 * May 23, 2020
 * Detects motion around photoresistor and activates an alarm if there is a disruption in brightness
 */

int sensorPin = A1; //photoresistor pin
int sensorValue = 0; //variable for photoresistors input

const int buttonPin = 2; //button pin
int buttonState = 0; //keep track of button click

const int POTENTIOMETER_PIN = A0; // potentiometer pin
const int BUZZER_PIN        = 3; // buzzer pin

int redPin = 11; 
int greenPin = 10;
int bluePin = 9;

int previous = LOW;    // button switching referenced from https://www.arduino.cc/en/tutorial/switch
long time = 0;         // the last time the output pin was toggled
long debounce = 500;   // the debounce time, increase if the output flickers
int currentState = LOW; //state of the alarm is low because it has not been activated

void setup() {
  pinMode(buttonPin, INPUT); //declaring each component as input or output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200); //used to adjust photoresistor values for range of alarm
}

void loop() {
  // read the value from the photoresistor
  sensorValue = analogRead(sensorPin);
  // read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);
  // read the value from the potentiometer
  int analogValue = analogRead(POTENTIOMETER_PIN);
  Serial.print(sensorValue);
  Serial.println("");
  if (buttonState == HIGH && previous == LOW && millis() - time > debounce) { //if the button is clicked and it has been 0.5 (debounce) seconds 
    if (currentState == HIGH) //change the state of the alarm to the opposite of what it initially was
      currentState = LOW;
    else
      currentState = HIGH;
    time = millis();  //update the time
  }
  
  if (currentState == HIGH) {      // if button clicked
    activateAlarm(sensorValue, analogValue);  //call function
  } else {
    setColor(0,0,0); //turn led off
    noTone(BUZZER_PIN); //turn buzzer off
  }
}

void activateAlarm(int range, int volume)
{
  if (range < 280) //if brightness is within 280 -- means motion has been detected
  {
    setColor(255, 0, 0); //make rgb red
    tone(BUZZER_PIN, 1000); //turn alarm on
    analogWrite(BUZZER_PIN,volume); //use potentiometer to get the volume
  } else {
    setColor(0, 255, 0); //if no motion, just have green light to show that alarm is on
    noTone(BUZZER_PIN); //turn alarm off
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red); //assign colors
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
