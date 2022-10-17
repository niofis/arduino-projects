/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Adafruit_BMP280.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN        2

Adafruit_BMP280 bmp; // I2C

Adafruit_NeoPixel pixels(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("ESP32-C3 Temp Sensor"));
  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  pixels.begin();
}

void loop() {
  float celcius = bmp.readTemperature();
  Serial.print(F("Temperature = "));
  Serial.print(celcius);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.println();

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  
  //color code from: https://community.home-assistant.io/t/icon-color-change/148936
  if (celcius < -23) { r = 255; g = 0; b = 255; }
  else if(celcius < -18) { r = 139; g = 0; b = 139; }
  else if(celcius < -12) { r = 128; g = 0; b = 128; }
  else if(celcius < -7) { r = 0; g = 0; b = 255; }
  else if(celcius < 4) { r = 0; g = 128; b = 0; }
  else if(celcius < 10) { r = 144; g = 238; b = 144; }
  else if(celcius < 16) { r = 255; g = 255; b = 0; }
  else if(celcius < 21) { r = 255; g = 165; b = 0; }
  else if(celcius < 27) { r = 255; g = 69; b = 0; }
  else if(celcius < 32) { r = 255; g = 0; b = 0; }
  else { r = 139; g = 0; b = 0; }


  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show(); 

  delay(2000);
}
