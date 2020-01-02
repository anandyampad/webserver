#pragma once
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

enum class RequestMethod
{
    GET,
    POST
};

class HttpParser
{
    string m_data;
    RequestMethod m_method;
    string m_httpversion;
    string m_path;
    string m_mimetype;

public:
    HttpParser(string& data):m_data{data} {}
    bool parse();

    RequestMethod getHttpMethod();
    string getMimeType();
    string getPath();
    string getVersion();
};

class PrintParsedHttpRequest
{
public:
    static void print(HttpParser& parser)
    {
        string method{};
        switch(parser.getHttpMethod())
        {
        case RequestMethod::GET:
            method = "GET";
            break;
        case RequestMethod::POST:
            method = "POST";
            break;
        default:
            cout<<"Wrong Request Method"<<endl;
        }
        ostringstream oss("");
        oss<<"Http Method:"<<method<<endl;
        oss<<"Mime Type:"<<parser.getMimeType()<<endl;
        oss<<"File Path:"<<parser.getPath()<<endl;
        oss<<"Http version:"<<parser.getVersion()<<endl;
        cout<<oss.str()<<endl;
    }
};
