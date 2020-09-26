//Setup Wifi 
void SetupWifi() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("Connecting");
  delay(500);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }  
}
