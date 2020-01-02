#include "WebServer.h"
#include "HttpBuilder.h"
#include "HttpParser.h"

void WebServer::onClientConnected(int clientSocket) {}
void WebServer::onClientDisconnected(int clientSocket) {}
void WebServer::onMessageReceived(int clientSocket,const char* msg, int length)
{
//parse http request
    string data(msg,length);
    HttpParser parser(data);

    if(parser.parse())
    {
        PrintParsedHttpRequest::print(parser);
        std::ostringstream content("<h1>404 page not found</h1>");
        int content_size = content.str().size();
        string content_type = "text/html";
        int errorcode = 404;

        //Handle GET method
        if(parser.getHttpMethod()==RequestMethod::GET)
        {
            string path = parser.getPath();
            string mimetype = parser.getMimeType();
            //handle png
            if (mimetype.compare("png")==0)
            {
                std::ifstream fin("./wwwroot"+path, std::ios::binary);
                if(fin.good())
                {
                    std::string data((std::istreambuf_iterator<char>(fin)),
                                     std::istreambuf_iterator<char>());
                    content.clear();
                    content<<data;
                    content_size = content.str().size();
                    content_type = "image/png";
                    errorcode = 200;
                }
            }
            //handle html
            else if(mimetype.compare("html")==0)
            {
                std::ifstream f("./wwwroot"+path);
                if(f.good())
                {
                    content_type = "text/html";
                    content.clear();
                    string line;
                    while(getline(f,line))
                        content<<line;
                    content_size = content.str().size();
                    errorcode = 200;
                }
            }
        }
        //Handle POST method

        //build http response
        HttpBuilder builder("HTTP","1.1",errorcode);
        builder.addResponseAttributes("Cache-Control","no-cache private")
        .addResponseAttributes("Content-Length",to_string(content_size))
        .addResponseAttributes("Content-Type",content_type)
        .addContent(content.str());

        string result = builder.str();

        //send http response to client
        sendToclient(clientSocket,result.c_str(),result.size()+1);
    }
}
