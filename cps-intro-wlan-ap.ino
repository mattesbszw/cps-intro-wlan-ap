#include <WiFi.h>
#include <WebServer.h>

constexpr uint8_t LED_OUTPUT_1 = 26;

constexpr const char* ssid = "...";
constexpr const char* password = "...";

WebServer server(80);

String state1Text = "off";

void handleRoot() {
  writeWebsite();
}

void handleLED1On() {
  Serial.println("LED #1 on");
  state1Text = "on";
  digitalWrite(LED_OUTPUT_1, HIGH);
  writeWebsite();
}

void handleLED1Off() {
  Serial.println("LED #1 off");
  state1Text = "off";
  digitalWrite(LED_OUTPUT_1, LOW);
  writeWebsite();
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_OUTPUT_1, OUTPUT);
  digitalWrite(LED_OUTPUT_1, LOW);

  // create AP, print ip and mac address
  Serial.println("Creating Access Point " + String(ssid));
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  Serial.println("");
  Serial.print("MAC address: ");
  Serial.println(WiFi.softAPmacAddress());
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // web server routing: url, function name 
  server.on("/", handleRoot);
  server.on("/LED/1/on", handleLED1On);
  server.on("/LED/1/off", handleLED1Off);

  server.begin();
}

void loop() {
  server.handleClient();
}

void writeWebsite() {
  String website_code = "";
  website_code += "<!DOCTYPE html>\n<html>\n";
  website_code += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
  website_code += "<link rel=\"icon\" href=\"data:,\">\n";
  website_code += "<style>html { font-family: Helvetica; text-align: center;}\n";
  website_code += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;\n";
  website_code += "font-size: 30px; margin: 2px; cursor: pointer;}\n";
  website_code += ".button2 {background-color: #555555;}</style>\n</head>\n";
  website_code += "<body>\n<h1>ESP32 Web Server</h1>\n";
  website_code += "<p>LED #1 - Current State: " + state1Text + "</p>\n";

  if (state1Text == "off") {
    website_code += "<p><a href=\"/LED/1/on\"><button class=\"button\">TURN ON</button></a></p>\n";
  } else {
    website_code += "<p><a href=\"/LED/1/off\"><button class=\"button button2\">TURN OFF</button></a></p>\n";
  }
  
  website_code += "</body>\n</html>\n";

  server.send(200, "text/html", website_code);
}
