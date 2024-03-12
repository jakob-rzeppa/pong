#include "raylib.h"

const Color mainColor = GREEN;


struct Ball
{
    int x, y;
    int speedX, speedY;
    int radius;
    Color color;

    void draw(void)
    {
        DrawCircle(x, y, radius, color);
    }

    void move(void)
    {
        x += speedX * GetFrameTime();
        y += speedY * GetFrameTime();

        if (y < 0)
        {
            y = 0;
            speedY *= -1;
        }
        if (y > GetScreenHeight())
        {
            y = GetScreenHeight();
            speedY *= -1;
        }
    }
};

struct Paddle
{
    int x, y;
    int speed;
    int width, height;
    Color color;
    KeyboardKey key_up;
    KeyboardKey key_down;

    Rectangle getRect(void)
    {
        return Rectangle{ (float)x - width / 2, (float)y - height / 2, (float) width, (float) height };
    }

    void draw(void)
    {
        DrawRectangleRec(getRect(), color);
    }

    void move(void)
    {
        if (IsKeyDown(key_up))
        {
            y -= speed * GetFrameTime();
        }
        if (IsKeyDown(key_down))
        {
            y += speed * GetFrameTime();
        }

        if (y < height / 2)
        {
            y = height / 2;
        }
        if (y > GetScreenHeight() - height / 2)
        {
            y = GetScreenHeight() - height / 2;
        }
    }
};

void drawTitlescreen(int headingHeight, int headingWidth, int startbuttonHeight, int startbuttonWidth, int settingsbuttonHeight, int settingsbuttonWidth)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // ---- Heading ----
    DrawText
    (
        (const char*)"Pong",
        GetScreenWidth() / 2 - headingWidth / 2,
        GetScreenHeight() / 2 - headingHeight,
        headingHeight,
        GREEN
    );

    // ---- Startbutton ----
    DrawRectangleLines
    (
        GetScreenWidth() / 2 - startbuttonWidth * 1.1 / 2,
        GetScreenHeight() / 2 + GetScreenHeight() / 50,
        startbuttonWidth * 1.1,
        startbuttonHeight,
        GREEN
    );
    DrawText(
        (const char*)"Start",
        GetScreenWidth() / 2 - startbuttonWidth / 2,
        GetScreenHeight() / 2 + GetScreenHeight() / 50,
        startbuttonHeight,
        GREEN
    );

    // ---- Settingsbutton ----
    DrawRectangleLines
    (
        GetScreenHeight() / 30,
        GetScreenHeight() / 30,
        settingsbuttonWidth * 1.1,
        settingsbuttonHeight,
        GREEN
    );
    DrawText
    (
        (const char*)"Settings",
        GetScreenHeight() / 30 + settingsbuttonWidth * 0.05,
        GetScreenHeight() / 30,
        settingsbuttonHeight,
        GREEN
    );
    EndDrawing();
}

void drawGameStartScreen(bool gameRunning)
{
    BeginDrawing();
    ClearBackground(BLACK);
    if (!gameRunning)
    {
        DrawText("Press space to start",
            GetScreenWidth() / 2 - MeasureText("Press space to start", GetScreenHeight() / 10) / 2,
            GetScreenHeight() / 2 - GetScreenHeight() / 10 / 2,
            GetScreenHeight() / 10,
            mainColor
        );
    }
    EndDrawing();
}

void drawGameWinnerScreen(const char* winner)
{
    const char* winnerText = nullptr;
    if (winner == "left player")
    {
        winnerText = "The left player has won!";
    }
    else if (winner == "right player")
    {
        winnerText = "The right player has won!";
    }

    BeginDrawing();
        ClearBackground(BLACK);
        //Dimensions
        int headingHeight = GetScreenHeight() / 10,
            headingWidth = MeasureText(winnerText, headingHeight);
        int textHeight = GetScreenHeight() / 20,
            textWidth = MeasureText("Press space to go to the Titlescreen", textHeight);

        // ---- Heading ----
        DrawText
        (
            winnerText,
            GetScreenWidth() / 2 - headingWidth / 2,
            GetScreenHeight() / 2 - headingHeight,
            headingHeight,
            GREEN
        );

        // ---- Text ----
        DrawText
        (
            (const char*) "Press space to go to the Titlescreen",
            GetScreenWidth() / 2 - textWidth / 2,
            GetScreenHeight() / 2 + textHeight,
            textHeight,
            GREEN
        );

    EndDrawing();
}

void drawSettingsScreen(void)
{
    BeginDrawing();
        ClearBackground(BLACK);
    EndDrawing();
}


int main(void)
{
    InitWindow(1000, 700, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);

    // TODO maybe HideCursor();
    
    Ball ball;
    Paddle leftPaddle;
    Paddle rightPaddle;

    int ballspeedX = 300;
    Color maincolor = GREEN;
    KeyboardKey leftPaddleUp = KEY_W;
    KeyboardKey leftPaddleDown = KEY_S;
    KeyboardKey rightPaddleUp = KEY_UP;
    KeyboardKey rightPaddleDown = KEY_DOWN;

    bool inTitleScreen = true;
    bool inGame = false;
    bool inSettingsScreen = false;
    bool closeWindow = false;

    //TODO Fullscreen/Change Screensize

    // ---- MAIN LOOP ----
    while (!closeWindow)
    {
        // ---- TITLE SCREEN LOOP ----
        while (inTitleScreen)
        {
            if (WindowShouldClose())
            {
                closeWindow = true;
                break;
            }

            // ---- Dimensions ----
            int headingHeight = GetScreenHeight() / 5,
                headingWidth = MeasureText("Pong", headingHeight);
            int startbuttonHeight = GetScreenHeight() / 7.5,
                startbuttonWidth = MeasureText("Start", startbuttonHeight);
            int settingsbuttonHeight = GetScreenHeight() / 10,
                settingsbuttonWidth = MeasureText("Settings", settingsbuttonHeight);

            // ---- Draw ----
            drawTitlescreen(headingHeight, headingWidth, startbuttonHeight, startbuttonWidth, settingsbuttonHeight, settingsbuttonWidth);
            
            // ---- Start ----
            if (GetMouseX() > GetScreenWidth() / 2 - startbuttonWidth * 1.1 / 2 && GetMouseX() < GetScreenWidth() / 2 + startbuttonWidth * 1.1 / 2 &&
                GetMouseY() > GetScreenHeight() / 2 + GetScreenHeight() / 50 && GetMouseY() < GetScreenHeight() / 2 + GetScreenHeight() / 50 + startbuttonHeight &&
                IsMouseButtonDown(0))
            {
                inGame = true;
                inTitleScreen = false;
                break;
            }

            // ---- Settings ----
            if (GetMouseX() > GetScreenHeight() / 30 && GetMouseX() < GetScreenHeight() / 30 + settingsbuttonWidth &&
                GetMouseY() > GetScreenHeight() / 30 && GetMouseY() < GetScreenHeight() / 30 + settingsbuttonHeight &&
                IsMouseButtonDown(0))
            {
                inSettingsScreen = true;
                inTitleScreen = false;
                break;
            }
        }
        
        // ---- GAME LOOP ----
        bool gameRunning = false;
        const char* winner = nullptr;
        if (inGame)
        {
            ball.x = GetScreenWidth() / 2;
            ball.y = GetScreenHeight() / 2;
            ball.speedX = 300;
            ball.speedY = GetScreenHeight() / 2;
            ball.radius = GetScreenHeight() / 100;
            ball.color = mainColor;

            leftPaddle.x = 50;
            leftPaddle.y = GetScreenHeight() / 2;
            leftPaddle.speed = GetScreenHeight();
            leftPaddle.width = GetScreenHeight() / 50;
            leftPaddle.height = GetScreenHeight() / 6;
            leftPaddle.key_up = KEY_W;
            leftPaddle.key_down = KEY_S;
            leftPaddle.color = mainColor;

            rightPaddle.x = GetScreenWidth() - 50;
            rightPaddle.y = GetScreenHeight() / 2;
            rightPaddle.speed = GetScreenHeight();
            rightPaddle.width = GetScreenHeight() / 50;
            rightPaddle.height = GetScreenHeight() / 6;
            rightPaddle.key_up = KEY_UP;
            rightPaddle.key_down = KEY_DOWN;
            rightPaddle.color = mainColor;
        }
        while (inGame)
        {
            if (WindowShouldClose())
            {
                closeWindow = true;
                break;
            }
            if (!gameRunning && winner)
            {
                drawGameWinnerScreen(winner);
                if (IsKeyDown(KEY_SPACE))
                {
                    inTitleScreen = true;
                    inGame = false;
                    break;
                }
            }
            else if (!gameRunning)
            {
                drawGameStartScreen(gameRunning);
                if (IsKeyDown(KEY_SPACE))
                {
                    gameRunning = true;
                }
            }
            else
            {
                ball.move();
                leftPaddle.move();
                rightPaddle.move();

                float speedYMultiplier = 1, speedXMultiplier = 1.1f;
                if (CheckCollisionCircleRec(Vector2{ (float)ball.x, (float)ball.y }, ball.radius, leftPaddle.getRect()))
                {
                    if (ball.speedX < 0)
                    {
                        ball.speedX *= speedXMultiplier * -1;

                        speedYMultiplier = (ball.y - leftPaddle.y) / leftPaddle.height + 1; if (speedYMultiplier < 0)
                        {
                            speedYMultiplier *= -1;
                        }

                        ball.speedY *= speedYMultiplier;

                        if (IsKeyDown(leftPaddle.key_up) && ball.speedY > 0)
                        {
                            ball.speedY *= -1;
                        }
                        if (IsKeyDown(leftPaddle.key_down) && ball.speedY < 0)
                        {
                            ball.speedY *= -1;
                        }
                    }
                }
                if (CheckCollisionCircleRec(Vector2{ (float)ball.x, (float)ball.y }, ball.radius, rightPaddle.getRect()))
                {
                    if (ball.speedX > 0)
                    {
                        ball.speedX *= speedXMultiplier * -1;

                        speedYMultiplier = (ball.y - rightPaddle.y) / rightPaddle.height + 1;
                        if (speedYMultiplier < 0)
                        {
                            speedYMultiplier *= -1;
                        }

                        ball.speedY *= speedYMultiplier;

                        if (IsKeyDown(rightPaddle.key_up) && ball.speedY > 0)
                        {
                            ball.speedY *= -1;
                        }
                        if (IsKeyDown(rightPaddle.key_down) && ball.speedY < 0)
                        {
                            ball.speedY *= -1;
                        }
                    }
                }

                BeginDrawing();
                    ClearBackground(BLACK);
                    ball.draw();
                    leftPaddle.draw();
                    rightPaddle.draw();
                EndDrawing();

                if (ball.x < 0)
                {
                    winner = "right player";
                    gameRunning = false;
                }
                if (ball.x > GetScreenWidth())
                {
                    winner = "left player";
                    gameRunning = false;
                }
            } 
        }

        // ---- SETTINGS LOOP ----
        while (inSettingsScreen)
        {
            if (WindowShouldClose())
            {
                closeWindow = true;
                break;
            }
            drawSettingsScreen();
        }
    }
    CloseWindow();
    return 0;
}