
/*
The Weather Wand! 

A small wand that lights Neo Pixels (addressable RGB LEDs) at the tip in response to temperature .

Hot and cold are determined by the standard deviation from the average of the 50 most recent temperature samples.

Hardware: An Arduino pro mini with a TMP36 temp sensor and an common cathode RGB LED.

By: Dan Fein with great super math help from John Celenza.

NeoPixel library from Adafruit

*/


//________________Setup________________________

int DEBUG = 1;      // DEBUG counter; if set to 1, will write values back via serial

// NeoPixel setup
#include <Adafruit_NeoPixel.h>
#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//Temp sensor stuff
int sensorPin = 0; //TMP36
float tempF;
float tempC;

//delay var
int del = 80; 

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
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(50); // Turn down for what 0-255
        
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
     
     // converting that reading to voltage, for 3.3v arduino use 3.3, for 5v use 5
     float voltage = reading * 3.3;
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
           colorWipe(strip.Color(255, 0, 255), 25);
        } else if (diff < -2.0) {
           colorWipe(strip.Color(0, 100, 255), 25);
        } else if (diff < -1.5) {
            colorWipe(strip.Color(0, 157, 200), 25);
        } else if (diff < -1.0) {
            colorWipe(strip.Color(150, 206, 150), 25);
        } else if (diff < -0.5) {
            colorWipe(strip.Color(0, 230, 50), 25);
        } else if (diff < 0.0) {      // 0 = Avg
            colorWipe(strip.Color(0, 255, 1), 25);
        } else if (diff < 0.5) {
            colorWipe(strip.Color(50, 230, 0), 25);
        } else if (diff< 1.0) {
            colorWipe(strip.Color(200, 200, 0), 25);
        } else if (diff < 1.5) {
            colorWipe(strip.Color(255, 100, 0), 25);
        } else if (diff < 2.0) {
            colorWipe(strip.Color(255, 50, 0), 25);
        } else {
            colorWipe(strip.Color(255, 0, 0), 25); //max out color
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
              Serial.println("X----0++++X");
          }
        }
   }   
 

//Send the RGB value to the color pins    
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
