
//Vars
int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial

// Init the Pins used for PWM
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

//Temp sensor stuff
int sensorPin = 0; //TMP36
float tempF;
float tempC;

// Vars for the color output
int redVal = 0;
int grnVal = 0;
int bluVal = 0;

// Color arrays - 0-100 (conversion to 0-255 happens in setColor2)
int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };
// etc.

int del = 500; //delay var

void setup()
  {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    if (DEBUG) {           // If we want to see values for debugging...
    Serial.begin(9600);  // ...set up the serial ouput 
    Serial.println("Starting");
    }
  }
  
void loop()
  {
    getTemp();
    tempColor();
    delay(del);
  }
  
// ---------------- Function Fun --------------------
  
void setColor(int r, int g, int b) //0-255 input
    {
      // plug it in
      int redVal = r;
      int grnVal = g;
      int bluVal = b;
      
      analogWrite(redPin, redVal);   // Write current values to LED pins
      analogWrite(greenPin, grnVal);      
      analogWrite(bluePin, bluVal);
      
      if (DEBUG)   // If we want to print it all to console...
        {
        Serial.print(redVal); Serial.print(" R ");
        Serial.print(grnVal); Serial.print(" G ");
        Serial.print(bluVal); Serial.println(" B ");
        Serial.println("--------------");
        }
    }

void setColor2(int color[3]) //0-100 input
    {
      // Convert to 0-255
      int redVal = (color[0] * 255) / 100;
      int grnVal = (color[1] * 255) / 100;
      int bluVal = (color[2] * 255) / 100;
      
      analogWrite(redPin, redVal);   // Write current values to LED pins
      analogWrite(greenPin, grnVal);      
      analogWrite(bluePin, bluVal);
      
      if (DEBUG)   // If we want to print it all to console...
        {
        Serial.print(redVal); Serial.print(" R ");
        Serial.print(grnVal); Serial.print(" G ");
        Serial.print(bluVal); Serial.println(" B ");
        Serial.println("--------------");
        }
    }

    
void getTemp()
    {
     //getting the voltage reading from the temperature sensor
     int reading = analogRead(sensorPin);  
     
     // converting that reading to voltage, for 3.3v arduino use 3.3
     float voltage = reading * 5.0;
     voltage /= 1024.0; 
    
     // now kick out the temperature
     tempC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                             //to degrees ((voltage - 500mV) times 100)
     
     // now convert to Fahrenheit
     tempF = (tempC * 9.0 / 5.0) + 32.0;
     
    if (DEBUG)   // If we want to print it all to console...
      {
      Serial.println("++++++++++++++");
      Serial.print(voltage); Serial.println(" volts");
      Serial.print(tempC); Serial.println(" degrees C");
      Serial.print(tempF); Serial.println(" degrees F");
      Serial.println("--------------");
      }
    }

void tempColor()
  {
        if(tempF < 30) {
            setColor(0, 164, 255);
        } else if (tempF < 40) {
            setColor(255, 0, 255);
        } else if (tempF < 45) {
            setColor(255, 127, 0);
        } else if (tempF < 50) {
            setColor(255, 206, 0);
        } else if (tempF < 55) {
            setColor(255, 254, 0);
        } else if (tempF < 60) {
            setColor(230, 255, 1);
        } else if (tempF < 65) {
            setColor(203, 255, 0);
        } else if (tempF < 70) {
            setColor(174, 255, 0);
        } else if (tempF < 75) {
            setColor(153, 255, 0);
        } else if (tempF < 80) {
            setColor(127, 255, 0);
        } else {
            setColor(7, 255, 0); //out of scope color
        }
   }   
