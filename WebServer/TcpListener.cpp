#include "TcpListener.h"
int TcpListener::init()
{
    //Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2,2);
    int wsok = WSAStartup(ver,&wsData);
    if(wsok!=0)
    {
        cerr<<"Can't initialize winsock, Quitting!!"<<endl;
        return wsok;
    }

    //create socket
    m_socket = socket(AF_INET,SOCK_STREAM,0);
    if(m_socket==INVALID_SOCKET)
    {
        cerr<<"Can't create socket, Quitting!!"<<endl;
        return WSAGetLastError();
    }

    //bind the socket to ip address and port
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(m_port);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
//    inet_pton(AF_INET,m_ipAddress,&addr.sin_addr);
    if(bind(m_socket,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
    {
        cerr<<"Can't bind to socket, Quitting!1"<<endl;
        return WSAGetLastError();
    }

    //tell winsock socket is for listening
    if(listen(m_socket,SOMAXCONN)==SOCKET_ERROR)
    {
        cerr<<"Can't listen to socket, Quitting!!"<<endl;
        return WSAGetLastError();
    }



    for(int i=0; i<10; i++)
        m_clients[i]=INVALID_SOCKET;

    return 0;
}

int TcpListener::run()
{

    while(true)
    {
        int max_fd = m_socket;
        FD_ZERO(&m_master);

        FD_SET(m_socket,&m_master);
        for(int i=0; i<10; i++)
        {
            if(m_clients[i]!=INVALID_SOCKET)
            {
                FD_SET(m_clients[i],&m_master);
                if(m_clients[i]>max_fd)
                    max_fd = m_clients[i];
            }
        }
        select(max_fd+1,&m_master,0,0,0);
        if(FD_ISSET(m_socket,&m_master))
        {
            cout<<"new connection has arrived"<<endl;
            sockaddr_in client;
            int sizeofclient = sizeof(client);
            SOCKET clientsocket = accept(m_socket,(sockaddr*)&client,&sizeofclient);
            char host[NI_MAXHOST];
            char clientPort[NI_MAXSERV];
            ZeroMemory(host,NI_MAXHOST);
            ZeroMemory(clientPort,NI_MAXSERV);


            cout<<inet_ntoa(client.sin_addr)<<" Connected on port:"
                <<ntohs(client.sin_port)<<endl;

            FD_SET(clientsocket,&m_master);
            for(int i=0; i<10; i++)
            {
                if(m_clients[i]==INVALID_SOCKET)
                {
                    m_clients[i]=clientsocket;
                    if(clientsocket>max_fd)
                        max_fd=clientsocket;
                    break;
                }
            }
            onClientConnected(clientsocket);
        }
        else
        {
            int recvSocket=INVALID_SOCKET;
            for(int i=0; i<10; i++)
            {
                if(FD_ISSET(m_clients[i],&m_master))
                {
                    recvSocket = m_clients[i];
                    break;
                }
            }

            char buf[4096];
            ZeroMemory(buf,4096);
            int bytesReceived = recv(recvSocket,buf,4096,0);
            if(bytesReceived == 0 || bytesReceived==SOCKET_ERROR)
            {
                cout<<"Client disconnected!!"<<endl;
                onClientDisconnected(recvSocket);
                //close socket
                for(int i=0; i<10; i++)
                {
                    if(m_clients[i]==recvSocket)
                    {
                        m_clients[i]=INVALID_SOCKET;
                        break;
                    }
                }
                closesocket(recvSocket);
                FD_CLR(recvSocket,&m_master);

            }
            else
            {
                onMessageReceived(recvSocket,buf,bytesReceived);
            }
        }
    }
    WSACleanup();
    return 0;
}

void TcpListener::sendToclient(int clientSock, const char* msg, int length)
{
    send(clientSock,msg,length,0);
}

void TcpListener::onClientConnected(int clientsocket) {}
void TcpListener::onClientDisconnected(int clientsocket) {}
void TcpListener::onMessageReceived(int clientsocket,const char* msg,int length)
{
    cout<<"Message Received from client:"<<string(msg,length)<<endl;
}
