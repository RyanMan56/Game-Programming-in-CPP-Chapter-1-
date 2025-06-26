#pragma once

#include <SDL2/SDL.h>

struct Vector2
{
    float x;
    float y;
};

class Game
{
public:
    Game();

    // Initialise the game
    bool initialise();

    // Runs the game loop until the game is over
    void runLoop();

    // Shutdown the game
    void shutdown();

private:
    // Helper functions for the game loop
    void processInput();

    void updateGame();

    void generateOutput();

    // Window created by SDL2
    SDL_Window *m_window;

    SDL_Renderer *m_renderer;

    // Number of ticks since start of game
    Uint32 m_ticks_count;

    // Game should continue to run
    bool m_is_running;

    // Pong specific
    // Direction of paddle
    int m_paddle_dir;
    // Position of paddle
    Vector2 m_paddle_pos;
    // Position of ball
    Vector2 m_ball_pos;
    // Velocity of ball
    Vector2 m_ball_vel;
};