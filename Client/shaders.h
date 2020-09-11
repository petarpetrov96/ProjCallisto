#ifndef _SHADERS_H
#define _SHADERS_H
#include<string.h>
#define GLEW_STATIC
#include<GL/glew.h>
class Shader {
    public:
    GLuint shader;
    GLint Result=GL_FALSE;
    char error[1024];
    int length=0;
    public:
    Shader(const char*,GLenum);
    bool getResult(char*,int*);
    GLuint getShader();
};

class Program {
    private:
    GLuint shaders[64];
    int shaderl=0;
    GLuint program;
    public:
    Program();
    bool attachShader(Shader);
    void create();
    void use();
    GLint getUniformLocation(const GLchar*);
    void destroy();
};

#endif // _SHADERS_H
