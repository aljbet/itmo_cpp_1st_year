#include "cpr/cpr.h"
#include "nlohmann/json.hpp"

#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

const std::vector<std::string> kClearSky = {"  \\  |  /   ",
                                            "   \\ | /    ",
                                            "---(   )--- ",
                                            "   / | \\    ",
                                            "  /  |  \\   "};

const std::vector<std::string> kOvercast = {"            ",
                                           "    .--.    ",
                                           " .-(    ).  ",
                                           "(___(___)_) ",
                                           "            "};

const std::vector<std::string> kFog = {"            ",
                                      "_ ~ _ - _`- ",
                                      " - _ - _ ~ _",
                                      "` _ - ~ - _ ",
                                      "            "};

const std::vector<std::string> kDrizzle = {"    .--.    ",
                                          " .-(    ).  ",
                                          "(___(___)_) ",
                                          " `,',`,',`, ",
                                          "`,',`,',`,' "};

const std::vector<std::string> kRain = {"    .--.    ",
                                       " .-(    ).  ",
                                       "(___(___)_) ",
                                       " /   /   /  ",
                                       "/   /   /   "};

const std::vector<std::string> kSnowFall = {"    .--.    ",
                                            " .-(    ).  ",
                                            "(___(___)_) ",
                                            " *  *  *  * ",
                                            "*  *  *  *  "};

const std::vector<std::string> kSnowGrains = {"    .--.    ",
                                              " .-(    ).  ",
                                              "(___(___)_) ",
                                              " * . * . *  ",
                                              "* . * . * . "};

const std::vector<std::string> kRainShowers = {"    .--.    ",
                                               " .-(    ).  ",
                                               "(___(___)_) ",
                                               " | | | | |  ",
                                               " | | | | |  "};

const std::vector<std::string> kSnowShowers = {"    .--.    ",
                                               " .-(    ).  ",
                                               "(___(___)_) ",
                                               "* * * * * * ",
                                               " * * * * *  "};

const std::vector<std::string> kThunderstorm = {"    .--.    ",
                                               " .-(    ).  ",
                                               "(___(___)_) ",
                                               "  /_ /_     ",
                                               "   /  /     "};

const std::string kHalfSep = "___________________________________________________________";

const int kHoursPerDay = 24;

const char kEscapeCode = 27;

std::vector<std::string> get_weather_from_code(int code);

std::string get_description_from_code(int code);

json get_separated_days(const std::string& text, int k_days);

void output(const json& weather, int k_days);

void forecast(json& h, int cur_city_index, int k_days);