/*
  Project: GreenVault 
  Description:
    ESP8266-based system that reads temperature, humidity, and gas levels,
    uploads them to Firebase Realtime Database, and displays data on an I2C LCD.
    The system also controls a relay based on temperature thresholds.
*/

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------------------- Wi-Fi Configurations -------------------- 
#define WIFI_SSID "Sanjixxx"        // Wi-Fi SSID
#define WIFI_PASSWORD "123xxxx"    // Wi-Fi Password

// -------------------- Sensor Pin Definitions -------------------- 
#define DHTPIN D5
#define DHTTYPE DHT11
#define MQ2_PIN A0                  
#define RELAY_PIN D4                // Relay control pin

// -------------------- Object Instantiations -------------------- 
DHT dht(DHTPIN, DHTTYPE);
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

unsigned long lastUpdate = 0;
int displayState = 0;               // variable to display current sensor data/state, 0: Temperature | 1: Humidity | 2: Gas

void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Relay configuration
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Connecting to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected. IP Address: ");
  Serial.println(WiFi.localIP());

  // Firebase initialization
  firebaseConfig.host = "greenvault-42631-default-rtdb.firebaseio.com";
  firebaseConfig.signer.tokens.legacy_token = "YAWjFqeYIbYveJxx3MRDPfMEQiVXRnPs57t20EHr";
  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

  // LCD initialization (I2C: SDA=D2, SCL=D1)
  Wire.begin(D2, D1);
  lcd.begin(16, 2);
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // Display Wi-Fi success
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");

  // Custom character for showing the degree symbol (°) on the LCD
  byte degreeSymbol[8] = {
    0b00000,
    0b00110,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };
  lcd.createChar(0, degreeSymbol);
}

void loop() {
  // Sensor Readings 
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int gasValue = analogRead(MQ2_PIN);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Sensor read failed!");
    return;
  }

  // Log readings to Serial
  Serial.printf("Humidity: %.2f%%  Temperature: %.2f°C  Gas: %d\n", humidity, temperature, gasValue);

  // Push Data to Firebase 
  Firebase.setFloat(firebaseData, "/Temp", temperature);
  Firebase.setFloat(firebaseData, "/Humidity", humidity);
  Firebase.setInt(firebaseData, "/Gas", gasValue);

  // LCD Update & Relay Control 
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= 2000) {
    lastUpdate = currentTime;
    displayState = (displayState + 1) % 3; // Rotate display mode

    if (displayState == 0) {
      // Display Temperature
      if (Firebase.get(firebaseData, "/FirebaseOK/LCD")) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature:");
        lcd.setCursor(0, 1);
        String tempStr = firebaseData.stringData();
        lcd.print(tempStr);
        lcd.write((byte)0);
        lcd.print(" C");

        // Relay logic(if less than 20°C  then both cooling fans operate at max power)
        int tempValue = tempStr.toInt();
        if (tempValue < 20) {
          digitalWrite(RELAY_PIN, HIGH);
          Serial.println("Relay ON");
        } else {
          digitalWrite(RELAY_PIN, LOW);
          Serial.println("Relay OFF");
        }
      } else {
        displayError();
      }

    } else if (displayState == 1) {
      // Display Humidity
      if (Firebase.get(firebaseData, "/Humidity")) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Humidity:");
        lcd.setCursor(0, 1);
        lcd.print(firebaseData.stringData());
        lcd.print(" %");
      } else {
        displayError();
      }

    } else if (displayState == 2) {
      // Display Gas Level
      if (Firebase.get(firebaseData, "/Gas")) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gas Level:");
        lcd.setCursor(0, 1);
        lcd.print(firebaseData.stringData());
      } else {
        displayError();
      }
    }
  }

  delay(3000);
}

// Firebase Error Handling and Display
void displayError() {
  Serial.print("Firebase error: ");
  Serial.println(firebaseData.errorReason());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Firebase Error");
}
