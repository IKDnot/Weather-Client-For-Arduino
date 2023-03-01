#include "WeatherClient.h"

WeatherClient::WeatherClient(const char* ssid, const char* password, const char* city, const char* apiKey, bool displayAsCelsius)
  : _ssid(ssid),
    _password(password),
    _city(city),
    _apiKey(apiKey),
    _displayAsCelsius(displayAsCelsius),
    _weather(0)
{
    _weather = new Weather;
}

WeatherClient::~WeatherClient(){
  delete _weather;
  _weather = 0;
}

float WeatherClient::getMaxTemperature() {
  return _weather->maxTemp;
}

float WeatherClient::getMinTemperature() {
  return _weather->minTemp;
}

uint WeatherClient::getWeatherId() {
  return _weather->weatherId;
}

String WeatherClient::getWeather() {
  return _weather->weatherAbst;
}

String WeatherClient::getWeatherDescription() {
  return _weather->weatherDesc;
}

bool WeatherClient::connectWiFi() {
  Serial.println("Connecting to WiFi...");
  Serial.println(_ssid);
  Serial.println(_password);
  WiFi.begin(_ssid, _password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    retries++;
    if (retries > 10) {
      Serial.println("Could not connect to WiFi");
      return false;
    }
  }
  Serial.println("Connected to WiFi");
  return true;
}

bool WeatherClient::fetchWeatherData() {
  HTTPClient http;
  char url[1024];
  sprintf(url, _serverName, _city, _apiKey);
  http.begin(url);
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    _weather->maxTemp = doc["main"]["temp_max"];
    _weather->minTemp = doc["main"]["temp_min"];
    if (_displayAsCelsius) {
      const float kF2C = 273.15;
      _weather->maxTemp -= kF2C;
      _weather->minTemp -= kF2C;
    }
    _weather->weatherId = doc["weather"][0]["id"];
    _weather->weatherAbst = doc["weather"][0]["main"].as<String>();
    _weather->weatherDesc = doc["weather"][0]["description"].as<String>();
  }
  else {
    Serial.print("Error getting weather data. HTTP error code: ");
    Serial.println(httpResponseCode);
    return false;
  }
  http.end();
  return true;
}

bool WeatherClient::update() {
  if (WiFi.status() != WL_CONNECTED) {
    if (!connectWiFi()) {
      return false;
    }
  }
  if (!fetchWeatherData()) {
    return false;
  }
  return true;
}
