#include "lib/weather.h"
#include <conio.h>

using json = nlohmann::json;

int main() {
    std::ifstream f("../bin/configuraton.json");
    json h = json::parse(f);
    int cur_city_index = 0;
    int k_days = h["forecast_days"].get<int>();
    int frequency = h["frequency"].get<int>();
    forecast(h, cur_city_index, k_days);
    auto start = std::chrono::high_resolution_clock::now();
    while (!kbhit()) {
        auto cur = std::chrono::high_resolution_clock::now();
        auto cur_ans = std::chrono::duration_cast<std::chrono::milliseconds>(cur - start).count();
        if (cur_ans > frequency) {
            system("cls");
            forecast(h, cur_city_index, k_days);
            start = std::chrono::high_resolution_clock::now();
            continue;
        }
        char x = _getch();
        if (x == '+') {
            k_days++;
        } else if (x == '-') {
            k_days--;
        } else if (x == 'n') {
            cur_city_index = (cur_city_index + 1) % h["cities"].size();
        } else if (x == 'p') {
            cur_city_index = (cur_city_index - 1) % h["cities"].size();
        } else if (x == kEscapeCode) {
            return 0;
        } else continue;
        system("cls");
        forecast(h, cur_city_index, k_days);
    }

    return 0;
}