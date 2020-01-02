#include "HttpParser.h"

RequestMethod HttpParser::getHttpMethod()
{
    return m_method;
}
string HttpParser::getMimeType()
{
    return m_mimetype;
}
string HttpParser::getPath()
{
    return m_path;
}
string HttpParser::getVersion()
{
    return m_httpversion;
}

bool HttpParser::parse()
{
    istringstream iss(m_data);
    string first_line;
    getline(iss,first_line);
    std::istringstream iss1(first_line);
    std::string token;
    int i=0;
    while (getline(iss1, token, ' '))
    {
        switch(i)
        {
        case 0:
            if(token.compare("GET")==0)
                m_method=RequestMethod::GET;
            else if(token.compare("POST")==0)
                m_method=RequestMethod::POST;
            break;
        case 1:
            m_path=token;
            break;
        case 2:
            m_httpversion=token;
            break;
        }
        i++;
    }
    if(m_path.compare("/")==0)
        m_path ="/index.html";
    m_mimetype = m_path.substr(m_path.find('.')+1,m_path.size()-1);
    return true;
}
