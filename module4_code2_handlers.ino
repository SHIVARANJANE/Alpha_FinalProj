void handleOn() {
  digitalWrite(RELAY_PIN, HIGH);
  server.sendHeader("Access-Control-Allow-Origin", "*"); // CORS header
  server.send(200, "text/plain", "Bulb turned ON");
}

void handleOff() {
  digitalWrite(RELAY_PIN, LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*"); // CORS header
  server.send(200, "text/plain", "Bulb turned OFF");
}

void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*"); // Optional: allow root access
  server.send(200, "text/plain", "ESP32 Bulb Controller. Use /on or /off");
}
