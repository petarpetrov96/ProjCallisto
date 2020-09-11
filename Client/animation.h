#include "textures.h"
#ifndef _ANIMATION_H
#define _ANIMATION_H
class Animation {
    private:
    Texture* textures=NULL;
    int texturesnum=0;
    int curr=0;
    public:
    Animation() {}
    void init(int);
    void addTexture(char*,int);
    void next();
    void prev();
    void use(Program);
    void destroy();
};
#endif // _ANIMATION_H
