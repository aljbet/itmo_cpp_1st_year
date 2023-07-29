#include "weather.h"

std::vector<std::string> get_weather_from_code(int code) {
    if (code == 0) {
        return kClearSky;
    } else if (1 <= code && code <= 3) {
        return kOvercast;
    } else if (code == 45 || code == 48) {
        return kFog;
    } else if (51 <= code && code <= 57) {
        return kDrizzle;
    } else if (61 <= code && code <= 67) {
        return kRain;
    } else if (71 <= code && code <= 75) {
        return kSnowFall;
    } else if (code == 77) {
        return kSnowGrains;
    } else if (80 <= code && code <= 82) {
        return kRainShowers;
    } else if (code == 85 || code == 86) {
        return kSnowShowers;
    } else if (code == 95 || code == 96 || code == 99) {
        return kThunderstorm;
    } else {
        throw "wrong weathercode";
    }
}

std::string get_description_from_code(int code) {
    if (code == 0) {
        return "Clear sky";
    } else if (1 <= code && code <= 3) {
        return "Overcast";
    } else if (code == 45 || code == 48) {
        return "Fog";
    } else if (51 <= code && code <= 57) {
        return "Drizzle";
    } else if (61 <= code && code <= 67) {
        return "Rain";
    } else if (71 <= code && code <= 75) {
        return "Snowfall";
    } else if (code == 77) {
        return "Snow grains";
    } else if (80 <= code && code <= 82) {
        return "Rain showers";
    } else if (code == 85 || code == 86) {
        return "Snow showers";
    } else if (code == 95 || code == 96 || code == 99) {
        return "Thunderstorm";
    }
    throw std::invalid_argument("wrong weather code");
}

json get_separated_days(const std::string& text, int k_days) {
    json new_j = json::parse(text);
    const std::initializer_list<std::string> param = {"temperature_2m", "relativehumidity_2m", "apparent_temperature",
                                                      "weathercode", "precipitation", "visibility", "windspeed_10m",
                                                      "winddirection_10m"};

    const std::unordered_map<std::string, int> times = {{"morning", 6},
                                                        {"day",     12},
                                                        {"evening", 18},
                                                        {"night",   22}};
    json ans;
    for (int i = 0; i < k_days; ++i) {
        std::string day_number = "day " + std::to_string(i + 1);
        std::string time = new_j["hourly"]["time"][24 * i];
        ans[day_number]["date"] = time.substr(0, 10);
        for (const std::string& str: param) {
            for (std::pair<std::string, int> pair: times) {
                auto t = new_j["hourly"][str][kHoursPerDay * i + pair.second];
                if (str == "weathercode") {
                    ans[day_number][pair.first][str] = get_weather_from_code(t);
                    ans[day_number][pair.first]["description"] = get_description_from_code(t);
                } else {
                    ans[day_number][pair.first][str] = t;
                }
            }
        }
    }
    std::string time = new_j["current_weather"]["time"];
    std::tm tm = {};
    std::istringstream ss(time);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M");
    int current_time = tm.tm_hour;
    for (const std::string& str: param) {
        auto t = new_j["hourly"][str][current_time];
        if (str == "weathercode") {
            ans["current_weather"][str] = get_weather_from_code(t);
            ans["current_weather"]["description"] = get_description_from_code(t);
        } else {
            ans["current_weather"][str] = t;
        }
    }
    return ans;
}

void output(const json& weather, int k_days) {
    std::cout << "Weather forecast: " << weather["city"].get<std::string>() << ", "
              << weather["country"].get<std::string>() << std::endl;
    std::vector<std::string> weather_pic = weather["current_weather"]["weathercode"];
    std::cout << weather_pic[0] << "\t" << weather["current_weather"]["description"].get<std::string>() << std::endl;
    std::cout << weather_pic[1] << "\t" << weather["current_weather"]["temperature_2m"] << "("
              << weather["current_weather"]["apparent_temperature"] << ") C\n";
    std::cout << weather_pic[2] << "\t" << weather["current_weather"]["windspeed_10m"] << " km/h\n";
    std::cout << weather_pic[3] << "\t" << weather["current_weather"]["visibility"].get<int>() / 1000 << " km\n";
    std::cout << weather_pic[4] << "\t" << weather["current_weather"]["precipitation"] << " mm | "
              << weather["current_weather"]["relativehumidity_2m"] << "%\n";
    std::cout << std::endl;

    std::initializer_list<std::string> times = {"morning", "day", "evening", "night"};
    for (int i = 0; i < k_days; ++i) {
        std::string day_number = "day " + std::to_string(i + 1);
        std::cout << kHalfSep << weather[day_number]["date"].get<std::string>() << kHalfSep << std::endl;
        std::cout << "Morning\t\t\t\t|Day\t\t\t\t|Evening\t\t\t|Night\t\t\t\t|\n";
        for (const std::string& daytime: times) {
            std::cout << weather[day_number][daytime]["weathercode"][0].get<std::string>() << "\t"
                      << weather[day_number][daytime]["description"].get<std::string>() << "\t|";
        }
        std::cout << std::endl;
        for (const std::string& daytime: times) {
            std::cout << weather[day_number][daytime]["weathercode"][1].get<std::string>() << "\t"
                      << weather[day_number][daytime]["temperature_2m"] << "("
                      << weather[day_number][daytime]["apparent_temperature"] << ") C\t|";
        }
        std::cout << std::endl;
        for (const std::string& daytime: times) {
            std::cout << weather[day_number][daytime]["weathercode"][2].get<std::string>() << "\t"
                      << weather[day_number][daytime]["windspeed_10m"] << " km/h\t|";
        }
        std::cout << std::endl;
        for (const std::string& daytime: times) {
            std::cout << weather[day_number][daytime]["weathercode"][3].get<std::string>() << "\t"
                      << weather[day_number][daytime]["visibility"].get<int>() / 1000 << " km\t\t|";
        }
        std::cout << std::endl;
        for (const std::string& daytime: times) {
            std::cout << weather[day_number][daytime]["weathercode"][4].get<std::string>() << "\t"
                      << weather[day_number][daytime]["precipitation"] << " mm | "
                      << weather[day_number][daytime]["relativehumidity_2m"] << "%\t|";
        }
        std::cout << std::endl << std::endl;
    }
}

void forecast(json& h, int cur_city_index, int k_days) {
    cpr::Response r1 = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                cpr::Header{{"X-Api-Key", h["X-Api-Key"]}},
                                cpr::Parameters{{"name", h["cities"][cur_city_index]}});
    json city = json::parse(r1.text);
    cpr::Response r2 = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                cpr::Parameters{{"latitude",        std::to_string(city[0]["latitude"].get<double>())},
                                                {"longitude",       std::to_string(city[0]["longitude"].get<double>())},
                                                {"current_weather", "true"},
                                                {"forecast_days",   std::to_string(h["forecast_days"].get<int>())},
                                                {"hourly",          h["weather parameters"]}});

    json Weather = get_separated_days(r2.text, h["forecast_days"]);
    Weather["city"] = city[0]["name"];
    Weather["country"] = city[0]["country"];
    output(Weather, k_days);
}