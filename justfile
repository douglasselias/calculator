just:
  mkdir -p build && clang -o ./build/main main.c -lraylib -lm ./vendor/libraylib.a && ./build/main