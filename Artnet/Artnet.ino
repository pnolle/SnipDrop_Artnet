#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>
#include "secrets.h"  // local variables
#include "dummypixels.h"

// LED Strip
const int numLeds = 300; // Change if your setup has more or less LED's
const int numberOfChannels = numLeds * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
#define DATA_PIN 12 //The data pin that the WS2812 strips are connected to.
CRGB leds[numLeds];

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0;

bool sendFrame = 1;
int previousDataLength = 0;
int frameNo = 0;

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;

  WiFi.begin(ssid, password);
  Serial.println("### ARTNET ESP32 ###");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  Serial.println("_");
  Serial.println(printf("onDmxFrame #%u uni%u l%u seq%u", frameNo, universe, length, sequence));
  sendFrame = 1;
  // set brightness of the whole strip 
  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
  }
  // read universe and put into the right part of the display buffer
  // using length/3 because 3 values define r/g/b of one pixel
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);

    if (led < numPx)
    {
      int16_t thisCount = 0;
      const int16_t *thisRegion;

      switch (led)
      {
        // row 1
        case 10:
            thisCount = len_p11_1;
            thisRegion = p11_1;
            break;
        case 11:
            thisCount = len_p12_1;
            thisRegion = p12_1;
            break;
        case 12:
            thisCount = len_p13_1;
            thisRegion = p13_1;
            break;
        case 13:
            thisCount = len_p14_1;
            thisRegion = p14_1;
            break;
        case 14:
            thisCount = len_p15_1;
            thisRegion = p15_1;
            break;

        // row 2
        case 23:
            thisCount = len_p14_2;
            thisRegion = p14_2;
            break;
        case 24:
            thisCount = len_p15_2;
            thisRegion = p15_2;
            break;
        case 25:
            thisCount = len_p16_2;
            thisRegion = p16_2;
            break;

        // row 3
        case 35:
            thisCount = len_p16_3;
            thisRegion = p16_3;
            break;
        case 36:
            thisCount = len_p17_3;
            thisRegion = p17_3;
            break;
        case 37:
            thisCount = len_p18_3;
            thisRegion = p18_3;
            break;

        // row 4
        case 46:
            thisCount = len_p17_4;
            thisRegion = p17_4;
            break;
        case 47:
            thisCount = len_p18_4;
            thisRegion = p18_4;
            break;
        case 48:
            thisCount = len_p19_4;
            thisRegion = p19_4;
            break;

        // row 5
        case 58:
            thisCount = len_p19_5;
            thisRegion = p19_5;
            break;
        case 59:
            thisCount = len_p20_5;
            thisRegion = p20_5;
            break;
        case 60:
            thisCount = len_p21_5;
            thisRegion = p21_5;
            break;

        // row 6
        case 69:
            thisCount = len_p20_6;
            thisRegion = p20_6;
            break;
        case 70:
            thisCount = len_p21_6;
            thisRegion = p21_6;
            break;
      }

      for(int l=0; l<thisCount; l++) {
        leds[thisRegion[l]] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      }
    }
  }
  previousDataLength = length;     
  FastLED.show();
  frameNo ++;
}

void setup()
{
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin();
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, numLeds);

  // onDmxFrame will execute every time a packet is received by the ESP32
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  artnet.read();
}
