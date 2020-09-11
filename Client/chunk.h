#ifndef _CHUNK_H
#define _CHUNK_H
#include<queue>
#include "map.h"
class ChunkManager {
    private:
    bool used[1681]={false};
    bool queued[1681]={false};
    Chunk *c;
    std::queue<int> q;
    public:
    ChunkManager();
	~ChunkManager();
    int getChunk(int,int);
    Chunk* getChunk(int);
    void draw();
    void update(int,int);
    void finish(int id) {queued[id]=false;c[id].convert();}
    int getQueueElement();
    int createNewChunk(int,int);
    void destroy();
};
#endif // _CHUNK_H
