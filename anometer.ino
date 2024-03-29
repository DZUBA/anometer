#include <Ethernet.h>


// include the SD library:
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

//Setup Variables

const int sensorPin = A0; //Defines the pin that the anemometer output is connected tou
int sensorValue = 0; //Variable stores the value direct from the analog pin
float sensorVoltage = 0; //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float windSpeed = 0; // Wind speed in meters per second (m/s)

float voltageConversionConstant = .004882814; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 1000; //Delay between sensor readings, measured in milliseconds (ms)

//Anemometer Technical Variables
//The following variables correspond to the anemometer sold by Adafruit, but could be modified to fit other anemometers.

float voltageMin = .4; // Mininum output voltage from anemometer in mV.
float windSpeedMin = 0; // Wind speed in meters/sec corresponding to minimum voltage

float voltageMax = 2.0; // Maximum output voltage from anemometer in mV.
float windSpeedMax = 32; // Wind speed in meters/sec corresponding to maximum voltage

File myFile;

const int pinCS = 4;

void setup()
{
  Serial.begin(9600);  //Start the serial connection

  pinMode(pinCS, OUTPUT);
}

void loop()
{
  tmElements_t tm;

  sensorValue = analogRead(sensorPin); //Get a value between 0 and 1023 from the analog pin connected to the anemometer

  sensorVoltage = sensorValue * voltageConversionConstant; //Convert sensor value to actual voltage

  //Convert voltage value to wind speed using range of max and min voltages and wind speed for the anemometer
  if (sensorVoltage <= voltageMin){
   windSpeed = 0; //Check if voltage is below minimum value. If so, set wind speed to zero.
  }else {
    windSpeed = (sensorVoltage - voltageMin)*windSpeedMax/(voltageMax - voltageMin); //For voltages above minimum value, use the linear relationship to calculate wind speed.
  }

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    if (RTC.read(tm)) {
      print2digits(tm.Hour);
      myFile.write(':');
      print2digits(tm.Minute);
      myFile.write(':');
      print2digits(tm.Second);
      myFile.print(tm.Day);
      myFile.write('/');
      myFile.print(tm.Month);
      myFile.write('/');
      myFile.print(tmYearToCalendar(tm.Year));
    }
    myFile.print("Voltage: ");
    myFile.print(sensorVoltage);
    myFile.print("Wind speed: ");
    myFile.println(windSpeed);
    myFile.close(); // close the file
  }
  // if the file didn't open, print an error:
  else {
    myFile.println("error opening test.txt");
  }
 delay(300);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    myFile.write('0');
  }
  myFile.print(number);
}
