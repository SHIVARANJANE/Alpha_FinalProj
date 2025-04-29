# Alpha_FinalProj
CONTROLLING AC DEVICES WIRELESSLY USING A WEBSITE AND ESP32
Abstract
This project enables wireless control of AC appliances like bulbs or fans using an ESP32 microcontroller. The system provides two modes of control:
1. Through the Blynk IoT App (cloud-based).
2. Via a web-based local server hosted on the ESP32 itself.

It is built using basic IoT components and ensures device control through relay switching. This project eliminates the need for third-party cloud services for the web interface, emphasizing self-hosted control and user accessibility over a local network.
Objectives
- To control AC devices remotely using a web browser or mobile application.
- To provide real-time switching feedback.
- To explore dual control paths (cloud-based and local control).
- To demonstrate IoT capabilities using ESP32 and relay modules.
Components Used
Hardware:
1. ESP32 Development Board
2. 4-Channel Relay Module (5V)
3. AC Bulbs (4 units)
4. Jumper Wires
5. 5V DC Power Supply
6. Breadboard (optional)
7. Switches (S1-S4 for manual override — optional)
Software:
1. Arduino IDE
2. Blynk IoT Platform
3. Postman (for API testing)
4. Visual Studio Code (for frontend development)
5. Web browser for local control
Working Principle
The ESP32 microcontroller is connected to a relay module which switches AC appliances ON or OFF based on digital signals. The control signal is generated either:
- Via Blynk App using the internet/cloud.
- Through a local web server running on the ESP32 accessed over Wi-Fi.

When a user interacts with the Blynk app or a web interface, a corresponding API or HTML request is sent to ESP32, which triggers a GPIO pin controlling the relay.


Control Method	User Action	Communication              	ESP32 Action	Appliance Effect
Blynk App	Tap button	Internet → Blynk Cloud	GPIO HIGH/LOW	ON/OFF
Web Interface	Toggle switch	Wi-Fi (HTTP GET)	GPIO HIGH/LOW	ON/OFF

Relay Module Control
•	A 4-channel relay module is used, allowing multiple AC appliances to be controlled independently.
•	Each relay is connected to a GPIO pin on the ESP32 and switches the LIVE wire of the AC supply.
•	When the relay coil is energized (GPIO set to HIGH), the Normally Open (NO) contact closes, completing the circuit and powering the appliance.
•	When the relay is OFF (GPIO set to LOW), the circuit is broken, and the appliance is turned OFF.

Circuit Diagram
 
Connection:
   

Modes of Control
1. Blynk App Control:
- ESP32 connects to Wi-Fi using credentials.
- Blynk platform sends commands via the internet to toggle relays.
- A specific virtual pin (V2) is used to control a GPIO pin connected to a relay.
 

 
Blynk Arduino Code:

#define BLYNK_PRINT Serial
#define RELAY_PIN 2 

#define BLYNK_TEMPLATE_ID "TMPL3L8nV2UVN"
#define BLYNK_TEMPLATE_NAME "IOT LIGHT"
#define BLYNK_AUTH_TOKEN "S7KbZhg1gYMfDJmg4acW9_2tmVbQzkrs"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Y35";
char pass[] = "2006____";

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V2) {
  int relayState = param.asInt();  
  digitalWrite(RELAY_PIN, relayState);
}
 
 


 
Working:

	        

2. Website Control (Local Web Server):
- ESP32 hosts a web server accessible via IP address on the local network.
- Web interface sends GET requests (/on and /off) to turn appliances ON/OFF.
       
Arduino Code for Web Server:

#include <WiFi.h>
#include <WebServer.h>

#define RELAY_PIN 2

const char* ssid = "Y35";
const char* password = "2006____";

WebServer server(80);

void handleOn() {
  digitalWrite(RELAY_PIN, HIGH);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Bulb turned ON");
}

void handleOff() {
  digitalWrite(RELAY_PIN, LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Bulb turned OFF");
}

void handleRoot() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "ESP32 Bulb Controller. Use /on or /off");
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
 
 
 
Frontend Web Interface (HTML + JS):

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Bulb Controller</title>
  <style>
    body { text-align: center; background: linear-gradient(45deg, #874da2, #c43a30); color: white; font-family: sans-serif; }
    .switch { display: inline-block; margin-top: 30px; }
    .slider { background-color: #ccc; transition: 0.4s; border-radius: 50px; height: 50px; width: 100px; position: relative; }
    .slider:before { content: ""; height: 40px; width: 40px; background: white; border-radius: 50%; position: absolute; bottom: 5px; left: 5px; transition: 0.4s; }
    input:checked + .slider { background-color: #4CAF50; }
    input:checked + .slider:before { transform: translateX(50px); }
  </style>
</head>
<body>
  <h1>💡 IoT Bulb Controller</h1>
  <label class="switch">
    <input type="checkbox" id="bulbToggle" onchange="toggleBulb(this.checked)">
    <span class="slider"></span>
  </label>
  <p id="status">Bulb is OFF</p>

  <script>
    const esp32IP = 'http://192.168.228.143';
    function toggleBulb(isOn) {
      const state = isOn ? 'on' : 'off';
      fetch(`${esp32IP}/${state}`)
        .then(res => res.text())
        .then(data => { document.getElementById("status").innerText = data; })
        .catch(err => { document.getElementById("status").innerText = "Error: " + err.message; });
    }
  </script>
</body>
</html>

Working:
	        


TESTING API USING POSTMAN:
            
Conclusion
This project successfully demonstrates how home automation can be achieved using simple IoT hardware and open-source platforms. By integrating both cloud and local control, it gives users flexibility and reliability, making it a practical and scalable solution for smart home applications.
Future Enhancements
- Add multiple device control (fan, TV, etc.).
- Integrate voice assistant control (e.g., Alexa, Google Home).
- Include mobile app notifications for device status.
- Add timer/scheduling functionality.
