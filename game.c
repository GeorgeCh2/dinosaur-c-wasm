#include <raylib.h>
float sinf(float);
float cosf(float);
float acosf(float);
float asinf(float);
double tan(double);
float atan2f(float, float);
float sqrtf(float);
float floorf(float);
double floor(double);
float fminf(float, float);
float fmaxf(float, float);
float fabsf(float);
double fabs(double);
#include <raymath.h>

#define BALL_RADIUS 50.0
#define GRAVITY 100.0

static Vector2 ball_position = {0};
static Vector2 ball_velocity = {100, 100};

void game_frame()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    float dt = GetFrameTime();
    ball_velocity.y += GRAVITY * dt;

    float x = ball_position.x + ball_velocity.x * dt;
    if (x >= GetScreenWidth() - BALL_RADIUS || x < BALL_RADIUS)
    {
        ball_velocity.x *= -1;
    } else {
        ball_position.x = x;
    }

    float y = ball_position.y + ball_velocity.y * dt;
    if (y >= GetScreenHeight() - BALL_RADIUS || y < BALL_RADIUS)
    {
        ball_velocity.y *= -1;
    } else {
        ball_position.y = y;
    }
    DrawCircleV(ball_position, BALL_RADIUS, DARKGREEN);

    EndDrawing();
}

void game_init()
{
    InitWindow(800, 600, "Hello, Dinosaur");
    SetTargetFPS(60);

    int w = GetScreenWidth();
    int h = GetScreenHeight();
    ball_position.x = w / 2;
    ball_position.y = h / 2;
}

void game_over()
{
    CloseWindow();
}

#ifndef PLATFORM_WEB
int main()
{
    game_init();
    while (!WindowShouldClose())
    {
        game_frame();
    }

    game_over();

    return 0;
}
#endif