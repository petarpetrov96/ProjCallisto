#include "animation.h"
void Animation::init(int num) {
    texturesnum=num;
    if(textures!=NULL)
        delete[] textures;
    textures=new Texture[num];
}
void Animation::addTexture(char* filename,int id) {
    textures[id].loadPNG(filename);
}
void Animation::next() {
    curr++;
    if(curr==texturesnum)
        curr=0;
}
void Animation::prev() {
    curr--;
    if(curr==-1)
        curr=texturesnum-1;
}
void Animation::use(Program p) {
    textures[curr].use(p);
}
void Animation::destroy() {
    for(int i=0;i<texturesnum;i++)
        textures[i].destroy();
}
