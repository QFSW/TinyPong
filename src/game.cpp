#include "game.hpp"

#include <cstdint>

Game::Game(HWND window, Paddle* players)
    : _window(window)
    , _players(players)
    , _last_tick(GetTickCount())
{
    _players[0] = Paddle();
    _players[1] = Paddle();
}

void Game::tick()
{
    while (GetTickCount() > _last_tick)
    {
        update();
        InvalidateRect(_window, NULL, TRUE);

        _last_tick += frame_time;
    }
}

void Game::update()
{
    for (uint8_t i = 0; i < 2; ++i)
    {
        _players[i].pos += _players[i].vel * (player_speed / frame_rate);
    }
}