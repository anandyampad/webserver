#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iterator>
#include <ws2tcpip.h>
using namespace std;


class TcpListener
{
private:
    const char* m_ipAddress; //server ip address
    int m_port; //server port
    int m_socket; //server socket to listen connections
    fd_set m_master;
    int m_clients[10];
protected:
    virtual void onClientConnected(int clientsocket);
    virtual void onClientDisconnected(int clientsocket);
    virtual void onMessageReceived(int clientsocket,const char* msg,int length);
    void sendToclient(int clientSock, const char* msg, int length);
public:
    //constructor
    TcpListener(const char* ip_addr,int port):m_ipAddress(ip_addr),m_port(port) {}
    //initializes the tcp server
    int init();
    //run the tcp server
    int run();
};
