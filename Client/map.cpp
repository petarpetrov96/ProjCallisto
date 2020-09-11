#include "map.h"
BasicBlockInfo BlockInfo[4096];
int BlockInfoSize=0;
void loadBlockInfo() {
    FILE* f;
    BlockInfoSize=2;
    f=fopen("id_1.blk","rb");
    int size;
    fread(&size,sizeof(int),1,f);
    BlockInfo[1].datasize=size;
    BlockInfo[1].vertex=new GLfloat[size];
    fread(BlockInfo[1].vertex,sizeof(float),size,f);
    BlockInfo[1].uv=new GLfloat[size];
    fread(BlockInfo[1].uv,sizeof(float),size,f);
	BlockInfo[1].normal=new GLfloat[size];
	fread(BlockInfo[1].normal,sizeof(float),size,f);
    fclose(f);
}
void Chunk::init(int cx,int cy) {
    chunkx=cx;
    chunky=cy;
}
void Chunk::load(char* buffer,int size) {
    datasize=size;
    for(int i=0;i<size;i++)
        memcpy(data+i,buffer+i*4,4);
}
void Chunk::convert() {
    int x,y,z;
    unsigned short split1,split2;
    for(x=0;x<8;x++)
        for(y=0;y<256;y++)
            for(z=0;z<8;z++)
                blocks[x][y][z]=0;
    int dtasize=0;
    for(int i=0;i<datasize;i++) {
        split2=data[i]&0xffff;
        if((split2>>4)>=BlockInfoSize)
            continue;
        dtasize+=BlockInfo[split2>>4].datasize;
    }
    GLfloat *vertex=new GLfloat[dtasize];
    GLfloat *uv=new GLfloat[dtasize];
	GLfloat *normal=new GLfloat[dtasize];
    int cur=0;
    for(int i=0;i<datasize;i++) {
        split1=(data[i]&0xffff0000)>>16;
        split2=data[i]&0xffff;
        x=(split1&0b1110000000000000)>>13;
        y=(split1&0b1111111100000)>>5;
        z=(split1&0b11100)>>2;
        blocks[x][y][z]=split2;
        memcpy(vertex+cur,BlockInfo[split2>>4].vertex,BlockInfo[split2>>4].datasize*sizeof(GLfloat));
        memcpy(uv+cur,BlockInfo[split2>>4].uv,BlockInfo[split2>>4].datasize*sizeof(GLfloat));
		memcpy(normal+cur,BlockInfo[split2>>4].normal,BlockInfo[split2>>4].datasize*sizeof(GLfloat));
        for(int j=cur;j<cur+BlockInfo[split2>>4].datasize;j+=3) {
            vertex[j]+=(x+chunkx*8);
            vertex[j+1]+=y;
            vertex[j+2]+=(z+chunky*8);
        }
        cur+=BlockInfo[split2>>4].datasize;
    }
    v.saveData(vertex,dtasize);
    u.saveData(uv,dtasize);
	n.saveData(normal,dtasize);
    triangles=dtasize/3;
    delete[] vertex;
    delete[] uv;
	delete[] normal;
}
void Chunk::draw() {
    if(used)
        return;
    v.use(0,3);
    u.use(1,3);
	n.use(2,3);
    glDrawArrays(GL_TRIANGLES,0,triangles);
    v.free(0);
    u.free(0);
	n.free(0);
}
void Chunk::destroy() {
    u.destroy();
    v.destroy();
	n.destroy();
}
