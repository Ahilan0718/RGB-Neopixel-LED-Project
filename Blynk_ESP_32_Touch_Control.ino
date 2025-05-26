#define BLYNK_TEMPLATE_ID "TMPL37Vw79qtG"
#define BLYNK_TEMPLATE_NAME "ESP 32 TOUCH CONTROL"
#define BLYNK_AUTH_TOKEN "x1fZCNJd3bcTh-glMKUOBomV3c1IEqRy"

#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "RnD";
char pass[] = "P@ssw0rd";

#define TOUCH_PIN     15
#define LED_PIN       5
#define NUM_LEDS      24

#define BLYNK_SWITCH  V0
#define RED_SLIDER    V1
#define GREEN_SLIDER  V2
#define BLUE_SLIDER   V3

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

bool appSwitchState = false;
bool lastTouchState = false;
int red = 0, green = 0, blue = 255;
void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);

  strip.begin();
  strip.show(); 

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Blynk.syncVirtual(RED_SLIDER, GREEN_SLIDER, BLUE_SLIDER, BLYNK_SWITCH);
}

BLYNK_WRITE(BLYNK_SWITCH) {
  appSwitchState = param.asInt();
  if (appSwitchState) {
    updateLEDs();
  } else {
    turnOffLEDs();
  }
}

BLYNK_WRITE(RED_SLIDER) {
  red = param.asInt();
  if (appSwitchState) updateLEDs();
}

BLYNK_WRITE(GREEN_SLIDER) {
  green = param.asInt();
  if (appSwitchState) updateLEDs();
}

BLYNK_WRITE(BLUE_SLIDER) {
  blue = param.asInt();
  if (appSwitchState) updateLEDs();
}

void loop() {
  Blynk.run();

  bool touchDetected = digitalRead(TOUCH_PIN) == HIGH;

  if (touchDetected != lastTouchState) {
    lastTouchState = touchDetected;
    appSwitchState = touchDetected;
    Blynk.virtualWrite(BLYNK_SWITCH, appSwitchState);

    if (appSwitchState) {
      updateLEDs();
    } else {
      turnOffLEDs();
    }
  }
}

void updateLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void turnOffLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}


