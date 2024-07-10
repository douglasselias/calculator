const char *game_title = "Simple Calculator";
int screen_width  = 400;
int screen_height = 600;

void init_screen() {  
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen_width, screen_height, game_title);
  SetTargetFPS(60);
}