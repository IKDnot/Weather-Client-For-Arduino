#ifndef WeatherClient_h
#define WeatherClient_h

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

typedef struct {
    float maxTemp;
    float minTemp;
    uint weatherId;
    String weatherAbst;
    String weatherDesc;
} Weather;

class WeatherClient {
  public:
    WeatherClient(const char* ssid, const char* password, const char* city, const char* apiKey, bool displayAsCelsius = true);
    ~WeatherClient();
    float getMaxTemperature();
    float getMinTemperature();
    uint getWeatherId();
    String getWeather();
    String getWeatherDescription();
    bool update();

  private:
    const char* _ssid;
    const char* _password;
    const char* _city;
    const char* _apiKey;
    const char* _serverName = "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s";
    bool _displayAsCelsius;
    Weather* _weather;
    bool connectWiFi();
    bool fetchWeatherData();
};

#endif
