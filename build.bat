@echo off
echo Compiling...
g++ main.cpp proc.cpp ui.cpp chess.cpp mouseProc.cpp AIWork.cpp -o main.exe -O2 -lm -lgdi32
echo done.
