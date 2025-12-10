#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

// WiFi adatok (cseréld ki a saját adataidra!)
const char* WIFI_SSID = "WIFI_NEV";
const char* WIFI_PASSWORD = "WIFI_JELSZO";
const char* SERVER_URL = "http://192.168.1.100:5000/api/weather"; 

// Szenzorok
DHT dht(D2, DHT11); // DHT D2 pin, típus: DHT11 (vagy DHT22)
Adafruit_BMP085 bmp;

// Időzítés
const unsigned long SEND_INTERVAL = 10000; // 10 másodpercenként küld adatot
unsigned long lastSendTime = 0;

// Hibakezelés
const int MAX_SENSOR_RETRIES = 3; // Max. próbálkozások száma a szenzorolvasáshoz
const int RETRY_DELAY_MS = 2000;  // Késleltetés az újrapróbálkozások között

void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Szenzorok indítása
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Hiba: BMP szenzor inicializálás sikertelen!");
    // Itt leállíthatod a programot, vagy folytathatod a hibás szenzor nélkül
  } else {
    Serial.println("BMP szenzor OK.");
  }
  
  // WiFi csatlakozás
  Serial.print("Csatlakozás WiFi-hez (");
  Serial.print(WIFI_SSID);
  Serial.println(")...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi csatlakozva! IP: ");
  Serial.println(WiFi.localIP());
}

bool readAndValidateSensors(float &temperature, float &humidity, float &pressure) {
  int retries = 0;
  
  while (retries < MAX_SENSOR_RETRIES) {
    Serial.print("  Szenzor olvasás (Próbálkozás: ");
    Serial.print(retries + 1);
    Serial.println(")");

    // Adatok beolvasása
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    pressure = bmp.readPressure() / 100.0; // Pa -> hPa

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
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Hiba: Nincs WiFi kapcsolat! Újrapróbálkozás...");
    // Ide lehetne tenni egy újraindítási logikát
    return;
  }
  
  // JSON létrehozása
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;

  String jsonPayload;
  serializeJson(doc, jsonPayload);
  
  Serial.print("JSON küldése: ");
  Serial.println(jsonPayload);

  // HTTP POST küldése
  HTTPClient http;
  
  http.begin(SERVER_URL); 
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonPayload);
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP válaszkód: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("HTTP hiba: ");
    Serial.println(httpResponseCode);
  }
  
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
    }
    
  }

  // Megjegyzés: ide kerülhetne más, nem blokkoló feladat, pl. kijelző frissítése
}