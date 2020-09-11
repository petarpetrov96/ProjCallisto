#ifndef _BUFFERS_H
#define _BUFFERS_H
#define GLEW_STATIC
#include<GL/glew.h>
class Buffer {
    private:
    GLuint buffer;
    public:
    Buffer();
    void saveData(void*,int);
    void use(int,int);
    void free(int);
    void destroy();
};
#endif // _BUFFERS_H
