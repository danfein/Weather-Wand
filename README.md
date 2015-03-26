# Weather-Wand

The Weather Wand! 

A small wand that light a LED (or set of LEDs) in response to temperature .

Hot and cold are determined by the standard deviation from the average of the 50 most recent temperature samples.

Hardware: An Arduino pro mini with a TMP36 temp sensor and RGB LED(s).

wx_wand_1 is setup to control a single RGB led

wx_wand_neopixel is setup to control a number of addressable RGB LEDs using the adafruit library
