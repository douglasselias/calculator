just:
  mkdir -p build && clang -g -o build/main main.c -lraylib -lm vendor/libraylib.a && build/main

debug:
  gf2 build/main