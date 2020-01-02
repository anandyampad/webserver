#pragma once
#include "TcpListener.h"
#define PROTOCOL "protocol"
#define PATH "path"
class WebServer : public TcpListener
{
public:
    WebServer(const char* ipaddr,int port):TcpListener(ipaddr,port) {}
protected:
    virtual void onClientConnected(int clientSocket);
    virtual void onClientDisconnected(int clientSocket);
    virtual void onMessageReceived(int clientSocket,const char* msg, int length);
};
