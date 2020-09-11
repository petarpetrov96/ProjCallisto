#ifndef _TCP_H
#define _TCP_H
#include<winsock2.h>
#include<ws2tcpip.h>
#include<atomic>
#include "map.h"
class GameTCP {
    private:
    std::atomic<int> error;
    std::atomic<bool> ready;
    std::atomic<SOCKET> ConnectSocket;
    public:
    GameTCP();
    ~GameTCP();
    int getError() {return error;}
    bool isReady() {return ready;}
    bool connectTo(const char*);
    bool login(const char*,const char*);
    void requestChunk(Chunk*);
    void disconnect();
};
#endif // _TCP_H
