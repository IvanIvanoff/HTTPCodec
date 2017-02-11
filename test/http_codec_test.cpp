#include <string>
#include <boost/spirit/include/qi.hpp>
#include <map>
#include "../include/httpcodec.h"
#include <chrono>
#include <iostream>

//==============================================================================================

int main()
{
    typedef std::string::const_iterator It;

    httpcodec::RequestHeaderGrammar<It> httpRequestGrammar;
    httpcodec::ResponseHeaderGrammar<It> httpResponseGrammar;

    httpcodec::RequestHeader  httpRequestHeader;
    httpcodec::ResponseHeader httpResponseHeader;

    std::string request(
            "GET www.google.com/something HTTP/1.0\r\n"
                    "Date: Sun, 28 Jul 2013 15:37:37 GMT\r\n"
                    "Server: Apache\r\n"
                    "Last-Modified: Sun, 07 Jul 2013 06:13:43 GMT\r\n"
                    "Transfer-Encoding: chunked\r\n"
                    "Connection: Keep-Alive\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n");

    std::string response(
            "HTTP/1.1 404 NotFound\r\n"
                    "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
                    "Server: Apache/2.2.14 (Win32)\r\n"
                    "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
                    "Content-Length: 88\r\n"
                    "Content-Type: text/html\r\n"
                    "Connection: Closed\r\n");

    std::cout << std::endl << "Tesing only the parsing capability without allocating grammars and strings" << std::endl;
    std::cout << "Running 10'000 parsings of request and 10'000 parsings and response..." << std::endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    int n = 10000;

    while((--n) > 0){
        It iterReq = request.begin(), endReq = request.end();
        It iterRes = response.begin(), endRes = response.end();

        bool req = phrase_parse(iterReq, endReq, httpRequestGrammar, boost::spirit::qi::ascii::blank, httpRequestHeader);
        bool res = phrase_parse(iterRes, endRes, httpResponseGrammar, boost::spirit::qi::ascii::blank, httpResponseHeader);

        if ((!req || iterReq != endReq) ||
            (!res || iterRes != endRes)) {
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \"" << std::string(iterReq, endReq) << "\n";
            break;
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Testing took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " milliseconds\n";

    return 0;
}