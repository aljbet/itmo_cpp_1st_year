add_library(WEATHER_FORECAST weather.h weather.cpp)
target_link_libraries(WEATHER_FORECAST PRIVATE cpr::cpr)
target_link_libraries(WEATHER_FORECAST PRIVATE nlohmann_json::nlohmann_json)