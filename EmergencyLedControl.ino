void tiggerEmergencyLights(CHSV color1, CHSV color2) {
  int delayArr[8] = {50, 63, 75, 88, 100, 113, 125, 138};
  int currentDelay = random(0, 7);
  // Turn the LEDs on
  for (int i = 0; i < NUM_LEDS; i++) {
    int centerLed = NUM_LEDS / 2;
    if (i < centerLed) {
      leds[i] = color1;
    } else {
      leds[i] = color2;
    }

  };
  FastLED.show();
  delay(delayArr[currentDelay]);

  // Now turn the LEDs off
  for (int i = 0; i < NUM_LEDS; i++) {
    int centerLed = NUM_LEDS / 2;
    if (i < centerLed) {
      leds[i] = color2;
    } else {
      leds[i] = color1;
    }
  }
  FastLED.show();
  delay(delayArr[currentDelay]);

  // Turn the LEDs on
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color1;
  };
  FastLED.show();
  delay(delayArr[currentDelay]);

  // Now turn the LEDs off
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color2;
  };
  FastLED.show();
  delay(delayArr[currentDelay]);

}

void emergencyLeds(int theme) {

  switch (theme) {
    case Police: {
        CHSV color1(0, 255, 255);    //red
        CHSV color2(160, 255, 255); //blue
        tiggerEmergencyLights(color1, color2);
        break;
      }
    case Fire: {
        CHSV color1(0, 255, 255);    //red
        CHSV color2(32, 255, 255); //Yellow
        tiggerEmergencyLights(color1, color2);
        break;
      }
    case Medical: {
        CHSV color1(0, 0, 255);    //White
        CHSV color2(160, 255, 255); //blue
        tiggerEmergencyLights(color1, color2);
        break;
      }
  }
}
