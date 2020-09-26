
//Configure Server For Blinds
void ServerSetup() {
  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });
  server.on("/open", []() {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP OPEN COMMAND RECEIVED");
    dirc = 0;  // direction for servo to run
    state = 2; // sets current state
  });
  server.on("/close", []() {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP CLOSE COMMAND RECEIVED");
    dirc = 180; // direction for servo to run
    state = 1;  // sets current state
  });
  server.on("/NewYear", []() {
    server.send(200, "text/html", webPage);
    theme = NewYear;
  });
  server.on("/Valentine", []() {
    server.send(200, "text/html", webPage);
    theme = Valentine;
  });
  server.on("/StPatty", []() {
    server.send(200, "text/html", webPage);
    theme = StPatty;
  });
  server.on("/Easter", []() {
    server.send(200, "text/html", webPage);
    theme = Easter;
  });
  server.on("/Memorial", []() {
    server.send(200, "text/html", webPage);
    theme = Memorial;
  });
  server.on("/Independence", []() {
    server.send(200, "text/html", webPage);
    theme = Independence;
  });
  server.on("/Labor", []() {
    server.send(200, "text/html", webPage);
    theme = Labor;
  });
  server.on("/Columbus", []() {
    server.send(200, "text/html", webPage);
    theme = Columbus;
  });
  server.on("/Holloween", []() {
    server.send(200, "text/html", webPage);
    theme = Holloween;
  });
  server.on("/Thanksgiving", []() {
    server.send(200, "text/html", webPage);
    theme = Thanksgiving;
  });
  server.on("/Vetrens", []() {
    server.send(200, "text/html", webPage);
    theme = Vetrens;
  });
  server.on("/Christmas", []() {
    server.send(200, "text/html", webPage);
    theme = Christmas;
  });
  server.on("/Ravens", []() {
    server.send(200, "text/html", webPage);
    theme = Ravens;
  });
  server.on("/Police", []() {
    server.send(200, "text/html", webPage);
    theme = Police;
  });
  server.on("/Fire", []() {
    server.send(200, "text/html", webPage);
    theme = Fire;
  });
  server.on("/Medical", []() {
    server.send(200, "text/html", webPage);
    theme = Medical;
  });




  server.begin();
  Serial.println("HTTP server started");
}
