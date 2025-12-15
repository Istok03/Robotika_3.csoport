#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>



// WiFi adatok (cseréld ki a saját adataidra!)
const char* WIFI_SSID = "Sajat wifinev";
const char* WIFI_PASSWORD = "kod";
const char* SERVER_URL = "http://asd:5000/api/weather"; 

// Szenzorok
//DHT dht(D2, DHT11); // DHT D2 pin, típus: DHT11 (vagy DHT22)
Adafruit_BME280 bme;

//lcd
#define USE_LCD true 

#if USE_LCD 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C cím lehet 0x27 vagy 0x3F
#endif


// Időzítés
const unsigned long SEND_INTERVAL = 10000; // 10 másodpercenként küld adatot
unsigned long lastSendTime = 0;

// Hibakezelés
const int MAX_SENSOR_RETRIES = 3; // Max. próbálkozások száma a szenzorolvasáshoz
const int RETRY_DELAY_MS = 2000;  // Késleltetés az újrapróbálkozások között

void ensureWiFiConnection() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.println("WiFi kapcsolódás folyamatban...");
  
  // Ha esetleg beragadt volna
  WiFi.disconnect(); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi sikeresen csatlakozva!");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Szenzorok indítása

  if (!bme.begin(0x76)) { // vagy 0x77, ha nem működik
    Serial.println("Hiba: BMP280 szenzor nem található!");
  } 
  else {
  Serial.println("BME280 szenzor OK.");

//lcd 
  #if USE_LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weather Ready");
#endif


}
 
  // WiFi csatlakozás
  // WiFi csatlakozás
  ensureWiFiConnection();
}

bool readAndValidateSensors(float &temperature, float &humidity, float &pressure) {
  int retries = 0;
  
  while (retries < MAX_SENSOR_RETRIES) {
    Serial.print("  Szenzor olvasás (Próbálkozás: ");
    Serial.print(retries + 1);
    Serial.println(")");

    // Adatok beolvasása
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100.0; // Pa -> hPa

    // Ellenőrzés NaN ellen: Ha MINDEN adat érvényes, sikeres az olvasás
    if (!isnan(temperature) && !isnan(humidity) && !isnan(pressure)) {
      Serial.println("  -> Sikeres adat olvasás!");
      return true; // Sikeres
    }
    
    // Hiba esetén
    Serial.println("  -> Hiba: Érvénytelen adat (NaN). Újrapróbálkozás...");
    retries++;

    if (retries < MAX_SENSOR_RETRIES) {
        delay(RETRY_DELAY_MS); // Várjunk, mielőtt újra próbálkozunk
    }

  }
  
  Serial.println("HIBA: Túl sok próbálkozás. Adatküldés kihagyva.");
  return false; // Sikertelen
}

void sendData(float temperature, float humidity, float pressure) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, SERVER_URL);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["id"] = 0;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;

  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = http.POST(requestBody);

  Serial.print("HTTP válaszkód: ");
  Serial.println(httpResponseCode);

  http.end();
}

void loop() {
  unsigned long currentTime = millis();

  // Csak a megadott időnként fut le
  if (currentTime - lastSendTime >= SEND_INTERVAL) {
    lastSendTime = currentTime; 
    
    float temperature, humidity, pressure;

    // 1. Olvasás és validálás
    if (readAndValidateSensors(temperature, humidity, pressure)) {
      // 2. Küldés, ha az olvasás sikeres volt
      sendData(temperature, humidity, pressure);
        #if USE_LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(temperature, 1);
        lcd.print("C H:");
        lcd.print(humidity, 0);

        lcd.setCursor(0, 1);
        lcd.print("P:");
        lcd.print(pressure, 0);
        lcd.print(" hPa");
        #endif


    }
    
  }
 

  // Megjegyzés: ide kerülhetne más, nem blokkoló feladat, pl. kijelző frissítése
}