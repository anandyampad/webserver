#pragma once
#include <map>
#include <string>
#include <sstream>
using namespace std;

class HttpBuilder
{
    map<string,string> m_attributes;
    string m_protocol{"HTTP"};
    string m_version{"1.1"};
    int m_errorcode{200};
    string m_content{};
public:
    HttpBuilder() {}
    HttpBuilder(string protocol,string version,int errorcode):m_protocol{protocol},m_version{version},m_errorcode{errorcode} {}
    HttpBuilder& addResponseAttributes(const string name, const string value);
    string str()const;
    void addContent(const string content);
};
