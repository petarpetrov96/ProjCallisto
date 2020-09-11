#include "tcp.h"
GameTCP::GameTCP() {
    WSADATA wsaData;
    if((error=WSAStartup(MAKEWORD(2,2),&wsaData))!=0) {
        MessageBox(NULL,"There was a problem initializing the Multiplayer!","Error",MB_OK);
        return;
    }
    error=0;
    ready=0;
    ConnectSocket=INVALID_SOCKET;
}
GameTCP::~GameTCP() {
    WSACleanup();
}
bool GameTCP::connectTo(const char* ip) {
    ready=0;
    disconnect();
    struct addrinfo *result=NULL,*ptr=NULL,hints;
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    if(getaddrinfo(ip,"1024",&hints,&result)!=0) {
        error=1;
        return false;
    }
    ptr=result;
    ConnectSocket=socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
    if(ConnectSocket==INVALID_SOCKET) {
        error=1;
        return false;
    }
    if(connect(ConnectSocket,ptr->ai_addr,(int)(ptr->ai_addrlen))==SOCKET_ERROR) {
        error=1;
        return false;
    }
    freeaddrinfo(result);
    if(ConnectSocket==INVALID_SOCKET) {
        error=1;
        return false;
    }
    ready=1;
    return true;
}
bool GameTCP::login(const char* username,const char* sessionid) {
    char* command=new char[18+strlen(username)];
    command[0]=0x11;
    command[1]=strlen(username);
    memcpy(command+2,username,strlen(username));
    memcpy(command+2+strlen(username),sessionid,16);
    if(send(ConnectSocket,command,18+strlen(sessionid),0)==SOCKET_ERROR) {
        error=1;
        delete[] command;
        return false;
    }
    delete[] command;
    char recvbuf[1];
    int t;
    t=recv(ConnectSocket,recvbuf,1,0);
    if(t!=1) {
        error=1;
        return false;
    }
    if(recvbuf[0]==0x01) {
        return true;
    }
    error=1;
    return false;
}
void GameTCP::requestChunk(Chunk* c) {
    ready=0;
    int cx,cy;
    cx=c->chunkx;
    cy=c->chunky;
    char* command=new char[9];
    command[0]=0x17;
    memcpy(command+1,&cx,sizeof(int));
    memcpy(command+5,&cy,sizeof(int));
    if(send(ConnectSocket,command,9,0)==SOCKET_ERROR) {
        error=1;
        delete[] command;
        return;
    }
    delete[] command;
    char recvbuf[65536];
    int t;
    unsigned short size=0;
    t=recv(ConnectSocket,recvbuf,3,0);
    if(t!=3) {
        error=1;
        return;
    }
    if(recvbuf[0]!=0x17) {
        error=1;
        return;
    }
    memcpy(&size,recvbuf+1,2);
    t=recv(ConnectSocket,recvbuf,size*4,0);
    if(t!=size*4) {
        error=1;
        return;
    }
    c->used=true;
    c->load(recvbuf,size);
    //c->convert();
    c->used=false;
    ready=1;
}
void GameTCP::disconnect() {
    closesocket(ConnectSocket);
}
