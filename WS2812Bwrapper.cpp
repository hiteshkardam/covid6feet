#include <FastLED.h>

#define DATA_PIN 5		// digital pin for LED strip
#define NUM_LEDS 9		// number of led diodes in the strip
CRGB leds[NUM_LEDS];

int minbrightness = 2;
int maxbrightness = 255;  //0-255
int brightness = minbrightness;
int dot;

void tored();
void togreen();


void initWS2812B()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void brightnessrampup()
{
  while(brightness != maxbrightness)
  {
    brightness++;
    FastLED.setBrightness(brightness);
    FastLED.show();
    FastLED.delay(2);
  }
}


void brightnessrampdown()
{
  while(brightness != minbrightness)
  {
    brightness--;
    FastLED.setBrightness(brightness);
    FastLED.show();
    FastLED.delay(2);
  }
}


void searching()
{
  if (leds[0].green != 255)
  {
    for(int r=leds[0].red, g=leds[0].green; g<=255 && r>=0; r--,g++)
    {
      fill_solid(leds, NUM_LEDS, CRGB(r, g, 0));
      FastLED.show();
      FastLED.delay(1);
    }
  }
  
  brightnessrampdown();
  
  for(dot=0; dot<NUM_LEDS; dot++)
  {
    if (dot==4)
    {
      leds[dot] = CRGB(0, 255, 0);
    }
    else
    {
      leds[dot] = CRGB(0, 255, 0);
      leds[NUM_LEDS-dot-1] = CRGB(0, 255, 0);
    }
    
    FastLED.setBrightness(brightness);
    FastLED.show();
    
    if (dot==4)
    {
      leds[dot] = CRGB(0, 0, 0);
    }
    else if (dot==0)
    {
      leds[NUM_LEDS-dot-1] = CRGB(0, 255, 0);
      leds[dot] = CRGB(0, 255, 0);
    }
    else
    {
      leds[NUM_LEDS-dot-1] = CRGB(0, 0, 0);
      leds[dot] = CRGB(0, 0, 0);
    }
    delay(150);
  }
}


void searchingfadetocolour(int r, int g, int b)
{
  for(dot=0; dot<=NUM_LEDS/2; dot++)
  {
    if (dot==4)
    {
      leds[dot] = CRGB(r, g, b);
    }
    else
    {
      leds[dot] = CRGB(r, g, b);
      leds[NUM_LEDS-dot-1] = CRGB(r, g, b);
    }
    FastLED.delay(150);
    FastLED.setBrightness(brightness);
    FastLED.show();
  }
  FastLED.delay(50);
  brightnessrampup();
}


void searchingfadeout()
{
  brightnessrampdown();
  FastLED.delay(150);
  for(dot = NUM_LEDS/2; dot>=0; dot--)
  {
    if (dot==4)
    {
      leds[dot] = CRGB(0, 0, 0);
    }
    else
    {
      leds[dot] = CRGB(0, 0, 0);
      leds[NUM_LEDS-dot-1] = CRGB(0, 0, 0);
    }
    FastLED.show();
    FastLED.delay(150);
  }
}


void showcolour(int r, int g, int b)
{
  for(dot=0; dot<NUM_LEDS; dot++)
  {
    leds[dot] = CRGB(r, g, b);
    FastLED.show();
  }
  
  if (brightness != maxbrightness)
  brightnessrampup();
}


void redflash()
{
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  //FastLED.delay(300);
  brightnessrampup();
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();
  FastLED.delay(300);
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  FastLED.delay(300);
}


void tored()
{
  if (leds[0].red != 255)
  {
    for(int r=leds[0].red; r<=255; r++)
    {
      fill_solid(leds, NUM_LEDS, CRGB(r, 255-r, 0));
      FastLED.show();
      FastLED.delay(1);
    }
  }
  
  if (brightness != maxbrightness)
  brightnessrampup();
}


void togreen()
{
  if (leds[0].green != 255)
  {
    for(int g=leds[0].green; g<=255; g++)
    {
      fill_solid(leds, NUM_LEDS, CRGB(255-g, g, 0));
      FastLED.show();
      FastLED.delay(1);
    }
  }
  
  if (brightness != maxbrightness)
  brightnessrampup();
}
