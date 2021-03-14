#pragma once

#include <windows.h>
#include <paddle.hpp>

class Game
{
public:
    Game(HWND window, Paddle* paddles);

    void tick();

private:
    HWND _window;
    Paddle* _players;
    DWORD _last_tick;

    static constexpr int frame_rate = 120;
    static constexpr int frame_time = 1000 / frame_rate;
    static constexpr int player_speed = 600;

    void update();
};
