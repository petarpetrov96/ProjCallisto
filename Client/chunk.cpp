#include "chunk.h"
void MsgBox(char* text) {
    MessageBox(NULL,text,"ERROR",MB_OK);
}
int abs(int a) {
    return (a>0)?a:(-a);
}
ChunkManager::ChunkManager() {
    c=new Chunk[1681];
}
ChunkManager::~ChunkManager() {
	delete[] c;
}
int ChunkManager::getChunk(int x,int y) {
    for(int i=0;i<1681;i++) {
        if(used[i]==true && queued[i]==false && c[i].chunkx==x && c[i].chunky==y)
            return i;
    }
    return -1;
}
Chunk* ChunkManager::getChunk(int id) {
    return &c[id];
}
void ChunkManager::draw() {
    for(int i=0;i<1681;i++)
        if(used[i]==true && queued[i]==false)
            c[i].draw();
}
void ChunkManager::update(int x,int y) {
    std::queue<int> empty;
    std::swap(q,empty);
    if(getChunk(x,y)==-1)
        q.push(createNewChunk(x,y));
    for(int i=1;i<=4;i++) {
        for(int j=1;j<=4;j++) {
            if(getChunk(x+i,y+j)==-1) q.push(createNewChunk(x+i,y+j));
            if(getChunk(x-i,y+j)==-1) q.push(createNewChunk(x-i,y+j));
            if(getChunk(x+i,y-j)==-1) q.push(createNewChunk(x+i,y-j));
            if(getChunk(x-i,y-j)==-1) q.push(createNewChunk(x-i,y-j));
            if(getChunk(x,y+j)==-1) q.push(createNewChunk(x,y+j));
            if(getChunk(x,y-j)==-1) q.push(createNewChunk(x,y-j));
            if(getChunk(x+i,y)==-1) q.push(createNewChunk(x+i,y));
            if(getChunk(x-i,y)==-1) q.push(createNewChunk(x-i,y));
        }
    }
    for(int i=0;i<1681;i++) {
        if(used[i]) {
            if(abs(c[i].chunkx-x)>8 || abs(c[i].chunky-y)>8)
                used[i]=false;
        }
    }
}
int ChunkManager::getQueueElement() {
    if(q.empty())
        return -1;
    int t=q.front();
    q.pop();
    //queued[t]=false;
    return t;
}
int ChunkManager::createNewChunk(int x,int y) {
    int id;
    for(id=0;id<1681;id++)
        if(used[id]==false)
            break;
    used[id]=true;
    queued[id]=true;
    c[id].init(x,y);
    return id;
}
void ChunkManager::destroy() {
    for(int i=0;i<1681;i++)
        c[i].destroy();
}
