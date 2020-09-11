class User {
    private:
    unsigned short inventory[8]={0};
    public:
    char username[255];
    float x,y,z;
    float rotx,rotz;
    SOCKET clientSocket;
    bool logged;
    time_t lastMovement;
    void loadData();
    void generateData();
    bool saveData();
    char* getInventory();
    void setInventory(char[12]);
    User(char*);
    User();
};
void User::loadData() {
    FILE* f;
    char filename[300];
    char* buffer=new char[275];
    strcpy(filename,"userdata/");
    strcat(filename,username);
    strcat(filename,".udt");
    f=fopen(filename,"rb");
    if(f==NULL) {
        generateData();
        delete[] buffer;
        return;
    }
    fread(buffer,sizeof(char),32,f);
    fseek(f,0,SEEK_END);
    int fsize=ftell(f);
    fclose(f);
    if(fsize!=32) {
        generateData();
        delete[] buffer;
        return;
    }
    char temp[12];
    cut(buffer,0,4,temp);
    x=stringToFloat(temp);
    cut(buffer,4,4,temp);
    y=stringToFloat(temp);
    cut(buffer,8,4,temp);
    z=stringToFloat(temp);
    cut(buffer,12,4,temp);
    rotx=stringToFloat(temp);
    cut(buffer,16,4,temp);
    rotz=stringToFloat(temp);
    cut(buffer,20,12,temp);
    setInventory(temp);
    clientSocket=INVALID_SOCKET;
    logged=false;
}
void User::generateData() {
    x=0.0f;
    y=64.0f;
    z=0.0f;
    rotx=0.0f;
    rotz=0.0f;
    clientSocket=INVALID_SOCKET;
    logged=false;
    lastMovement=time(0);
}
bool User::saveData() {
    FILE* f;
    char filename[300];
    char* buffer=new char[8];
    strcpy(filename,"userdata/");
    strcat(filename,username);
    strcat(filename,".udt");
    f=fopen(filename,"wb");
    printf("Saving to %s\n",filename);
    if(f==NULL) {
        delete[] buffer;
        return false;
    }
    floatToString(x,buffer);
    fwrite(buffer,sizeof(char),4,f);
    floatToString(y,buffer);
    fwrite(buffer,sizeof(char),4,f);
    floatToString(z,buffer);
    fwrite(buffer,sizeof(char),4,f);
    floatToString(rotx,buffer);
    fwrite(buffer,sizeof(char),4,f);
    floatToString(rotz,buffer);
    fwrite(buffer,sizeof(char),4,f);
    fwrite(getInventory(),sizeof(char),12,f);
    fclose(f);
    return true;
}
User::User(char* user) {
    strcpy(username,user);
    loadData();
}
User::User() {}
char* User::getInventory() {
    char* temp=new char[12];
    temp[0]=(inventory[0]&0b111111110000)>>4;
    temp[1]=((inventory[0]&0b1111)<<4)|((inventory[1]&0b111100000000)>>8);
    temp[2]=(inventory[1]&0b11111111);
    temp[3]=(inventory[2]&0b111111110000)>>4;
    temp[4]=((inventory[2]&0b1111)<<4)|((inventory[3]&0b111100000000)>>8);
    temp[5]=(inventory[3]&0b11111111);
    temp[6]=(inventory[4]&0b111111110000)>>4;
    temp[7]=((inventory[4]&0b1111)<<4)|((inventory[5]&0b111100000000)>>8);
    temp[8]=(inventory[5]&0b11111111);
    temp[9]=(inventory[6]&0b111111110000)>>4;
    temp[10]=((inventory[6]&0b1111)<<4)|((inventory[7]&0b111100000000)>>8);
    temp[11]=(inventory[7]&0b11111111);
    return temp;
}
void User::setInventory(char temp[12]) {
    inventory[0]=(temp[0]<<4)|((temp[1]&0b11110000)>>4);
    inventory[1]=((temp[1]&0b1111)<<8)|temp[2];
    inventory[2]=(temp[3]<<4)|((temp[4]&0b11110000)>>4);
    inventory[3]=((temp[4]&0b1111)<<8)|temp[5];
    inventory[4]=(temp[6]<<4)|((temp[7]&0b11110000)>>4);
    inventory[5]=((temp[7]&0b1111)<<8)|temp[8];
    inventory[6]=(temp[9]<<4)|((temp[10]&0b11110000)>>4);
    inventory[7]=((temp[10]&0b1111)<<8)|temp[11];
}
User users[255];
std::atomic<int> usersize(0);
void loadUsers() {
    char* buffer=new char[300];
    DIR* dir;
    struct dirent* ent;
    struct stat s;
    dir=opendir("userdata");
    if(dir==NULL) {
        delete[] buffer;
        return;
    }
    while((ent=readdir(dir))!=NULL) {
        strcpy(buffer,"userdata/");
        strcat(buffer,ent->d_name);
        stat(buffer,&s);
        if(!(s.st_mode&S_IFREG))
            continue;
        char temp[1024];
        cut(ent->d_name,strlen(ent->d_name)-4,4,temp);
        if(strcmp(temp,".udt")!=0)
            continue;
        cut(ent->d_name,0,strlen(ent->d_name)-4,temp);
        temp[strlen(ent->d_name)-4]='\0';
        users[usersize++]=User(temp);
    }
    closedir(dir);
}
int findUser(char* username) {
    for(int i=0;i<usersize;i++) {
        if(strcmp(users[i].username,username)==0)
            return i;
    }
    return -1;
}
