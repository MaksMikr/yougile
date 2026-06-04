#include <iostream>
#include <string>
using namespace std;

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

int main() {
    setlocale(LC_ALL, "rus");

    Weather w("Киров", 49.6601, 58.5966, 5.69, "дождь", 4.27, 100);
    w.print();

    return 0;
}