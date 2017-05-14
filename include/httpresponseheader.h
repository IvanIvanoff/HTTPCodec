#ifndef __HTTP_RESPONSE_CODEC__
#define __HTTP_RESPONSE_CODEC__
// standard
#include <string>

// 3rd party
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <map>

// current project
#include "defs.h"

namespace httpcodec {
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
    httpcodec::ResponseHeader,
    (std::string, http_version)
    (std::string, status_code)
    (std::string, status_message)
    (httpcodec::ss_map, headers)
)

#endif //__HTTP_RESPONSE_CODEC__
