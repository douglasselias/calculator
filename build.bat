@echo off

cls
where /q cl || call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

rmdir /S /Q .\build
mkdir build

pushd .\build

echo * > .gitignore

set system_libs=user32.lib shell32.lib gdi32.lib winmm.lib opengl32.lib

cl /nologo /fsanitize=address /Z7 /diagnostics:caret /MD /Wall /WX /W4 /wd4668 /wd4996 /wd4100 /wd4255 /wd5045 /wd4711 /wd4710 /wd4189 /wd4820 /wd4388 /wd4201 /wd4062 /wd4244 /I..\vendor\ ..\main.c /link ../vendor/raylib.lib %system_libs%

popd

if exist build\main.exe (
  build\main.exe
)