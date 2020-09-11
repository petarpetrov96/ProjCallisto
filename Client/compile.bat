@echo off
set INCLUDES=-I../../Libraries/glew-1.11.0/include/ -I../../Libraries/glfw-3.0.bin.WIN32/include/ -I../../Libraries/glm/
set LINKS=-L../../Libraries/glew-1.11.0/lib/Release/Win32/ -L../../Libraries/glfw-3.0.bin.WIN32/lib-mingw/
SET LINKING=-lstdc++ -lws2_32 -lglfw3 -lglew32s -lopengl32 -lglu32 -lkernel32 -luser32 -lgdi32 -mwindows
g++ -c animation.cpp -o animation.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c buffers.cpp -o buffers.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c chunk.cpp -o chunk.o -O3 -std=c++11 -Wall %INCLUDES% 
g++ -c font.cpp -o font.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c game.cpp -o game.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c lodepng.cpp -o lodepng.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c main.cpp -o main.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c map.cpp -o map.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c shaders.cpp -o shaders.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c tcp.cpp -o tcp.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c texturearray.cpp -o texturearray.o -O3 -std=c++11 -Wall %INCLUDES%
g++ -c textures.cpp -o textures.o -O3 -std=c++11 -Wall %INCLUDES%
g++ *.o -o bin/Callisto -O3 -std=c++11 -Wall %LINKS% %LINKING%
del *.o
echo Project successfully compiled!
pause >nul