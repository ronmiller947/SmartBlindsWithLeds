void add_glitter()
{
  int chance_of_glitter =  5; // percent of the time that we add glitter
  int number_of_glitters = 3; // number of glitter sparkles to add

  int r = random8(100);
  if ( r < chance_of_glitter ) {
    for ( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White; // very bright glitter
    }
  }
}
