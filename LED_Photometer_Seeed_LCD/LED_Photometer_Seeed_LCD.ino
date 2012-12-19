/*

  Maker Shed LED Atmospheric Analyzer Kit

  LED Photometer, based on Analog Input
  Original code by Patrick Di Justo. Modified by Eric Weinhoffer 
  
  This code is based on Patrick's LED Photometer project from the book "Atmospheric Monitoring with Arduino".
  You can find his code and a schematic on page 54. I modified it for use with the Seeed LCD screen, and left out data storage.
  Patrick's code is much more comprehensive; this test code is meant to get you up and running with the Seeed LCD.
  
  The LCD must be powered from the 5V pin on the Arduino, with TX hooked up to Digital Pin 3 and RX hooked up to Digital Pin 5.
  These pins can be changed on Line 29.

*/

#include <SerialLCD.h>
#if ARDUINO < 100
#include <NewSoftSerial.h> 
#else
#include <SoftwareSerial.h>
#endif


int redPin = A1;    
int greenPin = A2;
int bluePin= A3;

SerialLCD slcd(3,5); // Assign pins the LCD is connected to: slcd(TX,RX)

// A place to store the values coming from the analog port
int sensorValueRed = 0;  
int sensorValueGreen = 0;  
int sensorValueBlue = 0;

//A place to store the maximum value for each LED
int maxRed = 0;
int maxGreen = 0;
int maxBlue = 0;


int record= 0;
int reclen = 7;
int addr = 0; 


long timeSinceLastSensorHigh = 0;
int dataWritten = 0;

// music section
int notelen = 40;
int dlx = notelen * 1.33;


void setup() 
{
  Serial.begin(9600); // initialize serial communications at 9600 bps
  slcd.begin(); // initialize the LCD
  
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
  slcd.clear();
  pinMode(redPin, INPUT);
  delay(10);
  slcd.setCursor(0, 0);
  slcd.print("Reading red:  ");
  sensorValueRed = analogRead(redPin);
  pinMode(redPin, OUTPUT);  
  slcd.print(sensorValueRed,DEC);
  delay(3000);
  
  slcd.clear();
  pinMode(greenPin, INPUT);  
  delay(10);
  slcd.setCursor(0, 0);
  slcd.print("Reading grn:  ");
  sensorValueGreen = analogRead(greenPin);
  pinMode(greenPin, OUTPUT);
  slcd.print(sensorValueGreen,DEC);
  delay(3000);
  
  slcd.clear();
  pinMode(bluePin, INPUT);
  delay(10);
  slcd.setCursor(0, 0);
  slcd.print("Reading blue: ");
  sensorValueBlue = analogRead(bluePin);
  pinMode(bluePin, OUTPUT);
  slcd.print(sensorValueBlue,DEC);
  delay(3000);

  slcd.clear();
  slcd.print("Comparing values...");
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
}
