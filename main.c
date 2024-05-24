#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "ctype.h"

#include "raylib.h"
#include "raymath.h"

void log_float(const char* name, float f) {
  TraceLog(LOG_WARNING, TextFormat("%s: %2f", name, f));
}

char input[20] = {0};
int input_size = 0;
char current_token = '\0';
static int token_index = 0;

void next_token() {
  token_index++;
  current_token = input[token_index];
}

int parse_number() {
  int value = atoi(&current_token);
  next_token();
  return value;
}

int expression(int binding_power);

int precedence(char token) {
  switch (token) {
    case '+': return 1;
    case '*': return 2;
    default: return 0;
  }
}

int parse_expression(int left) {
  switch (current_token) {
    case '+':
      next_token();
      return left + expression(1);
    case '*':
      next_token();
      return left * expression(2);
    default: return 0;
  }
}

int expression(int min_precedence) {
  int left = 0;
  if('1' <= current_token && current_token <= '9') {
    left = parse_number();
  }

  while (precedence(current_token) > min_precedence) {
    left = parse_expression(left);
  }

  return left;
}

void clear_input(char input[20]) {
  for(int i = 0; i < 20; i++) input[i] = '\0';
}

void evaluate() {
  current_token = input[0];
  int result = expression(0);
  clear_input(input);
  input_size = 0;
  sprintf(input, "%d", result);
  
  while(input[input_size] != '\0') {
    input_size++;
  }
  token_index = 0;
}

int main() {
  const char *game_title = "Simple Calculator";
  int screen_width  = 700;
  int screen_height = 700;
  
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen_width, screen_height, game_title);
  SetTargetFPS(60);

  char buttons[4][4] = {
    {'+', '0', '.', '='},
    {'-', '7', '8', '9'},
    {'*', '4', '5', '6'},
    {'/', '1', '2', '3'},
  };
  clear_input(input);

  int tile_size = 100;

  int width_rect = 4 * 100;
  Rectangle clear_rect = {0, 4 * 100, width_rect-1, 100};

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    Vector2 mouse_position = GetMousePosition();
    int x = mouse_position.x / tile_size;
    int y = mouse_position.y / tile_size;

    if(IsKeyPressed(KEY_ZERO)) {
      input[input_size++] = '0';
    }
    if(IsKeyPressed(KEY_ONE)) {
      input[input_size++] = '1';
    }
    if(IsKeyPressed(KEY_TWO)) {
      input[input_size++] = '2';
    }
    if(IsKeyPressed(KEY_THREE)) {
      input[input_size++] = '3';
    }
    if(IsKeyPressed(KEY_FOUR)) {
      input[input_size++] = '4';
    }
    if(IsKeyPressed(KEY_FIVE)) {
      input[input_size++] = '5';
    }
    if(IsKeyPressed(KEY_SIX)) {
      input[input_size++] = '6';
    }
    if(IsKeyPressed(KEY_SEVEN)) {
      input[input_size++] = '7';
    }
    if(IsKeyPressed(KEY_EIGHT)) {
      input[input_size++] = '8';
    }
    if(IsKeyPressed(KEY_NINE)) {
      input[input_size++] = '9';
    }

    if(IsKeyPressed(KEY_PERIOD)) {
      input[input_size++] = '.';
    }

    if(IsKeyPressed(KEY_KP_ADD)) {
      input[input_size++] = '+';
    }
    if(IsKeyPressed(KEY_MINUS)) {
      input[input_size++] = '-';
    }
    if(IsKeyPressed(KEY_KP_MULTIPLY)) {
      input[input_size++] = '*';
    }
    if(IsKeyPressed(KEY_SLASH)) {
      input[input_size++] = '/';
    }

    if(IsKeyPressed(KEY_EQUAL)) {
      evaluate();
    }

    if(IsKeyPressed(KEY_BACKSPACE)) {
      input[--input_size] = '\0';
    }

    if(IsMouseButtonPressed(0)) {
      if(x < 4 && y < 4) {
        char button = buttons[y][x];
        if(button == '=') {
          evaluate();
        } else {
          input[input_size++] = button;
        }
      }

      if(CheckCollisionPointRec(mouse_position, clear_rect)) {
        clear_input(input);
        input_size = 0;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 4; j++) {
        int half_size = tile_size / 2;
        int j_size = j * tile_size;
        int i_size = i * tile_size;

        Color tile_bg = x == j && y == i ? MAGENTA : LIGHTGRAY;
        DrawRectangleRec((Rectangle){j_size, i_size, tile_size - 1, tile_size - 1}, tile_bg);

        const char* text = TextFormat("%c", buttons[i][j]);
        int half_width = MeasureText(text, 70) / 2;
        DrawText(text, j_size + half_size - half_width, i_size + half_size - 30, 70, BLACK);
      }
    }

    Color tile_bg = LIGHTGRAY;
    if(CheckCollisionPointRec(mouse_position, clear_rect)) {
      tile_bg = MAGENTA;
    }
    DrawRectangleRec(clear_rect, tile_bg);

    char* text = "CLEAR";
    int half_width = MeasureText(text, 70) / 2;
    DrawText(text, (width_rect / 2) - half_width, width_rect + 25, 70, BLACK);

    DrawText(input, 30, 700 - 70, 70, WHITE);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}