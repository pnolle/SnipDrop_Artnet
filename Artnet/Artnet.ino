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
  Serial.println(printf("\nonDmxFrame #%u uni%u l%u seq%u", frameNo, universe, length, sequence));
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

    // half circle line length 134
    // int led = i + (universe - startUniverse) * 134;
    
    //Serial.println(printf("numleds %i %i \n", led, numLeds));
    if (led < numPx)
    {
      // leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

      int16_t thisCount = 0;
      const int16_t *thisRegion;

      switch (led)
      {
        case 0:
            thisCount = len_p11_1;
            thisRegion = p11_1;
            break;
        case 1:
            thisCount = len_p12_1;
            thisRegion = p12_1;
            break;
        case 2:
            thisCount = len_p13_1;
            thisRegion = p13_1;
            break;
        case 3:
            thisCount = len_p14_1;
            thisRegion = p14_1;
            break;
        case 4:
            thisCount = len_p14_2;
            thisRegion = p14_2;
            break;
        case 5:
            thisCount = len_p15_1;
            thisRegion = p15_1;
            break;
        case 6:
            thisCount = len_p15_2;
            thisRegion = p15_2;
            break;
        case 7:
            thisCount = len_p16_2;
            thisRegion = p16_2;
            break;
        case 8:
            thisCount = len_p16_3;
            thisRegion = p16_3;
            break;
        case 9:
            thisCount = len_p17_3;
            thisRegion = p17_3;
            break;
        case 10:
            thisCount = len_p17_4;
            thisRegion = p17_4;
            break;
        case 11:
            thisCount = len_p18_3;
            thisRegion = p18_3;
            break;
        case 12:
            thisCount = len_p18_4;
            thisRegion = p18_4;
            break;
        case 13:
            thisCount = len_p19_4;
            thisRegion = p19_4;
            break;
        case 14:
            thisCount = len_p19_5;
            thisRegion = p19_5;
            break;
        case 15:
            thisCount = len_p20_5;
            thisRegion = p20_5;
            break;
        case 16:
            thisCount = len_p20_6;
            thisRegion = p20_6;
            break;
        case 17:
            thisCount = len_p21_5;
            thisRegion = p21_5;
            break;
        case 18:
            thisCount = len_p21_6;
            thisRegion = p21_6;
            break;
        case 19:
            thisCount = len_p22_6;
            thisRegion = p22_6;
            break;
        case 20:
            thisCount = len_p23_6;
            thisRegion = p23_6;
            break;
        case 21:
            thisCount = len_p24_6;
            thisRegion = p24_6;
            break;
        case 22:
            thisCount = len_p25_6;
            thisRegion = p25_6;
            break;
        case 23:
            thisCount = len_p26_6;
            thisRegion = p26_6;
            break;
        case 24:
            thisCount = len_p27_6;
            thisRegion = p27_6;
            break;
        case 25:
            thisCount = len_p28_5;
            thisRegion = p28_5;
            break;
        case 26:
            thisCount = len_p28_6;
            thisRegion = p28_6;
            break;
        case 27:
            thisCount = len_p29_5;
            thisRegion = p29_5;
            break;
        case 28:
            thisCount = len_p29_6;
            thisRegion = p29_6;
            break;
        case 29:
            thisCount = len_p30_4;
            thisRegion = p30_4;
            break;
        case 30:
            thisCount = len_p30_5;
            thisRegion = p30_5;
            break;
        case 31:
            thisCount = len_p30b_4;
            thisRegion = p30b_4;
            break;
        case 32:
            thisCount = len_p30b_3;
            thisRegion = p30b_3;
            break;
        case 33:
            thisCount = len_p31_4;
            thisRegion = p31_4;
            break;
        case 34:
            thisCount = len_p31_3;
            thisRegion = p31_3;
            break;
        case 35:
            thisCount = len_p31_2;
            thisRegion = p31_2;
            break;
        case 36:
            thisCount = len_p32_2;
            thisRegion = p32_2;
            break;
        case 37:
            thisCount = len_p32_1;
            thisRegion = p32_1;
            break;
        case 38:
            thisCount = len_p32_0;
            thisRegion = p32_0;
            break;
        case 39:
            thisCount = len_p33_1;
            thisRegion = p33_1;
            break;
        case 40:
            thisCount = len_p33_0;
            thisRegion = p33_0;
            break;
        case 41:
            thisCount = len_p33_m1;
            thisRegion = p33_m1;
            break;
        case 42:
            thisCount = len_p33_m2;
            thisRegion = p33_m2;
            break;
        case 43:
            thisCount = len_p33_m3;
            thisRegion = p33_m3;
            break;
      }

      Serial.println(printf("pixel %i | region length %i \n", led, thisCount));

      for(int l=0; l<thisCount; l++) {
//        Serial.println(printf("illuminating led %i %i | i %i | data start: %i \n", thisRegion[l], numLeds, i, i * 3));
        leds[thisRegion[l]] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      }
    }
    else {
      Serial.println(printf("pixel not illuminated %i \n", led));
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
  // Serial.println("loop");
  // we call the read function inside the loop
  artnet.read();
}
