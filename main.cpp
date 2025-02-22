#include "raylib.h"
#include <math.h>

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1000
#define BACKGROUND_COLOR (Color){184, 186, 130}
#define CAR_MAXSPEED 1000
#define ROTATION_SPEED 8
#define DRIFT_FACTOR 0.3f
#define GRIP_FACTOR 0.98f
#define CAMERA_SMOOTHING 15.0f
#define CAMERA_ZOOM_SPEED_THRESHOLD 200
#define CAMERA_ZOOM_OUT 1.5f
#define CAMERA_ZOOM_IN 1.0f
#define MAX_SKIDMARKS 500
#define SKIDMARK_TIME 3

struct Car
{
    float x, y;
    float height, width;
    float speed;
    float rotation;
    float slipAngle;

    Texture2D carTexture;

    Rectangle GetRect() {
        return Rectangle{x, y, width, height};
    }

    Vector2 car_origin;
    Rectangle car_texture_rec;

    void DrawCar() {
        DrawTexturePro(carTexture, car_texture_rec, GetRect(), car_origin, rotation, WHITE);
    }
};

struct Skidmark
{
    float x, y;
    double time;
};


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Drag-Race -- EthicalAniruddha");
    SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    int world_width = 100000;
    int world_height = 100000;

    Skidmark skidmarks[MAX_SKIDMARKS];
    int skidmark_count = 0;

    Car car;
    car.height = 110;
    car.width = 70;
    car.x = world_width / 2.0f - car.width / 2.0f;
    car.y = world_height / 2.0f - car.height / 2.0f;
    car.speed = 0;
    car.car_origin = {car.width / 2, car.height / 2};
    car.slipAngle = 0;

    Image car_image = LoadImage("craftpix-889156-free-racing-game-kit/PNG/Car_1_Main_Positions/Car_1_01.png");
    car.carTexture = LoadTextureFromImage(car_image);
    car.car_texture_rec = {
        .x = 0,
        .y = 0,
        .width = (float)car.carTexture.width,
        .height = (float)car.carTexture.height
    };

    Image soil_image = LoadImage("craftpix-889156-free-racing-game-kit/PNG/Background_Tiles/Soil_Tile.png");
    Texture2D soilTexture = LoadTextureFromImage(soil_image);

    Camera2D camera = {
        .offset = (Vector2){0,0},
        .target = (Vector2){0,0},
        .rotation = 0,
        .zoom = 1.0
    };

    Vector2 targetCameraOffset = {0.0f, 0.0f};

    int direction = -1;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            car.speed += 5 * dt;
        } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            car.speed -= 5 * dt;
        } else {
            if (car.speed > 0) {
                car.speed -= 10 * dt;
                if (car.speed < 0) car.speed = 0;
            } else if (car.speed < 0) {
                car.speed += 10 * dt;
                if (car.speed > 0) car.speed = 0;
            }
            if (car.speed == 0) {
                if (direction == -1 && car.speed < 0) car.speed = 0;
                if (direction == 1 && car.speed < 0) car.speed -= 2.5 * dt;
            }
        }

        if (abs(car.speed) > CAR_MAXSPEED) {
            if (car.speed > 0) {
                car.speed = CAR_MAXSPEED;
            } else {
                car.speed = -CAR_MAXSPEED;
            }
        }

        float turnInput = 0;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            turnInput = -1;
        } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            turnInput = 1;
        }

        if (car.speed != 0) {
            car.rotation += turnInput * ROTATION_SPEED * dt * abs(car.speed);
            car.slipAngle = car.slipAngle * GRIP_FACTOR + turnInput * car.speed * dt * DRIFT_FACTOR;
        } else {
            car.slipAngle = 0;
        }

        if (car.rotation >= 360 || car.rotation <= -360) car.rotation = 0;

        float rad = PI * car.rotation / 180.0f;
        float slipRad = PI * car.slipAngle / 180.0f;
        float x_move = -car.speed * sinf(rad + slipRad);
        float y_move = -car.speed * cosf(rad + slipRad);

        car.x -= x_move;
        car.y += y_move;

        targetCameraOffset.x = -car.x + car.width / 2 + SCREEN_WIDTH / 2;
        targetCameraOffset.y = -car.y + car.height / 2 + SCREEN_HEIGHT / 2;

        if (targetCameraOffset.x > 0) targetCameraOffset.x = 0;
        if (targetCameraOffset.x < -world_width) targetCameraOffset.x = -world_width;

        camera.offset.x += (targetCameraOffset.x - camera.offset.x) * CAMERA_SMOOTHING * dt;
        camera.offset.y += (targetCameraOffset.y - camera.offset.y) * CAMERA_SMOOTHING * dt;

        bool drifting = abs(car.slipAngle) > 30;

        if (drifting && abs(car.speed) > 50) { // Only create skid marks when drifting and moving
            skidmarks[skidmark_count % MAX_SKIDMARKS] = (Skidmark){
                car.x + (float)(rand() % (int)car.width - car.width / 2), // Random x within car width
                car.y + (float)(rand() % (int)car.height - car.height / 2), // Random y within car height
                GetTime(),
            };
            skidmark_count++;
        }

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        BeginMode2D(camera);

        int tileCount_col = ceil((float)world_width / soilTexture.width); // Added (float) cast
        int tileCount_row = ceil((float)world_height / soilTexture.height); // Added (float) cast

        for (int i = 0; i < tileCount_row; i++) {
            for (int j = 0; j < tileCount_col; j++) {
                DrawTexture(soilTexture, j * soilTexture.width, i * soilTexture.height, WHITE);
            }
        }

        for (int i = 0; i < skidmark_count && i < MAX_SKIDMARKS; i++) {
            Skidmark skidmark = skidmarks[i];
            double current_time = GetTime();
            if (current_time - skidmark.time > SKIDMARK_TIME) {
                continue;
            }
            DrawCircle(skidmark.x, skidmark.y, 3, BLACK);
        }

        car.DrawCar();
        EndMode2D();

        Color custom_fps_color = {0, 255, 0, 255};
        DrawText(TextFormat("%d FPS", GetFPS()), 10, 10, 20, custom_fps_color);
        DrawText(TextFormat("Speed: %.2f", abs(car.speed * 10)), 100, 10, 20, custom_fps_color);
        DrawText(TextFormat("Rotation: %.2f", car.rotation), 250, 10, 20, custom_fps_color);
        DrawText(TextFormat("Slip Angle: %.2f", car.slipAngle), 450, 10, 20, custom_fps_color);
        EndDrawing();
    }

    UnloadImage(car_image);
    UnloadTexture(car.carTexture);
    UnloadImage(soil_image);
    UnloadTexture(soilTexture);
    CloseWindow();

    return 0;
}
