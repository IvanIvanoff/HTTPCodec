#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <map>

#ifndef HTTPCODEC_H
#define HTTPCODEC_H

namespace httpcodec {
    typedef std::map<std::string, std::string> ss_map;

    struct RequestHeader {
        std::string method;
        std::string uri;
        std::string http_version;
        ss_map headers;
    };

    struct ResponseHeader {
        std::string http_version;
        std::string status_code;
        std::string status_message;
        ss_map headers;
    };

    namespace qi = boost::spirit::qi;

    //==============================================================================================
    template<typename Iterator, typename Skipper = qi::ascii::blank_type>
    struct RequestHeaderGrammar : qi::grammar<Iterator, RequestHeader(), Skipper> {

        RequestHeaderGrammar() :
            RequestHeaderGrammar::base_type(http_header, "RequestHeader Grammar") {
                method      = +qi::alpha;
                uri         = +qi::graph;
                http_ver    = "HTTP/" >> +qi::char_("0-9.");
                field_key   = +qi::char_("0-9a-zA-Z-");
                field_value = +~qi::char_("\r\n");
                fields      = *(field_key >> ':' >> field_value >> qi::lexeme["\r\n"]);
                http_header = method >> uri >> http_ver >> qi::lexeme["\r\n"] >> fields;

    #ifdef DEBUG
                BOOST_SPIRIT_DEBUG_NODES((method)(uri)(http_ver)(fields)(http_header))
    #endif
    }

    private:
        qi::rule<Iterator, std::map<std::string, std::string>(), Skipper> fields;
        qi::rule<Iterator, RequestHeader(), Skipper> http_header;
        qi::rule<Iterator, std::string()> method;
        qi::rule<Iterator, std::string()> uri;
        qi::rule<Iterator, std::string()> http_ver;
        qi::rule<Iterator, std::string()> field_key;
        qi::rule<Iterator, std::string()> field_value;
    };


//==============================================================================================
    template<typename Iterator, typename Skipper = qi::ascii::blank_type>
    struct ResponseHeaderGrammar : qi::grammar<Iterator, ResponseHeader(), Skipper> {

        ResponseHeaderGrammar() :
            ResponseHeaderGrammar::base_type(http_header, "ResponseHeader Grammar") {
                http_ver       = "HTTP/" >> +qi::char_("0-9.");
                status_code    = +qi::char_("0-9");
                status_message = +qi::char_("0-9a-zA-Z- "); //allow empty spaces
                field_key      = +qi::char_("0-9a-zA-Z-");
                field_value    = +~qi::char_("\r\n");
                fields         = *(field_key >> ':' >> field_value >> qi::lexeme["\r\n"]);
                http_header    = http_ver >> status_code >> status_message >> qi::lexeme["\r\n"] >> fields;

    #ifdef DEBUG
                BOOST_SPIRIT_DEBUG_NODES((http_ver)(status_code)(status_message)(fields)(http_header))
    #endif
        }

    private:
        qi::rule<Iterator, std::map<std::string, std::string>(), Skipper> fields;
        qi::rule<Iterator, ResponseHeader(), Skipper> http_header;
        qi::rule<Iterator, std::string()> http_ver;
        qi::rule<Iterator, std::string()> status_code;
        qi::rule<Iterator, std::string()> status_message;
        qi::rule<Iterator, std::string()> field_key;
        qi::rule<Iterator, std::string()> field_value;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    httpcodec::RequestHeader,
    (std::string, method)
    (std::string, uri)
    (std::string, http_version)
    (httpcodec::ss_map, headers)
)

BOOST_FUSION_ADAPT_STRUCT(
    httpcodec::ResponseHeader,
    (std::string, http_version)
    (std::string, status_code)
    (std::string, status_message)
    (httpcodec::ss_map, headers)
)

#endif //HTTPCODEC_HTTPCODEC_H
