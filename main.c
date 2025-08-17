#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>

#include "raylib.h"
#include "raymath.h"

#include "text.c"
#include "parser.c"

int main() {
  int screen_width  = 400;
  int screen_height = 600;

  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen_width, screen_height, "Simple Calculator");
  SetTargetFPS(60);

  init_font();

  Color default_tile_bg = (Color){19, 53, 74, 255};
  Color hover_tile_bg   = (Color){};
  hover_tile_bg.r = default_tile_bg.r * 0.5;
  hover_tile_bg.g = default_tile_bg.g * 0.5;
  hover_tile_bg.b = default_tile_bg.b * 0.5;
  hover_tile_bg.a = 255;
  
  char buttons[4][4] =
  {
    {'+', '0', '<', '='},
    {'-', '7', '8', '9'},
    {'*', '4', '5', '6'},
    {'/', '1', '2', '3'},
  };
  clear_input();

  int tile_size  = 100;
  int width_rect = screen_width;
  Rectangle clear_rect = {0, 4 * tile_size, width_rect, tile_size};

  int keys[] =
  {
    KEY_ZERO,
    KEY_ONE,
    KEY_TWO,
    KEY_THREE,
    KEY_FOUR,
    KEY_FIVE,
    KEY_SIX,
    KEY_SEVEN,
    KEY_EIGHT,
    KEY_NINE,
  };
  int NUMPAD_SIZE = 10;

  while (!WindowShouldClose())
  {
    float dt = GetFrameTime();
    Vector2 mouse_position = GetMousePosition();
    int x = mouse_position.x / tile_size;
    int y = mouse_position.y / tile_size;

    for(int i = 0; i < NUMPAD_SIZE; i++)
    {
      if(IsKeyPressed(keys[i]))
      {
        input[input_size++] = i + '0';
      }
    }

    if(IsKeyPressed(KEY_KP_ADD))
    {
      input[input_size++] = '+';
    }

    if(IsKeyPressed(KEY_MINUS))
    {
      input[input_size++] = '-';
    }

    if(IsKeyPressed(KEY_KP_MULTIPLY))
    {
      input[input_size++] = '*';
    }

    if(IsKeyPressed(KEY_SLASH))
    {
      input[input_size++] = '/';
    }

    if(IsKeyPressed(KEY_EQUAL))
    {
      evaluate();
    }

    if(IsKeyPressed(KEY_BACKSPACE))
    {
      input[--input_size] = '\0';
    }

    if(IsMouseButtonPressed(0))
    {
      if(x < 4 && y < 4)
      {
        char button = buttons[y][x];
        if(button == '=') {
          evaluate();
        }
        else if (button == '<')
        {
          input[--input_size] = '\0';
        }
        else
        {
          input[input_size++] = button;
        }
      }

      if(CheckCollisionPointRec(mouse_position, clear_rect))
      {
        clear_input();
      }
    }

    if(input_size < 0) input_size = 0;

    BeginDrawing();
    ClearBackground(BLACK);

    for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 4; j++)
      {
        int half_size = tile_size / 2;
        int j_size = j * tile_size;
        int i_size = i * tile_size;

        Rectangle tile_rect = {j_size, i_size, tile_size - 1, tile_size - 1};
        Color tile_bg = default_tile_bg;

        if(CheckCollisionPointRec(mouse_position, tile_rect))
        {
          tile_bg = hover_tile_bg;
        }

        DrawRectangleRec(tile_rect, tile_bg);

        const char *text = TextFormat("%c", buttons[i][j]);
        if(buttons[i][j] == '<') text = "⌫";
        int half_width = measure_text(text).x / 2;
        draw_text(text, (Vector2){j_size + half_size - half_width, i_size + half_size - 30});
      }
    }

    Color tile_bg = default_tile_bg;
    if(CheckCollisionPointRec(mouse_position, clear_rect))
    {
      tile_bg = hover_tile_bg;
    }
    DrawRectangleRec(clear_rect, tile_bg);

    char *text = "CLEAR";
    Vector2 text_size = measure_text(text);
    int half_width  = text_size.x / 2;
    int half_height = text_size.y / 4; // TODO: Not sure why 4 is the correct value.
    draw_text(text, (Vector2){(width_rect / 2) - half_width, clear_rect.y + half_height});

    draw_text(input, (Vector2){30, screen_height - 70});

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
