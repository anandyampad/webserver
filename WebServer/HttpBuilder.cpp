#include "HttpBuilder.h"

HttpBuilder& HttpBuilder::addResponseAttributes(const string name, const string value)
{
    m_attributes[name]=value;
    return *this;
}
void HttpBuilder::addContent(const string content)
{
    m_content = content;
}
string HttpBuilder::str()const
{
    ostringstream oss;
    oss<<m_protocol<<"/"<<m_version<<" "
       <<m_errorcode<<" "<<"OK"<<"\r\n";
    for(auto entry:m_attributes)
    {
        oss<<entry.first<<":"<<entry.second<<"\r\n";
    }
    oss<<"\r\n";
    oss<<m_content;
    return oss.str();
}
