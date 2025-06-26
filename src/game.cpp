#include <iostream>
#include "game.h"

constexpr int THICKNESS = 15;
constexpr float PADDLE_H = 100.0f;
constexpr float PADDLE_SPEED = 300.0f;
constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

Game::Game() : m_window{nullptr}, m_ticks_count{0}, m_is_running{true}, m_paddle_dir{0} {};

bool Game::initialise()
{
    // Initialise SDL
    int sdl_result = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_result != 0)
    {
        SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 1)",
        100,           // top-left x-coordinate of the window
        100,           // top-left y-coordinate of the window
        WINDOW_WIDTH,  // width of the window
        WINDOW_HEIGHT, // height of the window
        0              // flags (0 for no flags set)
    );
    if (!m_window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    m_paddle_pos.x = 10.0f;
    m_paddle_pos.y = WINDOW_HEIGHT / 2.0f;
    m_ball_pos.x = WINDOW_WIDTH / 2.0f;
    m_ball_pos.y = WINDOW_HEIGHT / 2.0f;
    m_ball_vel.x = -200.0f;
    m_ball_vel.y = 235.0f;

    return true;
}

void Game::runLoop()
{
    while (m_is_running)
    {
        processInput();
        updateGame();
        generateOutput();
    }
}

void Game::processInput()
{
    SDL_Event event;

    // While there are still events in the queue
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_is_running = false;
            break;
        }
    }

    // Get state of keyboard
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
        m_is_running = false;
    }

    // Update paddle direction based on W and S keys
    m_paddle_dir = 0;
    if (state[SDL_SCANCODE_W])
    {
        m_paddle_dir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        m_paddle_dir += 1;
    }
}

void Game::updateGame()
{
    // Wait until 16ms has elapsed since last frame (this caps the frame rate to 60FPS)
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticks_count + 16))
        ;
    // Delta time is the number of ticks since last frame (converted to seconds)
    float delta_time = (SDL_GetTicks() - m_ticks_count) / 1000.0f;
    // Clamp maximum delta time value (e.g. so the delta time doesn't get too high when stepping through breakpoints)
    if (delta_time > 0.05f)
    {
        delta_time = 0.05f;
    }
    // Update ticks count (for next frame)
    m_ticks_count = SDL_GetTicks();

    // Update paddle position based on direction
    if (m_paddle_dir != 0)
    {
        m_paddle_pos.y += m_paddle_dir * PADDLE_SPEED * delta_time;
        // Make sure paddle doesn't move off screen
        if (m_paddle_pos.y < (PADDLE_H / 2.0f + THICKNESS))
        {
            m_paddle_pos.y = PADDLE_H / 2.0f + THICKNESS;
        }
        else if (m_paddle_pos.y > (WINDOW_HEIGHT - PADDLE_H / 2.0f - THICKNESS))
        {
            m_paddle_pos.y = WINDOW_HEIGHT - PADDLE_H / 2.0f - THICKNESS;
        }
    }

    // Update ball position based on ball velocity
    m_ball_pos.x += m_ball_vel.x * delta_time;
    m_ball_pos.y += m_ball_vel.y * delta_time;

    // Bounce if needed
    // Did the ball collide with the paddle?
    float diff = m_paddle_pos.y - m_ball_pos.y;
    // Take absolute value of difference
    diff = (diff > 0.0f) ? diff : -diff;
    if (
        // Our y-difference is small enough
        diff <= PADDLE_H / 2.0f &&
        // We are in the correct x-position
        m_ball_pos.x <= 25.0f && m_ball_pos.x >= 20.0f &&
        // The ball is moving to the left
        m_ball_vel.x < 0.0f)
    {
        m_ball_vel.x *= -1.0f;
    }
    // Did the ball go off screen? If so, end game
    else if (m_ball_pos.x <= 0.0f)
    {
        m_is_running = false;
    }
    // Did the ball collide with the right wall?
    else if (m_ball_pos.x >= (WINDOW_WIDTH - THICKNESS * 1.5f) && m_ball_vel.x > 0.0f)
    {
        m_ball_vel.x *= -1.0f;
    }

    // Did the ball collide with the top wall?
    if (m_ball_pos.y <= THICKNESS * 1.5f && m_ball_vel.y < 0.0f)
    {
        m_ball_vel.y *= -1.0f;
    }
    // Did the ball collide with the bottom wall?
    else if (m_ball_pos.y >= (WINDOW_HEIGHT - THICKNESS * 1.5f) && m_ball_vel.y > 0.0f)
    {
        m_ball_vel.y *= -1.0f;
    }
}

void Game::generateOutput()
{
    SDL_SetRenderDrawColor(m_renderer, 117, 142, 79, 255);
    SDL_RenderClear(m_renderer); // Clear the back buffer

    // ---- Rendering ----

    SDL_SetRenderDrawColor(m_renderer, 99, 55, 44, 255);
    // Draw top wall
    SDL_Rect wall{
        0,            // x
        0,            // y
        WINDOW_WIDTH, // width
        THICKNESS     // height
    };
    SDL_RenderFillRect(m_renderer, &wall);

    // Draw bottom wall
    wall.y = WINDOW_HEIGHT - THICKNESS;
    SDL_RenderFillRect(m_renderer, &wall);

    // Draw right wall
    wall.x = WINDOW_WIDTH - THICKNESS;
    wall.y = 0;
    wall.w = THICKNESS;
    wall.h = WINDOW_WIDTH;
    SDL_RenderFillRect(m_renderer, &wall);

    SDL_SetRenderDrawColor(m_renderer, 240, 226, 231, 255);

    // Draw paddle
    SDL_Rect paddle{
        static_cast<int>(m_paddle_pos.x - THICKNESS / 2),
        static_cast<int>(m_paddle_pos.y - PADDLE_H / 2),
        THICKNESS,
        static_cast<int>(PADDLE_H),
    };
    SDL_RenderFillRect(m_renderer, &paddle);

    // Draw ball
    SDL_Rect ball{
        static_cast<int>(m_ball_pos.x - THICKNESS / 2),
        static_cast<int>(m_ball_pos.y - THICKNESS / 2),
        THICKNESS,
        THICKNESS,
    };
    SDL_RenderFillRect(m_renderer, &ball);

    // ---- Rendering ----

    SDL_RenderPresent(m_renderer); // Swaps front and back buffers
}

void Game::shutdown()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}