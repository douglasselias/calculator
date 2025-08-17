Font font;
int font_size    = 70;
int font_spacing = 0;

void init_font()
{
  int codepoint_count = 0;
  int *codepoints = LoadCodepoints("1234567890+-/*=CLEAR⌫", &codepoint_count);
  font = LoadFontEx("FiraMono-Regular.ttf", font_size, codepoints, codepoint_count);
}

Vector2 measure_text(const char *text)
{
  return MeasureTextEx(font, text, font_size, font_spacing);
}

void draw_text(const char *text, Vector2 position)
{
  DrawTextEx(font, text, position, font_size, font_spacing, WHITE);
}