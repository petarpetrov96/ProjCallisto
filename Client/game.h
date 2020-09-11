#ifndef _GAME_H
#define _GAME_H
#define GLEW_STATIC
#include<ctime>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform.hpp>
using namespace glm;
#include<thread>
#include<queue>
#include "shaders.h"
#include "buffers.h"
#include "textures.h"
#include "texturearray.h"
#include "font.h"
#include "animation.h"
#include "tcp.h"
#include "chunk.h"
class Game {
    private:
    GLFWwindow* window=NULL;
    GLFWmonitor* monitor=NULL;
    GLuint VertexArrayID;
    GameTCP TCP;
    public:
    Game() {}
    bool init();
    void mainMenu();
    void multiplayer(char*);
    void exit();
};
#endif // _GAME_H
