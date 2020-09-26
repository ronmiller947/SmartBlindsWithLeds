

void render_data_with_palette(int theme)
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    switch (theme) {
      case NewYear:
        NewYearTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette , Data[i], 128, LINEARBLEND);
        break;
      case Valentine:
        ValentineTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case StPatty:
        StPattyTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Easter:
        EasterTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, NOBLEND);
        break;
      case Memorial:
        MemorialTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Independence:
        IndependenceTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Labor:
        LaborTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Columbus:
        ColumbusTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Holloween:
        HolloweenTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Thanksgiving:
        ThanksgivingTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Vetrens:
        VetrensTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Christmas:
        ChristmasTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;
      case Ravens:
        RavensTheme_gPalette();
        leds[i] = ColorFromPalette( currentPalette, Data[i], 128, LINEARBLEND);
        break;

      case Police: emergencyLeds(theme); break;
      case Fire: emergencyLeds(theme); break;
      case Medical: emergencyLeds(theme); break;
    }
  }
}
