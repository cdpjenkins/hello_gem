#ifndef MANDIE_SCREEN_HPP
#define MANDIE_SCREEN_HPP

#include <cstdint>
#include <memory>

using namespace std;

constexpr int16_t SCREEN_WIDTH = 640;
constexpr int16_t SCREEN_HEIGHT = 480;

using Screen = std::array<uint16_t, SCREEN_WIDTH * SCREEN_HEIGHT>;

#endif //MANDIE_SCREEN_HPP
