#include "WebServer.h"
int main()
{
    WebServer webserver("0.0.0.0",8080);
    if(webserver.init()!=0)
        return 0;
    webserver.run();
    return 0;
}
