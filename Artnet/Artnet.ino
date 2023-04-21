#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>
#include "secrets.h" // local variables
#include "dummypixels.h"

// LED Strip
const int numLeds = 300;                  // Change if your setup has more or less LED's
const int numberOfChannels = numLeds * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
#define DATA_PIN 12                       // The data pin that the WS2812 strips are connected to.
CRGB leds[numLeds];

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0;

int previousDataLength = 0;
int frameNo = 0;
int testState = 0;
int numberOfStates = 3;
int switchAfterFrames = 100;

// color state
uint8_t colorR = 50;
uint8_t colorG = 100;
uint8_t colorB = 150;


void ledTestFrame()
{
  frameNo++;
  if (frameNo >= switchAfterFrames)
  {
    testState++;
    if (testState>numberOfStates) {
      testState=0;
    }
    frameNo = 0;
  }
  colorR++;
  colorG+=2;
  colorB+=3;
//  Serial.println("_");
//  Serial.println(printf("COLORS %i %i %i", colorR, colorG, colorB));
  switch (testState)
  {
  case 0:
    for (int i = 0; i < numLeds; i++)
    {
      leds[i] = CRGB(colorR, colorG, colorB);
    }
    break;
  case 1:
    blackout();
    for (int i = 0; i < (numLeds / 2)-16; i++)
    {
      leds[i] = CRGB(colorR, colorG, colorB);
    }
    break;
  case 2:
    blackout();
    for (int i = (numLeds / 2)-16; i < numLeds-38; i++)
    {
      leds[i] = CRGB(colorR, colorG, colorB);
    }
    break;
  case 3:
    blackout();
    for (int i = numLeds-38; i < numLeds; i++)
    {
      leds[i] = CRGB(colorR, colorG, colorB);
    }
    break;
  }
  FastLED.show();
}

void blackout() {
    for (int i = 0; i < numLeds; i++)
    {
      leds[i] = CRGB::Black;
    }
}


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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i > 20)
    {
      state = false;
      break;
    }
    i++;
  }
  if (state)
  {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data)
{
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
    Serial.println(printf("onDmxFrame #%u => led #%i uni%u l%u seq%u", frameNo, led, universe, length, sequence));

    // half circle line length 134
    // int led = i + (universe - startUniverse) * 134;

    // Serial.println(printf("numleds %i %i \n", led, numLeds));
    if (led < numPx)
    {
      // leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

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

        // row 7
        // case 19:
        //     thisCount = len_p21_7;
        //     thisRegion = p21_7;
        //     break;
        // case 20:
        //     thisCount = len_p21_8;
        //     thisRegion = p21_8;
        //     break;
        // case 21:
        //     thisCount = len_p21_9;
        //     thisRegion = p21_9;
        //     break;
        // case 22:
        //     thisCount = len_p21_10;
        //     thisRegion = p21_10;
        //     break;
        // case 23:
        //     thisCount = len_p20_11;
        //     thisRegion = p20_11;
        //     break;
        // case 24:
        //     thisCount = len_p20_12;
        //     thisRegion = p20_12;
        //     break;
        // case 25:
        //     thisCount = len_p19_13;
        //     thisRegion = p19_13;
        //     break;
        // case 26:
        //     thisCount = len_p20_13;
        //     thisRegion = p20_13;
        //     break;
        // case 27:
        //     thisCount = len_p19_14;
        //     thisRegion = p19_14;
        //     break;
        // case 28:
        //     thisCount = len_p20_14;
        //     thisRegion = p20_14;
        //     break;
        // case 29:
        //     thisCount = len_p18_15;
        //     thisRegion = p18_15;
        //     break;
        // case 30:
        //     thisCount = len_p19_15;
        //     thisRegion = p19_15;
        //     break;
        // case 31:
        //     thisCount = len_p18_16;
        //     thisRegion = p18_16;
        //     break;
        // case 32:
        //     thisCount = len_p17_16;
        //     thisRegion = p17_16;
        //     break;
        // case 33:
        //     thisCount = len_p18_17;
        //     thisRegion = p18_17;
        //     break;
        // case 34:
        //     thisCount = len_p17_17;
        //     thisRegion = p17_17;
        //     break;
        // case 35:
        //     thisCount = len_p16_17;
        //     thisRegion = p16_17;
        //     break;
        // case 36:
        //     thisCount = len_p16_18;
        //     thisRegion = p16_18;
        //     break;
        // case 37:
        //     thisCount = len_p15_18;
        //     thisRegion = p15_18;
        //     break;
        // case 38:
        //     thisCount = len_p14_18;
        //     thisRegion = p14_18;
        //     break;
        // case 39:
        //     thisCount = len_p15_19;
        //     thisRegion = p15_19;
        //     break;
        // case 40:
        //     thisCount = len_p14_19;
        //     thisRegion = p14_19;
        //     break;
        // case 41:
        //     thisCount = len_p13_19;
        //     thisRegion = p13_19;
        //     break;
        // case 42:
        //     thisCount = len_p12_19;
        //     thisRegion = p12_19;
        //     break;
        // case 43:
        //     thisCount = len_p11_19;
        //     thisRegion = p11_19;
        //     break;
      }

      for (int l = 0; l < thisCount; l++)
      {
        //        Serial.println(printf("illuminating led %i %i | i %i | data start: %i \n", thisRegion[l], numLeds, i, i * 3));
        leds[thisRegion[l]] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      }
    }
    //    else {
    //    //   Serial.println(printf("pixel not illuminated %i \n", led));
    //      Serial.print(".");
    //    }
  }
  previousDataLength = length;
  FastLED.show();
  frameNo++;
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

  //ledTestFrame();
}
