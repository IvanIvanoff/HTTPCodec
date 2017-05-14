#ifndef PROJECT_DEFS_H
#define PROJECT_DEFS_H

// standard
#include <string>LD_LIBRARY_PATH

// 3rd party
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <map>

namespace httpcodec {
    namespace qi = boost::spirit::qi;
    typedef std::map <std::string, std::string> ss_map;

    struct ResponseHeader {
        std::string http_version;
        std::string status_code;
        std::string status_message;
        ss_map headers;
    };

    struct RequestHeader {
        std::string method;
        std::string uri;
        std::string http_version;
        ss_map headers;
    };

}
#endif //PROJECT_DEFS_H
