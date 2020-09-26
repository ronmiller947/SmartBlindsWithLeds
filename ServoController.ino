void servo_move() {
  Serial.println("State Change. Rotating Servo");
  if ( dirc == 180) {
    myservo.attach(SERVO_PIN);              // energize servo
    delay(50);
    for (pos = 0; pos <= 179; pos += 1) {   // goes from 0 degrees to 90 degrees in steps of 1 degree CHANGE 90 TO MATCH ANGLE OF TILT DESIRED
      myservo.write(pos);                  // tell servo to go to position in variable 'pos'
      delay(30);                           // waits 30ms between each degree to slow rotation speed
    }
    delay(50);
    myservo.detach();                      // movement finished so detach servo to conserve power
  }
  else if (dirc == 0) {
    myservo.attach(SERVO_PIN);              // energize servo
    delay(50);
    for (pos = 179; pos >= 0; pos -= 1) {   // goes from 90 degrees to 0 degrees in steps of 1 degree CHANGE 90 TO MIRROR ANGLE OF TILT DESIRED ABOVE
      myservo.write(pos);                  // tell servo to go to position in variable 'pos'
      delay(30);                           // waits 30ms between each degree to slow rotation speed
    }
    delay(50);
    myservo.detach();                      // movement finished so detach servo to conserve power
  }

  Serial.println("Returning to main loop");
  return;
}
