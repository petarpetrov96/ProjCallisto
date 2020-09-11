class Chunk {
    private:
    short blocks[8][256][8];
    unsigned int data[16384];
    int datasize=0;
    public:
    std::atomic<bool> used;
    int chunkx,chunky;
    Chunk() {used=false;}
    void init(int,int);
    bool load(FILE*);
    void generate();
    void save();
    int getBlock(int x,int y,int z) {return blocks[x][y][z];}
    void convertToData();
    int getConvertSize();
    void convert(char*);
};
void Chunk::init(int cx,int cy) {
    chunkx=cx;
    chunky=cy;
    FILE* f;
    char *sx,*sy;
    sx=new char[21];
    sy=new char[21];
    itoa(cx,sx,10);
    itoa(cy,sy,10);
    char filename[255];
    strcpy(filename,"chunks/");
    strcat(filename,sx);
    strcat(filename,"_");
    strcat(filename,sy);
    strcat(filename,".chk");
    f=fopen(filename,"rb");
    if(f==NULL)
        generate();
    if(!load(f))
        generate();
    fclose(f);
    convertToData();
    delete[] sx;
    delete[] sy;
}
bool Chunk::load(FILE* f) {
    short* buffer=new short[16384];
    int size;
    int pos=0;
    fread(buffer,sizeof(short),16384,f);
    fseek(f,0,SEEK_END);
    size=ftell(f);
    if(size!=32768) {
        delete[] buffer;
        return false;
    }
    for(int x=0;x<8;x++)
        for(int y=0;y<256;y++)
            for(int z=0;z<8;z++)
                blocks[x][y][z]=buffer[pos++];
    delete[] buffer;
    return true;
}
void Chunk::generate() {
    for(int x=0;x<8;x++)
        for(int y=0;y<256;y++)
            for(int z=0;z<8;z++)
                if(y>=64)
                    blocks[x][y][z]=0;
                else
                    blocks[x][y][z]=1<<4;
	int t=rand()%10;
	blocks[3][64+t][3]=1<<4;
}
void Chunk::save() {
    short* buffer=new short[16384];
    int pos=0;
    for(int x=0;x<8;x++)
        for(int y=0;y<256;y++)
            for(int z=0;z<8;z++) {
                //buffer[pos++]=(blocks[x][y][z]|0xff00)>>8;
                //buffer[pos++]=(blocks[x][y][z]|0xff);
                buffer[pos++]=blocks[x][y][z];
            }
    FILE* f;
    char *sx,*sy;
    sx=new char[21];
    sy=new char[21];
    itoa(chunkx,sx,10);
    itoa(chunky,sy,10);
    char filename[255];
    strcpy(filename,"chunks/");
    strcat(filename,sx);
    strcat(filename,"_");
    strcat(filename,sy);
    strcat(filename,".chk");
    f=fopen(filename,"wb");
    if(f==NULL) {
        delete[] buffer;
        delete[] sx;
        delete[] sy;
        return;
    }
    fwrite(buffer,sizeof(short),16384,f);
    delete[] buffer;
    delete[] sx;
    delete[] sy;
    fclose(f);
}
void Chunk::convertToData() {
    for(int x=0;x<8;x++)
        for(int y=0;y<256;y++)
            for(int z=0;z<8;z++) {
                if(blocks[x][y][z]!=0 && ((x>0 && blocks[x-1][y][z]==0) || (x<7 && blocks[x+1][y][z]==0) || (y>0 && blocks[x][y-1][z]==0) || (y<255 && blocks[x][y+1][z]==0) || (z>0 && blocks[x][y][z-1]==0) || (z<7 && blocks[x][y][z+1]==0))) {
                    data[datasize]=x&7;
                    data[datasize]<<=8;
                    data[datasize]|=y&255;
                    data[datasize]<<=3;
                    data[datasize]|=z&7;
                    data[datasize]<<=18;
                    data[datasize]|=((blocks[x][y][z]&((1<<17)-1)));
                    datasize++;
                }
            }
}
int Chunk::getConvertSize() {
    return 3+datasize*4;
}
void Chunk::convert(char* string) {
    if(string==NULL)
        return;
    unsigned short size=(unsigned short)(datasize);
    int i;
    unsigned char t1,t2,t3,t4;
    unsigned int t;
    string[0]=0x17;
    memcpy(string+1,&size,2);
    for(i=3;i<3+datasize*4;i+=4) {
        /*t=data[(i-3)/4];
        t4=t&0b11111111;
        t>>=8;
        t3=t&0b11111111;
        t>>=8;
        t2=t&0b11111111;
        t>>=8;
        t1=t&0b11111111;
        string[i]=t1;
        string[i+1]=t2;
        string[i+2]=t3;
        string[i+3]=t4;*/
        memcpy(string+i,&data[(i-3)/4],4);
    }
}
Chunk chunks[16384];
std::atomic<int> chunksize(0);
int findChunk(int cx,int cy) {
    for(int i=0;i<chunksize;i++)
        if(chunks[i].chunkx==cx && chunks[i].chunky==cy)
            return i;
    return -1;
}
