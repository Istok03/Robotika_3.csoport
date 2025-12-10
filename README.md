# 🌦️ WeatherBackend – IoT Időjárás REST API

Ez a projekt egy ASP.NET Core Web API, amely fogadja az ESP8266 által küldött időjárási adatokat (hőmérséklet, páratartalom, légnyomás), és válaszként visszajelez, hogy az adatokat sikeresen fogadta.

## 📌 API végpont

### POST `/api/weather`

Fogadja az ESP8266 által küldött JSON adatokat.

## 🧰 Szükséges fizikai eszközök

A projekt működéséhez az alábbi hardverelemekre van szükség:

| Eszköz neve                  | Leírás / Funkció                                                                 |
|-----------------------------|----------------------------------------------------------------------------------|
| **ESP8266 (pl. NodeMCU)**   | Mikrovezérlő WiFi kapcsolattal, amely a szenzoradatokat gyűjti és továbbítja az API-nak |
| **DHT11 vagy DHT22 szenzor**| Hőmérséklet és páratartalom mérésére szolgáló digitális szenzor                 |
| **Adafruit BMP180 vagy BMP085 szenzor** | Légnyomás mérésére alkalmas szenzor (I2C kommunikációval)             |
| **USB kábel (micro USB)**   | Az ESP8266 programozásához és tápellátásához szükséges                          |
| **Breadboard (próbatábla)** | Az áramkör összekötéséhez forrasztás nélkül                                     |
| **Jumper kábelek (male-male)** | A szenzorok és az ESP8266 összekötéséhez                                     |
| **5V-os tápegység (opcionális)** | Ha nem USB-ről kap tápot az ESP8266, külső tápegység is használható     |



#### 🔸 Kérés (példa):

```json
{
  "temperature": 22.5,
  "humidity": 55.0,
  "pressure": 1013.2
}

