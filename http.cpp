#include <type_traits>

#include "../Socket/File.hpp"
#include "../Socket/error.hpp"

#include "http.hpp"

constexpr const char* web_root = "/home/vincent/www/";

namespace http
{
    static inline std::string to_string(status status_code)
    {
        return std::to_string(
          static_cast
          <std::underlying_type_t<status>>(status_code));
    } 

    std::string get_name(status status_code)
    {
        constexpr char* names[] =
        {
            //TODO
        };
        return "";
        //return names[static_cast<int>(status_code)];
    }
    std::string header(status status_code)
    {
        std::string hdr;
        hdr +=
"HTTP/1.0 " + to_string(status_code) + ' ' + get_name(status_code) + """\r\n"""
"""Server: Tiny webserver\r\n\r\n""";
        return hdr;
    }

    static inline
    std::string static_error_body(status original_status_code,
                                  status status_code)
    {
         return
            """<!doctype html>"""
            """<html>"""
            """<head>"""
              """<title>404 - Not Found</title>"""
              """<meta charset=\"utf-8\">""" 
            """</head>"""
            """<body>"""
              """<p>"""
              """A """ + to_string(original_status_code) + """ Error occured."""
              """</br>"""
              """Additional a " + to_string(status_code) + """ Error occured """
              """while searching for the """
              """error document"""
              """</p>"""
            """</body>"""
            """</html>"""; 
    }
    std::string error_body(status status_code)
    {
        try
        {
            std::string path{web_root};
            path += "error/" + to_string(status_code) + ".html";
            File error_file{path, File::RDONLY};
            return error_file.to_string();
        }
        catch(file_error& e)
        {
            status http_error;

            //TODO see tinyweb.cpp:103
            switch(e.err_num())
            {
                case EACCES: http_error = status::forbidden;
                             break;
                case EINTR:
                            http_error = status::internal_server_error;
                            break;
                case EOVERFLOW: 
                case EIO:
                case ELOOP:
                case ENAMETOOLONG: http_error = status::internal_server_error;
                                   break;
                case ENFILE:
                case EMFILE: http_error = status::internal_server_error;
                             break;
                case ENOENT: http_error = status::not_found;
                             break;
                case ENXIO:
                case ENOSR:  http_error = status::internal_server_error;
                             break;
            }
            return static_error_body(status_code, http_error);
        }
    }
}
