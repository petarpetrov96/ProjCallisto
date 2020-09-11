#ifndef _MAP_H
#define _MAP_H
#include<atomic>
#include<stdio.h>
#include<string.h>
#include<windows.h>
#include "buffers.h"
struct BasicBlockInfo {
    int datasize=0;
    GLfloat* vertex=NULL;
    GLfloat* uv=NULL;
	GLfloat* normal=NULL;
};
//BasicBlockInfo BlockInfo[4096];
//int BlockInfoSize=0;
void loadBlockInfo();
class Chunk {
    private:
    short blocks[8][256][8];
    unsigned int data[16384];
    int datasize=0;
    int triangles=0;
    Buffer u,v,n;
    public:
    std::atomic<bool> used;
    std::atomic<int> chunkx,chunky;
    Chunk() {used=false;}
    void init(int,int);
    void load(char*,int);
    int getBlock(int x,int y,int z) {if(x>=0 && x<=7 && y>=0 && y<=255 && z>=0 && z<=7) return blocks[x][y][z]; return -1;}
    void convert();
    void draw();
    void destroy();
};
#endif // _MAP_H
