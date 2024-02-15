#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#define SPEED 6
#define FPS 60
#define LINE_WIDTH 600
#define MAX_JUMP_HEIGHT 30
#define GRAVITY 0.6
#define DROP_VELOCITY -5

typedef unsigned char u8;
typedef unsigned int u32;

typedef enum {
    STATE_INIT = 0,
    STATE_GAMEPLAY,
    STATE_PAUSE,
    STATE_GAMEOVER,
} State;

typedef struct
{
    Vector2 position;
    int ypos;
    int frame;
    int currentFrame;
    float jumpVelocity;
    bool reachedHeight;
    bool isJumping;
    bool isFalling;
} Dinosaur;

static Dinosaur dinosaur = {0};

typedef struct
{
    int xpos[2];
    int sourceXPos[2];
    int ypos;
} Horizon;

static Horizon horizon = {0};

typedef struct
{
    State state;
    Texture imgSprite;
} Game;

static Game game = {0};

static void *memset(void *mem, u32 c, u32 n)
{
    void *result = mem;
    u8 *bytes = mem;
    while (n-- > 0) *bytes++ = c;
    return result;
}

static double getRandom()
{
    return (double)rand() / (double)RAND_MAX;

}

void update_horizon(int pos, int increment)
{
    if (game.state != STATE_GAMEPLAY)
    {
        return;
    }
    int line1 = pos;
    int line2 = pos == 0 ? 1 : 0;

    horizon.xpos[line1] -= increment;
    horizon.xpos[line2] = horizon.xpos[line1] + LINE_WIDTH;

    if (horizon.xpos[line1] <= -LINE_WIDTH)
    {
        horizon.xpos[line1] += LINE_WIDTH * 2;
        horizon.xpos[line2] = horizon.xpos[line1] - LINE_WIDTH;
        horizon.sourceXPos[line1] = (getRandom() > 0.5) ? LINE_WIDTH : 0 + 2;
    }
}

// 地平线渲染
void horizon_render(float deltaTime)
{
    int increment = SPEED * FPS * deltaTime;
    if (horizon.xpos[0] <= 0)
    {
        update_horizon(0, increment);
    } else {
        update_horizon(1, increment);
    }

    DrawTextureRec(game.imgSprite, (Rectangle){horizon.sourceXPos[0], horizon.ypos, LINE_WIDTH, 12}, (Vector2){horizon.xpos[0], 316}, WHITE);
    DrawTextureRec(game.imgSprite, (Rectangle){horizon.sourceXPos[1], horizon.ypos, LINE_WIDTH, 12}, (Vector2){horizon.xpos[1], 316}, WHITE);

}

// 仙人掌渲染
void cactus_render(void)
{

}

// 翼手龙渲染
void pterodactyl_render(void)
{

}

// 云渲染
void cloud_render(void)
{

}

void startJump()
{
    if (!dinosaur.isJumping)
    {
        dinosaur.isJumping = true;
        dinosaur.jumpVelocity = -10;
    }
}

void endJump()
{
    if (dinosaur.reachedHeight && dinosaur.jumpVelocity < DROP_VELOCITY) 
    {
        dinosaur.jumpVelocity = DROP_VELOCITY;
    }
}

void updateJump(float deltaTime)
{
    if (dinosaur.isJumping)
    {
        int framesElapsed = deltaTime * FPS;
        dinosaur.ypos += (int)(dinosaur.jumpVelocity * framesElapsed);
        dinosaur.jumpVelocity += GRAVITY * framesElapsed;
        if (dinosaur.ypos < 200)
        {
            dinosaur.reachedHeight = true;
        }
        
        if (dinosaur.ypos < 30) {
            endJump();
        }

        if (dinosaur.ypos > 280)
        {
            dinosaur.ypos = 280;
            dinosaur.isJumping = false;
            dinosaur.reachedHeight = false;
        }
    }
}

// 恐龙渲染
void dinosaur_render(float deltaTime)
{
    dinosaur.frame++;
    if (game.state == STATE_GAMEPLAY && dinosaur.frame >= (FPS / SPEED))
    {
        dinosaur.frame = 0;
        dinosaur.currentFrame++;

        if (dinosaur.currentFrame > 1)
        {
            dinosaur.currentFrame = 0;
        }
        dinosaur.position.x = dinosaur.currentFrame * 44 + 936;
    }

    if (dinosaur.isJumping)
    {
        updateJump(deltaTime);
    }

    DrawTextureRec(game.imgSprite, (Rectangle){dinosaur.position.x, dinosaur.position.y, 44, 47}, (Vector2){100, dinosaur.ypos}, WHITE);
}

void game_start()
{
    memset(&game, 0, sizeof(game));
    game.state = STATE_INIT;
    game.imgSprite = LoadTexture("game-sprites.png");

    memset(&dinosaur, 0, sizeof(dinosaur));
    dinosaur.position = (Vector2){848, 2};
    dinosaur.ypos = 280;
    dinosaur.frame = 0;
    dinosaur.currentFrame = 0;

    memset(&horizon, 0, sizeof(horizon));
    horizon.xpos[0] = 0;
    horizon.xpos[1] = LINE_WIDTH;
    horizon.sourceXPos[0] = 2;
    horizon.sourceXPos[1] = horizon.sourceXPos[0] + LINE_WIDTH;
    horizon.ypos = 54;
}

void game_keydown(int key)
{
    switch (game.state) {
        case STATE_INIT: {
            game.state = STATE_GAMEPLAY;
            break;
        }
        break;
        case STATE_GAMEPLAY: {
            switch (key)
            {
                case KEY_SPACE: {
                    startJump();
                    break;
                }
                case KEY_ENTER: {
                    printf("pause\n");
                    game.state = STATE_PAUSE;
                    break;
                }
            }
        }
        break;
        case STATE_PAUSE: {
            switch (key)
            {
            case KEY_ENTER:
                game.state = STATE_GAMEPLAY;
                break;
            case KEY_R:
                game_start();
                break;
            }
        }
        break;
        case STATE_GAMEOVER: {
            printf("game over\n");
            game_start();
            break;
        }
        break;
        default:
            break;
    }
}

void game_frame()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        game_keydown(KEY_SPACE);
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        game_keydown(KEY_ENTER);
    }

    BeginDrawing();

        float deltaTime = GetFrameTime();

        ClearBackground(RAYWHITE);

        dinosaur_render(deltaTime);
        horizon_render(deltaTime);

    EndDrawing();
}

void game_init()
{
    InitWindow(800, 600, "Dinosaur T-Rex Game");
    SetTargetFPS(FPS);

    game_start();
}

void game_over()
{
    UnloadTexture(game.imgSprite);
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