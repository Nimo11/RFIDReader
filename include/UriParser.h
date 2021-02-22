///
/// thanks to https://stackoverflow.com/questions/2616011/easy-way-to-parse-a-url-in-c-cross-platform
///

#ifndef UriParser_h
#define UriParser_h    
#include <string>
#include <algorithm>    // find

struct UriParser
{
public:
std::string QueryString, Path, Protocol, Host;
uint16_t Port;

static UriParser parseURI(const std::string &uri)
{
    UriParser result;

    typedef std::string::const_iterator iterator_t;

    if (uri.length() == 0)
        return result;

    iterator_t uriEnd = uri.end();

    // get query start
    iterator_t queryStart = std::find(uri.begin(), uriEnd, '?');

    // protocol
    iterator_t protocolStart = uri.begin();
    iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');            //"://");

    if (protocolEnd != uriEnd)
    {
        std::string prot = &*(protocolEnd);
        if ((prot.length() > 3) && (prot.substr(0, 3) == "://"))
        {
            result.Protocol = std::string(protocolStart, protocolEnd);
            protocolEnd += 3;   //      ://
        }
        else
            protocolEnd = uri.begin();  // no protocol
    }
    else
        protocolEnd = uri.begin();  // no protocol

    // host
    iterator_t hostStart = protocolEnd;
    iterator_t pathStart = std::find(hostStart, uriEnd, '/');  // get pathStart

    iterator_t hostEnd = std::find(protocolEnd, 
        (pathStart != uriEnd) ? pathStart : queryStart,
        ':');  // check for port

    result.Host = std::string(hostStart, hostEnd);

    // port
    if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))  // we have a port
    {
        hostEnd++;
        iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
        
        Serial.println(std::string(hostEnd, portEnd).c_str());
        result.Port = to_int(std::string(hostEnd, portEnd).c_str());
    }

    // path
    if (pathStart != uriEnd)
        result.Path = std::string(pathStart, queryStart);

    // query
    if (queryStart != uriEnd)
        result.QueryString = std::string(queryStart, uri.end());

    return result;

}   // Parse

private:

static int to_int(char const *s)
{
     if ( s == NULL || *s == '\0' )
        return 0;

     bool negate = (s[0] == '-');
     if ( *s == '+' || *s == '-' ) 
         ++s;

     if ( *s == '\0')
        return 0;

     int result = 0;
     while(*s)
     {
          if ( *s < '0' || *s > '9' )
            return 0;
          result = result * 10  - (*s - '0');  //assume negative number
          ++s;
     }
     return negate ? result : -result;
} 

};  // uri
#endif /* URL_HH_ */