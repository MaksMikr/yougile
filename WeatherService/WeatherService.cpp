#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Weather {
private:
    string city;
    double lon;
    double lat;
    double temperature;
    string weather;
    double windSpeed;
    int clouds;

public:
    Weather(string city, double lon, double lat,
        double temperature, string weather,
        double windSpeed, int clouds)
        : city(city), lon(lon), lat(lat), temperature(temperature),
        weather(weather), windSpeed(windSpeed), clouds(clouds) {}

    void print() const {
        cout << "Информация о погоде: " << endl;
        cout << "Город: " << city << endl;
        cout << "Координаты: (" << lat << ", " << lon << ")" << endl;
        cout << "Температура: " << temperature << "°C" << endl;
        cout << "Погода: " << weather << endl;
        cout << "Скорость ветра: " << windSpeed << " м/с" << endl;
        cout << "Облачность: " << clouds << "%" << endl;
    }
};

class Service {
public:
    virtual Weather getWeather(string s) = 0;
    virtual ~Service() {}
};

class JsonService : public Service {
public:
    virtual Weather getWeather(string s) override {
        ifstream fin(s);
        if (!fin.is_open()) {
            throw runtime_error("Не удалось открыть файл");
        }

        json j;
        fin >> j;

        string city = j["name"];
        double lon = j["coord"]["lon"];
        double lat = j["coord"]["lat"];
        double temperature = j["main"]["temp"];
        string weather = j["weather"][0]["description"];
        double windSpeed = j["wind"]["speed"];
        int clouds = j["clouds"]["all"];

        return Weather(city, lon, lat, temperature, weather, windSpeed, clouds);
    }
};

int main() {
    setlocale(LC_ALL, "rus");

    try {
        JsonService js;
        Weather w = js.getWeather("weather.json");
        w.print();
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }

    return 0;
}