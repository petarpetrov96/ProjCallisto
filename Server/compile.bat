@echo off
echo Compiling...
mkdir bin
g++ -o bin/Server.exe -std=c++11 main.cpp -lws2_32 -w
pause >nul
echo Server compiled successfully!