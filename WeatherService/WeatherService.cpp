#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

#include "nlohmann/json.hpp"
#include "pugixml.hpp"
using namespace pugi;
using json = nlohmann::json;

/// <summary>
/// Класс для хранения информации о погоде
/// </summary>
/// <remarks>
/// Содержит основные параметры погоды: город, координаты, температуру,описание погоды, скорость ветра и облачность.
/// </remarks>
class Weather {
private:
    string city;                  /// <summery>Название города</summery>
    double lon;                   /// <summery>Долгота</summery>
    double lat;                   /// <summery>Широта</summery>
    double temperature;           /// <summery>Температура в градумах Цельсия</summery>
    string weather;               /// <summery>Краткое описание погоды</summery>
    double windSpeed;             /// <summery>Скорость ветра в м/с</summery>
    int clouds;                   /// <summery>Облачность в процентах</summery>

public:
 /// <summary>
 /// Конструктор класса Weather
 /// </summary>
 /// <param name="city">Название города</param>
 /// <param name="lon">Долгота</param>
 /// <param name="lat">Широта</param>
 /// <param name="temperature">Температура (°C)</param>
 /// <param name="weather">Описание погоды</param>
 /// <param name="windSpeed">Скорость ветра (м/с)</param>
 /// <param name="clouds">Облачность (%)</param>
    Weather(string city, double lon, double lat,
        double temperature, string weather,
        double windSpeed, int clouds)
        : city(city), lon(lon), lat(lat), temperature(temperature),
        weather(weather), windSpeed(windSpeed), clouds(clouds) {}
    /// <summmary>
    /// Выводим информацию о погоде 
    /// </summmary>
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
/// <summary>Абстрактный класс для сервисов погоды</summary>
class Service {
public:
    /// <summary>Получить погоду из файла</summary>
    /// <param name="s">Путь к файлу</param>
    /// <returns>Объект Weather</returns>
    /// <exception cref="runtime_error">Ошибка при чтении файла</exception>
    virtual Weather getWeather(string s) = 0;
    virtual ~Service() {}
};
/// <summary>Сервис для JSON-файлов</summary>
class JsonService : public Service {
public:
    virtual Weather getWeather(string s) override {
        ifstream fin(s);
        if (!fin.is_open()) {
            throw runtime_error("Не удалось открыть JSON файл");
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
/// <summary>Сервис для XML-файлов</summary>
class XmlService : public Service {
public:
    /// <summary>
    /// Получает погоду из XML-файла
    /// </summary>
    /// <param name="s">Путь к XML-файлу</param>
    /// <returns>Объект Weather с данными из файла</returns>
    /// <exception cref="runtime_error">Файл не найден или имеет неверный формат</exception>
    virtual Weather getWeather(string s) override {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(s.c_str());
        if (!result) {
            throw runtime_error("Не удалось открыть XML файл");
        }

        pugi::xml_node root = doc.child("current");

        string city = root.child("city").attribute("name").as_string();
        double lon = root.child("city").child("coord").attribute("lon").as_double();
        double lat = root.child("city").child("coord").attribute("lat").as_double();
        double temperature = root.child("temperature").attribute("value").as_double();
        string weather = root.child("weather").attribute("value").as_string();
        double windSpeed = root.child("wind").child("speed").attribute("value").as_double();
        int clouds = root.child("clouds").attribute("value").as_int();

        return Weather(city, lon, lat, temperature, weather, windSpeed, clouds);
    }
};
/// <summary>
/// Главная функция программы
/// </summary>
/// <returns>0 при успешном выполнении</returns>
int main() {
    setlocale(LC_ALL, "rus");

    cout << "=== Проверка JSON ===" << endl;
    try {
        JsonService js;
        Weather w = js.getWeather("weather.json");
        w.print();
    }
    catch (const exception& e) {
        cout << "Ошибка JSON: " << e.what() << endl;
    }

    cout << "\n=== Проверка XML ===" << endl;
    try {
        XmlService xs;
        Weather w = xs.getWeather("weather.xml");
        w.print();
    }
    catch (const exception& e) {
        cout << "Ошибка XML: " << e.what() << endl;
    }

    return 0;
}