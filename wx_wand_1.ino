
/*
The Weather Wand! 

A small wand that lights up at the tip based on if it is hot or cold.

Hot and cold are determined by the standard deviation from the average of the 50 most recent temperature samples.

Hardware: An Arduino pro mini with a TMP36 temp sensor and an common cathode RGB LED.

By: Dan Fein with great super math help from John Celenza.


*/


//________________Setup________________________

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

//delay var
int del = 500; 

//statistics
int max_samples = 50;
int temperature_samples[50];
int n_temperature_samples = 0;
int i_temperature_samples = 0;
float stddev_temperature = 5;
float avg_temperature = 50;
float tempF_recent = 0;

//________________Run Time________________________

void setup()
  {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    if (DEBUG) {         // If we want to see values for debugging...
    Serial.begin(9600);  // ...set up the serial ouput 
    Serial.println("Starting");
    }
  }
  
void loop()
  {
    getTemp();
    
    temperature_samples[i_temperature_samples] = tempF;
    i_temperature_samples++;
    if (n_temperature_samples < max_samples) {
          n_temperature_samples++;
    }
    if (i_temperature_samples >= max_samples) {
       i_temperature_samples = 0;
    }    
  
    computeStdDev();
   
    tempColor();

    delay(del);
  }
  
// ---------------- Function Fun --------------------
  
   
void getTemp()
    {
     //getting the voltage reading from the temperature sensor
     int reading = analogRead(sensorPin);  
     
     // converting that reading to voltage, for 3.3v arduino use 3.3
     float voltage = reading * 5.0;
     voltage /= 1024.0; 
    
     // now kick out the temperature
     tempC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
                                      //to degrees ((voltage - 500mV) times 100)
     
     // Convert to Fahrenheit
     tempF = (tempC * 9.0 / 5.0) + 32.0;
     
    if (DEBUG)   // If we want to print it all to console...
      {
      Serial.println("++++++++++++++");
      Serial.print(voltage); Serial.println(" volts");
      Serial.print(tempC); Serial.println(" degrees C");
      Serial.print(tempF); Serial.println(" degrees F");
      Serial.print(tempF_recent); Serial.println(" degrees F (recent)");

      Serial.println("--------------");
      }
      
    }

void computeStdDev() {
  int i;
  avg_temperature = 0;
  for (i = 0; i < n_temperature_samples; i++){
    avg_temperature = avg_temperature + temperature_samples[i];
  }
  avg_temperature = avg_temperature / n_temperature_samples;
  
  stddev_temperature = 0;
  for (i = 0; i < n_temperature_samples; i++){
    stddev_temperature = stddev_temperature + (temperature_samples[i] - avg_temperature) * (temperature_samples[i] - avg_temperature);
  }
  
  stddev_temperature = stddev_temperature / n_temperature_samples;
  stddev_temperature = sqrt(stddev_temperature);
  
  int start_i = i_temperature_samples - 5;
  if (start_i < 0) {
    start_i = 0;
  }
  
  if (i_temperature_samples - start_i > 0){
  
    tempF_recent = 0;
    for (i = start_i; i < i_temperature_samples; i++) {
      tempF_recent = tempF_recent + temperature_samples[i];
    }
    tempF_recent /= (i_temperature_samples - start_i);
 
  } else {
    tempF_recent = tempF;
  }
  
    if (DEBUG)   // If we want to print it all to console...
    {          
     Serial.print("n_temperature_samples: ");
     Serial.println(n_temperature_samples);
     Serial.print("avg_temperature: ");
     Serial.println(avg_temperature);
     Serial.print("temperature: ");
     Serial.println(tempF);
     Serial.print("i: ");
     Serial.println(i_temperature_samples);
     Serial.print("stddev_temperature: ");
     Serial.println(stddev_temperature);
    }  
}

//Set the RGB color based on standard deviation distance from average temp 
void tempColor()
  {
    float diff = (tempF_recent - avg_temperature) / stddev_temperature;
       
        if(diff < -2.5) {
            setColor(0, 0, 255);
        } else if (diff < -2.0) {
            setColor(0, 100, 255);
        } else if (diff < -1.5) {
            setColor(0, 157, 255);
        } else if (diff < -1.0) {
            setColor(150, 206, 150);
        } else if (diff < -0.5) {
            setColor(0, 254, 0);
        } else if (diff < 0.0) {
            setColor(0, 255, 1);
        } else if (diff < 0.5) {
            setColor(150, 255, 0);
        } else if (diff< 1.0) {
            setColor(200, 200, 0);
        } else if (diff < 1.5) {
            setColor(255, 150, 0);
        } else if (diff < 2.0) {
            setColor(255, 100, 0);
        } else {
            setColor(255, 0, 255); //out of scope color
        }
       
       if (DEBUG)   // Standard deviation visualizer
        {
         Serial.print("diff: ");  
         Serial.println(diff);
         
          if(diff < -2.5) {
              Serial.println("X----0+++++");
          } else if (diff < -2.0) {
              Serial.println("-X---0+++++");
          } else if (diff < -1.5) {
              Serial.println("--X--0+++++");
          } else if (diff < -1.0) {
              Serial.println("---X-0+++++");
          } else if (diff < -0.5) {
              Serial.println("----X0+++++");
          } else if (diff < 0.0) {
              Serial.println("-----0+++++");
          } else if (diff < 0.5) {
              Serial.println("-----0X++++");
          } else if (diff< 1.0) {
              Serial.println("-----0+X+++");
          } else if (diff < 1.5) {
              Serial.println("-----0++X++");
          } else if (diff < 2.0) {
              Serial.println("-----0+++X+");
          } else {
              setColor(255, 0, 255); //out of scope color
              Serial.println("X----0++++X");
          }
        }
   }   
 
//Send the RGB value to the color pins
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
