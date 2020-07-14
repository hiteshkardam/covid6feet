#include <FastLED.h>

#define DATA_PIN 5		// digital pin for LED strip
#define NUM_LEDS 9		// number of led diodes in the strip
CRGB leds[NUM_LEDS];

int minbrightness = 2;
int maxbrightness = 50;  //0-255
int brightness = minbrightness;
int dot;

void initWS2812B()
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}


void searching()
{
  for(dot=0; dot<NUM_LEDS; dot++)
  {
    if (dot==4)
    {
      leds[dot] = CHSV(96, 255, brightness);
    }
    else
    {
      leds[dot] = CHSV(96, 255, brightness);
      leds[NUM_LEDS-dot-1] = CHSV(96, 255, brightness);
    }
   
    FastLED.show();
    
    if (dot==4)
    {
      leds[dot] = CHSV(96, 0, 0);
    }
    else
    {
      leds[NUM_LEDS-dot-1] = CHSV(96, 0, 0);
      leds[dot] = CHSV(96, 0, 0);
    }
    delay(150);
  }
}


void searchingfadein()
{
  for(dot=0; dot<=NUM_LEDS/2; dot++)
  {
    if (dot==4)
    {
      leds[dot] = CHSV(96, 255, brightness);
    }
    else
    {
      leds[dot] = CHSV(96, 255, brightness);
      leds[NUM_LEDS-dot-1] = CHSV(96, 255, brightness);
    }
    delay(150);
    FastLED.show();
  }
  delay(50);
  while(brightness !=maxbrightness)
  {
    brightness++;
    fill_solid(leds, NUM_LEDS, CHSV( 96, 255, brightness));
    FastLED.show();
    delay(5);
  }
}


void searchingfadeout()
{
  while(brightness != minbrightness)
  {
    brightness--;
    fill_solid(leds, NUM_LEDS, CHSV( 96, 255, brightness));
    FastLED.show();
    delay(5);
  }
  
  delay(50);
  for(dot = NUM_LEDS/2; dot>=0; dot--)
  {
    if (dot==4)
    {
      leds[dot] = CHSV(96, 0, 0);
    }
    else
    {
      leds[dot] = CHSV(96, 0, 0);
      leds[NUM_LEDS-dot-1] = CHSV(96, 0, 0);
    }
    FastLED.show();
    delay(150);
  }
}


void showcolour(int colourhue)
{
  for(dot=0; dot<NUM_LEDS; dot++)
  {
    leds[dot] = CHSV(colourhue, 255, brightness);
    FastLED.show();
  }
}


void redflash()
{
  fill_solid(leds, NUM_LEDS, CHSV( 0, 255, brightness));
  FastLED.show();
  delay(300);
  fill_solid(leds, NUM_LEDS, CHSV( 0, 0, 0));
  FastLED.show();
  delay(300);
  fill_solid(leds, NUM_LEDS, CHSV( 0, 255, brightness));
  FastLED.show();
  delay(300);
}


void greentored()
{
  for(int hue=96; hue>0; hue--)
  {
    fill_solid(leds, NUM_LEDS, CHSV(hue, 255, brightness));
    FastLED.show();
    delay(10);
  }
}


void redtogreen()
{
  for(int hue=0; hue<96; hue++)
  {
    fill_solid(leds, NUM_LEDS, CHSV(hue, 255, brightness));
    FastLED.show();
    delay(10);
  }
}