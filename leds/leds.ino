
#define LED_PIN    23

#define LED_COUNT 300
const int top_start = 33;
const int top_end = 164;
const int top_center = 100;

const int bottom_start = 177;
const int bottom_end = 300;
const int bottom_center = 240;
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

unsigned long firefliesArray[LED_COUNT] = {0};
void setup() {
  Serial.begin(115200);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  firefliesDTHSOC(.004, 1500, 180, 255, 200, true, 2, top_start, top_end, top_center);
  firefliesDTHSOC(.004, 1500, 180, 255, 200, true, 2, bottom_start, bottom_end, bottom_center);
  strip.show();
}


void firefliesDTHSOC(float d, int t, uint16_t h, byte s, byte b, boolean o, byte c, int start, int end, int center) {
  for (int i = start; i < end; i++) {
    if (millis() - firefliesArray[i] > (unsigned long)t) {
      if (random(0, 1000) < d * 1000.0000) {
        firefliesArray[i] = millis();
      }
    }
  }
  for (int i = start; i < end; i++) {
    // more color modes here (c)
    if (c == 1) {
      h = (firefliesArray[i] << 8) % 65535;
    } if (c == 2) {
      if (i > (start + end) / 2) {
        h = (firefliesArray[i] << 2) - ((i - center) << 10);
      } else {
        h = (firefliesArray[i] << 2) + ((i - center) << 10);
      }
    }
    if (-firefliesArray[i] + millis() <= t / 2 && -firefliesArray[i] + millis() > 0) {
      strip.setPixelColor(i, strip.ColorHSV(h, s, map(millis() - firefliesArray[i], 0, t / 2, 0, b)));
    }
    if (-firefliesArray[i] + millis() > t / 2 && -firefliesArray[i] + millis() <= t) {
      strip.setPixelColor(i, strip.ColorHSV(h, s, map(millis() - firefliesArray[i], t / 2, t, b, 0)));
    }
    if (-firefliesArray[i] + millis() > t && o == true) {
      strip.setPixelColor(i, strip.ColorHSV(0, 0, 0));
    }
  }
}
