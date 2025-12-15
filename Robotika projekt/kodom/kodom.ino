#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3); // RX=2
const int LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  espSerial.begin(9600);
  
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("--- VEVO INDUL ---");
}

void loop() {
  if (espSerial.available()) {
    delay(50); 
    String data = espSerial.readStringUntil('\n');

    Serial.print("Bejovo adat: ");
    Serial.println(data);

    if (data.indexOf("temp") >= 0) {
      Serial.println(">> ADAT OK! LED BEKAPCSOLASA...");
      
      
      sendToBackend(data); 
    } 
    else {
    }
  }
}

void sendToBackend(String packet) {
  Serial.println("[API] Adat feltoltve.");
}