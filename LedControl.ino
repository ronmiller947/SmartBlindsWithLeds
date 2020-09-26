// Set defult settings for Leds
void SetLedDefaults() {
  delay(3000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  theme = 1;
  count = 1;
  ledsOff = false;
  testMode = false;
  useTheme = true;
}

void turnOffLeds() {
  FastLED.clear();
  FastLED.show();
}

void RunLedTheme() {
  switch (theme)
    {
      case NewYear: render_data_with_palette(theme); FastLED.show(); FastLED.delay(20); break;
      case Valentine: render_data_with_palette(theme);  FastLED.show(); FastLED.delay(20); break;
      case StPatty: render_data_with_palette(theme);  FastLED.delay(20); break;
      case Easter: render_data_with_palette(theme); FastLED.show(); FastLED.delay(20); break;
      case Memorial: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Independence: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Labor: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Columbus: render_data_with_palette(theme);  FastLED.show(); FastLED.delay(20); break;
      case Holloween: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Thanksgiving: render_data_with_palette(theme);  FastLED.show(); FastLED.delay(20); break;
      case Vetrens: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Christmas: render_data_with_palette(theme); FastLED.show(); FastLED.delay(20); break;
      case Police: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Fire: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Medical: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
      case Ravens: render_data_with_palette(theme); add_glitter(); FastLED.show(); FastLED.delay(20); break;
    }
}

void LedTestMode() {
  //For Testing Themes Cycle through themes
  Serial.print("current theme: ");
  Serial.println(theme);
  Serial.print("current count: ");
  Serial.println(count);
  count += 1;
  if (theme >= 14 && count >= 1)
  {
    Serial.println("previous theme: " + theme);
    Serial.println("previous count: " + count);
    if (theme >= 16)
    {
      theme = 1;
    }
    else
    {
      theme += 1;
    }
    count = 0;
    Serial.println("new theme: " + theme);
    Serial.println("new count: " + count);
  }
  else if (count >= 400)
  {
    Serial.println("previous theme: " + theme);
    Serial.println("previous count: " + count);
    if (theme >= 16) {
      theme = 1;
    }
    else
    {
      theme += 1;
    }
    count = 0;
    Serial.println("new theme: " + theme);
    Serial.println("new count: " + count);
  }
}


void fill_data_array()
{
  static uint8_t startValue = 0;
  startValue = startValue + 2;
  uint8_t value = startValue;
  for ( int i = 0; i < NUM_LEDS; i++) {
    Data[i] = triwave8( value); // convert value to an up-and-down wave
    value += 7;
  }
}
