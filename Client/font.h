#ifndef _FONT_H
#define _FONT_H
#include "textures.h"
#include "buffers.h"
class Font {
    private:
    Texture t;
    Buffer v,u;
    GLfloat tr[540],uv[360];
    int nums[15];
    int num=0;
    int cursize=0;
    public:
    bool show=false;
    Font(const char*);
    void add(int);
    void rem();
    void draw(Program);
    void destroy();
    void getIP(char*,int*);
};
class Button {
    private:
    Texture t;
    Buffer v,u1,u2;
    int sw=0,sh=0;
    int bx=0,by=0,bw=0,bh=0;
    bool hover=false;
    public:
    Button(const char*,int,int,int,int);
    void setResolution(int,int);
    bool check(int,int);
    void draw(Program);
    void destroy();
};
#endif // _FONT_H
