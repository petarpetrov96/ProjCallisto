# Project Callisto

This is a simple render demo written by myself back in 2015. The aim of this project is to explore the std::thread addition in C++11 and implement basic TCP networking for a small game.

It is assumed for the Client part of the application that GLEW, GLFW and GLM are placed in a particular directory Libraries, because it was easier for me to move the entire set of libraries that I use between different machines. Windows binaries are provided in case compilation fails due to the lack of dependencies. With small modifications it should be straightforward to run under Linux as well.

## Dependencies versions

* GLEW 1.11.0
* GLFW 3.0
* GLM (any version)

## How to run

1. Run compile.bat in Client/ and Server/ directories
2. Run Server/bin/Server.exe 
3. Run Client/bin/Callisto.exe, type 127.0.0.1 in the IP box and press Connect
4. Controls are the arrow keys for movement and space for flying up, gravity automatically drops the player down