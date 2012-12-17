/*

  Maker Shed LED Atmospheric Analyzer Kit

  LED Photometer, based on Analog Input
  Original code by Patrick Di Justo. Modified by Eric Weinhoffer 
  
  This code is based on Patrick's LED Photometer project from the book "Atmospheric Monitoring with Arduino".
  You can find his code and a schematic on page 54. I modified it for use with the Sparkfun LCD screen, and left out data storage.
  The LCD must be powered from the 5V pin on the Arduino, with RX from the LCD hooked up to Digital Pin 1 (TX) on the Arduino.
  These pins can be changed on Line 27.

*/

#include <SoftwareSerial.h>
#include <SerLCD.h>
#define txPin2

int redPin = A1;    
int greenPin =A2;
int bluePin=A3;


// A place to store the values coming from the analog port
int sensorValueRed =0;  
int sensorValueGreen =0;  
int sensorValueBlue = 0;

//A place to store the maximum value for each LED
int maxRed = 0;
int maxGreen =0;
int maxBlue = 0;


int record=0;
int reclen = 7;
int addr =0; 


long timeSinceLastSensorHigh = 0;
int dataWritten = 0;

// music section
int notelen =40;
int dlx = notelen *1.33;


void setup() 
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  backlightOn(); 
  // Set the Analog Pins
  // Why are we setting input pins to output? 
  // We're doing this to prevent "leakage" from the pins.
  // Setting the pins to output activates a pullup resistor,
  // which makes it difficult for voltage to come into the Arduino,
  // until we're ready for it.

  Serial.println("Setting up the Analog Pins");
  
  pinMode(redPin, OUTPUT);
  digitalWrite(redPin, LOW);
  pinMode(greenPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  pinMode(bluePin, OUTPUT); 
  digitalWrite(bluePin, LOW);



// Play music to let user know the gadget is ready
  tone(7,294,notelen);
  delay(dlx);
  tone(7,330,notelen);
  delay(dlx);
  tone(7,261,notelen);
  delay(dlx);
  tone(7,131,notelen);
  delay(dlx);
  tone(7,196,notelen);
  delay(dlx);
  delay(3000);  
}


void loop() 
{
  // read the value from the sensor:
  /* 
    Back in setup(), we enabled a pullup resistor on the analog pins, which
    made it difficult for electricity to come into the analog pins.
    Here, we disable the pullup resistor, wait 10ms for the pin to stabilize,
    read the voltage coming into the pin, then reenable the pullup resistor. 
  */
  clearLCD();
  pinMode(redPin, INPUT);
  delay(10);
  selectLineOne();
  Serial.print("Reading red:  ");
  sensorValueRed= analogRead(redPin);
  pinMode(redPin, OUTPUT);  
  Serial.print(sensorValueRed,DEC);
  delay(3000);
  
  clearLCD();
  pinMode(greenPin, INPUT);  
  delay(10);
  selectLineOne();
  Serial.print("Reading grn:  ");
  sensorValueGreen = analogRead(greenPin);
  pinMode(greenPin, OUTPUT);
  Serial.print(sensorValueGreen,DEC);
  delay(3000);
  
  clearLCD();
  pinMode(bluePin, INPUT);
  delay(10);
  selectLineOne();
  Serial.print("Reading blue: ");
  sensorValueBlue = analogRead(bluePin);
  pinMode(bluePin, OUTPUT);
  Serial.print(sensorValueBlue,DEC);
  delay(3000);

  clearLCD();
  Serial.print("Comparing values...");
  delay(5000);

  // Here we compare each sensor to its maximum value.
  // If any of the sensors has reached a new peak, sound a tone
  if(    (sensorValueRed>maxRed) 
      || (sensorValueGreen>maxGreen) 
      || (sensorValueBlue>maxBlue))
    {
    tone(7,maxRed+maxGreen+maxBlue,500);
    timeSinceLastSensorHigh = millis();
    }

  // Here we reset the old maximum value with a new one, if necessary
  if(sensorValueRed>maxRed) maxRed = sensorValueRed;
  if(sensorValueGreen>maxGreen) maxGreen = sensorValueGreen;
  if(sensorValueBlue>maxBlue) maxBlue = sensorValueBlue;

  // Display the sensor values on the LCD screen
}


void selectLineOne()
{
   Serial.write(0xFE);   // command flag
   Serial.write(128);    // position
   delay(10);
}

void selectLineTwo()
{
   Serial.write(0xFE);   // command flag
   Serial.write(192);    // position
   delay(10);
}

void clearLCD()
{
   Serial.write(0xFE);   // command flag
   Serial.write(0x01);   // clear command.
   delay(10);
}

void backlightOn()
{  //turns on the backlight
   Serial.write(0x7C);   // command flag for backlight stuff
   Serial.write(140);    // brightness level. (140 = 40%, 150 = 73%, 157 = 100%) 
   delay(10);
}

void backlightOff()
{  //turns off the backlight
   Serial.write(0x7C);   // command flag for backlight stuff
   Serial.write(128);     // light level for off.
   delay(10);
}

