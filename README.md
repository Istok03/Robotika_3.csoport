# 🌦️ WeatherBackend – IoT Időjárás REST API

Ez a projekt egy ASP.NET Core Web API, amely fogadja az ESP8266 által küldött időjárási adatokat (hőmérséklet, páratartalom, légnyomás), és válaszként visszajelez, hogy az adatokat sikeresen fogadta.

## 📌 API végpont

### POST `/api/weather`

Fogadja az ESP8266 által küldött JSON adatokat.

#### 🔸 Kérés (példa):

```json
{
  "temperature": 22.5,
  "humidity": 55.0,
  "pressure": 1013.2
}