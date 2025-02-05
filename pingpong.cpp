#include <iostream>
#include "raylib.h"

int cpu_score {0}; // AI player score
int player_score {0}; // Human player score

class Ball {
public:
    float x, y;
    float radius;
    int speed_x, speed_y;

    Ball(float startX, float startY, float r, int sx, int sy) 
        : x(startX), y(startY), radius(r), speed_x(sx), speed_y(sy) {}

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update(int screenWidth, int screenHeight) {
        x += speed_x;
        y += speed_y;

        
        if (y + radius >= screenHeight || y - radius <= 0) {
            speed_y *= -1;
        }

        
        if (x + radius >= screenWidth) {
            cpu_score++;
            ResetBall(screenWidth, screenHeight);
        }
        if (x - radius <= 0) {
            player_score++;
            ResetBall(screenWidth, screenHeight);
        }
    }

    void ResetBall(int screenWidth, int screenHeight) {
        x = screenWidth / 2;
        y = screenHeight / 2;

        int dirX = (GetRandomValue(0, 1) == 0) ? 1 : -1;
        int dirY = (GetRandomValue(0, 1) == 0) ? 1 : -1;

        speed_x = 7 * dirX;  
        speed_y = 7 * dirY;
    }
};

class Paddle {
public:
    float x, y;
    float width, height;
    int speed_y;

    Paddle(float startX, float startY, float w, float h, int sy) 
        : x(startX), y(startY), width(w), height(h), speed_y(sy) {}

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(int screenHeight) {
        if (IsKeyDown(KEY_UP)) {
            y -= speed_y;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed_y;
        }

        
        if (y < 0) y = 0;
        if (y + height > screenHeight) y = screenHeight - height;
    }
};

class CpuPaddle : public Paddle {
public:
    CpuPaddle(float startX, float startY, float w, float h, int sy) 
        : Paddle(startX, startY, w, h, sy) {}

    void Update(int ball_y, int screenHeight) {
        int reactionDelay = 3; 

        if (y + height / 2 > ball_y + reactionDelay) {
            y -= speed_y;
        } 
        else if (y + height / 2 < ball_y - reactionDelay) {
            y += speed_y;
        }

       
        if (y < 0) y = 0;
        if (y + height > screenHeight) y = screenHeight - height;
    }
};

int main() {
    std::cout << "Starting Ping Pong...\n";

    const int screenWidth = 1280; // Screen width ----> 1280 px
    const int screenHeight = 800; // Screen height ----> 800 px
    const char* title = "Ping Pong";

    InitWindow(screenWidth, screenHeight, title);
    SetTargetFPS(60);

    
    Ball ball(screenWidth / 2, screenHeight / 2, 20, 7, 7);
    Paddle playerPaddle(screenWidth - 35, screenHeight / 2 - 60, 20, 120, 6);
    CpuPaddle cpu(10, 350, 20, 120, 6);

    while (!WindowShouldClose()) {
        // Update logic
        // -------------------------------------------------
        ball.Update(screenWidth, screenHeight);
        playerPaddle.Update(screenHeight);
        cpu.Update(ball.y, screenHeight);
        // -------------------------------------------------

        // Collision detection (optimized)
        // -------------------------------------------------
        Rectangle playerRect = { playerPaddle.x, playerPaddle.y, playerPaddle.width, playerPaddle.height };
        Rectangle cpuRect = { cpu.x, cpu.y, cpu.width, cpu.height };

        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, playerRect) ||
            CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, cpuRect)) {
            ball.speed_x *= -1;
        }
        // --------------------------------------------------

        // Rendering
        // --------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        ball.Draw(); // Draw ping pong ball
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE); // Draw midpoint boundary
        cpu.Draw(); // Draw cpu paddle
        playerPaddle.Draw(); // Draw human(you!) paddle :p
        DrawText(TextFormat("%i", cpu_score), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

        EndDrawing();
        // ---------------------------------------------------
    }

    CloseWindow();
    return 0;
}
