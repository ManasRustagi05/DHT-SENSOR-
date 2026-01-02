#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* -------- USER SETTINGS -------- */
const char* ssid = "JioFiber-Rita";
const char* password = "(@bAbu#1)";
/* -------------------------------- */

#define DHTPIN 4
#define DHTTYPE DHT11        // Change to DHT22 if needed

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float temp = 0, hum = 0;

/* -------- WEB PAGE -------- */
void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='3'></head><body>";
  html += "<h2>ESP32 DHT Monitor</h2>";
  html += "<p>Temperature: " + String(temp) + " °C</p>";
  html += "<p>Humidity: " + String(hum) + " %</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

/* -------- SETUP -------- */
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nESP32 Starting...");

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected");
  display.println(WiFi.localIP());
  display.display();

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Web Server Started");
}

/* -------- LOOP -------- */
void loop() {
  server.handleClient();

  hum = dht.readHumidity();
  temp = dht.readTemperature();

  if (!isnan(temp) && !isnan(hum)) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(temp);
    display.println(" C");

    display.setCursor(0, 32);
    display.print(hum);
    display.println(" %");
    display.display();
  }

  delay(2000);
}