#include "raylib.h"
#include <iostream>

#include "ecs/ecs.hpp"

int main()
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    BeginDrawing();

    if (IsKeyPressed(KEY_ENTER)) {
      // make asan do an error
      std::cout << "Hello, world!" << std::endl;
      int *a = new int[10];
      a[10] = 0;
      delete[] a;
      delete[] a;
    }

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
