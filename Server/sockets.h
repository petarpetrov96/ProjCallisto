namespace Sockets {
    bool startup() {
        WSAData wsaData;
        if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
            return false;
        return true;
    }
    void shutdown() {
        WSACleanup();
    }
}
bool startServer(char* port, SOCKET &ListenSocket) {
    struct addrinfo *result=NULL,*ptr=NULL,hints;
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    hints.ai_flags=AI_PASSIVE;
    if(getaddrinfo(NULL,port,&hints,&result)!=0) {
        return false;
    }
    ListenSocket=INVALID_SOCKET;
    ListenSocket=socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if(ListenSocket==INVALID_SOCKET) {
        freeaddrinfo(result);
        return false;
    }
    if(bind(ListenSocket,result->ai_addr,(int)result->ai_addrlen)==SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        return false;
    }
    freeaddrinfo(result);
    if(listen(ListenSocket,SOMAXCONN)==SOCKET_ERROR) {
        closesocket(ListenSocket);
        return false;
    }
    return true;
}
