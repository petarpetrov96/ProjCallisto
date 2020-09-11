#include<winsock2.h>
#define _WIN32_WINNT 0x501
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<thread>
#include<atomic>
#include<dirent.h>
#include<sys/stat.h>
#include<time.h>
#include "sockets.h"
#include "strings.h"
#include "users.h"
#include "map.h"
std::atomic<bool> quit(false);
std::atomic<bool> usedConnection[255];
std::atomic<bool> freeConnection[255];
std::atomic<int> MAXCONN;
void client(SOCKET,char*,int);
void acceptConnections(SOCKET ListenSocket) {
    std::thread connections[255];
    struct sockaddr_in client_info={0};
    int addrsize=sizeof(client_info);
    int i;
    bool fl;
    while(1) {
        SOCKET ClientSocket=INVALID_SOCKET;
        ClientSocket=accept(ListenSocket,(struct sockaddr*)&client_info,&addrsize);
        if(ClientSocket==INVALID_SOCKET) {
            printf("Could not accept client!\n");
            closesocket(ListenSocket);
            break;
        }
        if(quit==true)
            break;
        char* ip;
        ip=inet_ntoa(client_info.sin_addr);
        fl=false;
        for(i=0;i<MAXCONN;i++) {
            if(freeConnection[i]==true) {
                if(connections[i].joinable())
                    connections[i].join();
                freeConnection[i]=false;
            }
        }
        for(i=0;i<MAXCONN;i++) {
            if(usedConnection[i]==false) {
                fl=true;
                connections[i]=std::thread(client,ClientSocket,ip,i);
                usedConnection[i]=true;
                break;
            }
        }
        if(fl==false)
            closesocket(ClientSocket);
    }
    closesocket(ListenSocket);
    for(i=0;i<MAXCONN;i++) {
        if(usedConnection[i]==true || freeConnection[i]==true)
            connections[i].join();
    }
}
void client(SOCKET ClientSocket,char* ip,int id) {
    char recvbuf[1024];
    char* sendbuf;
    char sessionid[20];
    char temp[1024];
    sendbuf=new char[65536];
    int t;
    int userid=-1;
    while(1) {
        if(quit==true)
            break;
        t=recv(ClientSocket,recvbuf,1,0);
        if(t!=1)
            break;
        if(recvbuf[0]==0x10) {
            sendbuf[0]=0x10;
            sendbuf[1]=0x00;
            send(ClientSocket,sendbuf,2,0);
            printf("Ping successful from %s!\n",ip);
            break;
        }
        else if(recvbuf[0]==0x11) {
            t=recv(ClientSocket,recvbuf,1,0);
            if(t<=0)
                break;
            int l=(unsigned char)(recvbuf[0]);
            t=recv(ClientSocket,recvbuf,l+16,0);
            if(t!=l+16)
                break;
            cut(recvbuf,0,l,temp);
            temp[l]='\0';
            userid=findUser(temp);
            if(userid==-1) {
                users[usersize++]=User(temp);
                userid=usersize-1;
            }
            if(users[userid].logged==true) {
                printf("User %s already logged!\n",users[userid].username);
                userid=-1;
                break;
            }
            users[userid].logged=true;
            users[userid].clientSocket=ClientSocket;
            cut(recvbuf,l,16,sessionid);
            sessionid[16]='\0';
            printf("Logged in as %s\nSession id: %s\nIP: %s\n",users[userid].username,sessionid,ip);
            sendbuf[0]=0x01;
            if(send(ClientSocket,sendbuf,1,0)==SOCKET_ERROR)
                break;
        }
        else if(recvbuf[0]==0x12) {
            if(userid==-1)
                break;
            sendbuf[0]=0x12;
            char temp[4];
            floatToString(users[userid].x,temp);
            append(sendbuf,temp,1,4);
            floatToString(users[userid].y,temp);
            append(sendbuf,temp,5,4);
            floatToString(users[userid].z,temp);
            append(sendbuf,temp,9,4);
            floatToString(users[userid].rotx,temp);
            append(sendbuf,temp,13,4);
            floatToString(users[userid].rotz,temp);
            append(sendbuf,temp,17,4);
            if(send(ClientSocket,sendbuf,21,0)==SOCKET_ERROR)
                break;
            printf("DATA SENT\n");
        }
        else if(recvbuf[0]==0x13) {
            if(userid==-1)
                break;
            t=recv(ClientSocket,recvbuf,20,0);
            if(t!=20)
                break;
            float tx,ty,tz,trotx,trotz;
            double d;
            time_t now0;
            double diff;
            cut(recvbuf,0,4,temp);
            tx=stringToFloat(temp);
            cut(recvbuf,4,4,temp);
            ty=stringToFloat(temp);
            cut(recvbuf,8,4,temp);
            tz=stringToFloat(temp);
            cut(recvbuf,12,4,temp);
            trotx=stringToFloat(temp);
            cut(recvbuf,16,4,temp);
            trotz=stringToFloat(temp);
            sendbuf[0]=0x13;
            d=sqrt((tx-users[userid].x)*(tx-users[userid].x)+(ty-users[userid].y)*(ty-users[userid].y)+(tz-users[userid].z)*(tz-users[userid].z));
            now0=time(0);
            diff=difftime(now0,users[userid].lastMovement);
            if(d>10 || d/diff>2.0f)
                sendbuf[1]=0x00;
            else {
                sendbuf[0]=0x01;
                users[userid].x=tx;
                users[userid].y=ty;
                users[userid].z=tz;
                users[userid].rotx=trotx;
                users[userid].rotz=trotz;
            }
            if(send(ClientSocket,sendbuf,2,0)==SOCKET_ERROR)
                break;
            printf("Movement successful!\nNew coordinates: %f %f %f\n",users[userid].x,users[userid].y,users[userid].z);
        }
        else if(recvbuf[0]==0x14) {
            if(userid==-1)
                break;
            memcpy(sendbuf,users[userid].getInventory(),12);
            if(send(ClientSocket,sendbuf,12,0)==SOCKET_ERROR)
                break;
            printf("Inventory sent!\n");
        }
        //Mobs protocol
        //Objects protocol
        else if(recvbuf[0]==0x17) {
            t=recv(ClientSocket,recvbuf,8,0);
            if(t!=8)
                break;
            unsigned int cx,cy;
            memcpy(&cx,recvbuf,4);
            memcpy(&cy,recvbuf+4,4);
            int cid=findChunk(cx,cy);
            if(cid==-1) {
                chunks[chunksize++].init(cx,cy);
                cid=chunksize-1;
            }
            int size=chunks[cid].getConvertSize();
            chunks[cid].convert(sendbuf);
            if(send(ClientSocket,sendbuf,size,0)==SOCKET_ERROR)
                break;
            printf("Chunk sent successfully!\n");

        }
    }
    delete[] sendbuf;
    closesocket(ClientSocket);
    if(userid!=-1) {
        users[userid].logged=false;
        users[userid].clientSocket=INVALID_SOCKET;
        printf("User %s logged out!\n",users[userid].username);
    }
    usedConnection[id]=false;
    freeConnection[id]=true;
}
int main() {
	srand(time(0));
    //User Data
    CreateDirectory("userdata",NULL);
    printf("Loading user data...\n");
    loadUsers();
    for(int ttt=0;ttt<usersize;ttt++) {
        printf("%s\n",users[ttt].username);
    }
    //Chunks
    CreateDirectory("chunks",NULL);
    printf("Loading chunks...\n");
    chunks[0].init(0,0);
    //Server data
    printf("Starting server...\n");
    quit=false;
    std::thread listen;
    SOCKET ListenSocket;
    if(!Sockets::startup())
        return 0;
    if(!startServer("1024",ListenSocket)) {
        printf("There was a problem while starting the server: %d\n",WSAGetLastError());
        Sockets::shutdown();
        return 0;
    }
    MAXCONN=20;
    listen=std::thread(acceptConnections,ListenSocket);
    char t[255];
    printf("Server started successfully!\n");
    while(1) {
        scanf("%s",t);
        if(strcmp(t,"exit")==0) {
            quit=true;
            closesocket(ListenSocket);
            break;
        }
        else {
            printf("Unrecognised command!\n");
            printf("Please type help for more commands!\n");
        }
    }
    listen.join();
    printf("Saving chunks...\n");
    for(int i=0;i<chunksize;i++)
        chunks[i].save();
    printf("Saving user data...\n");
    for(int i=0;i<usersize;i++)
        users[i].saveData();
    printf("Shutting down server...\n");
    Sockets::shutdown();
    return 0;
}
