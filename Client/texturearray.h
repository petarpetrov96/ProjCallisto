#ifndef _TEXTUREARRAY_H
#define _TEXTUREARRAY_H
#include "lodepng.h"
#include "shaders.h"
class TextureArray {
    private:
    GLuint texture;
    public:
    TextureArray(int);
    bool loadPNG(int,const char*);
    void use(Program);
    void destroy();
};
#endif // _TEXTUREARRAY_H
