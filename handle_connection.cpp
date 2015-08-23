#include <iostream>

#include <posixpp/file.hpp>
#include <posixpp/error.hpp>

#include "http.hpp"

using namespace http;

std::string uri_to_path(const std::string&);
std::string get_resource(const std::string&);
void handle_connection(Connection& conn, std::ostream& log = std::cerr)
{
    http::Request request{};
    conn.first >> request;

    http::Response response{};

    if(request.method() != method::GET && request.method() != method::HEAD)
    {
        log << "ERROR UNSUPPORTED METHOD USED\n";
        return;
    }

    std::string  body;
    http::status status_code;
    try
    {
        body        = get_resource(request.URI());
        status_code = status::ok;
    }catch(not_found&){
        body        = error_body(status::not_found);
        status_code = status::not_found;
    }catch(access_denied&){
        body        = error_body(status::forbidden);
        status_code = status::forbidden;
    }catch(std::exception& e)
    {
        body        = error_body(status::server_error);
        status_code = status::server_error;
        log << "An unknown exception was thrown.\n";
        log << ".what(): " << e.what();
    }

    response.set_status(status_code);
    if(request.method() != method::HEAD)
    {
        response.set_body(std::move(body));
    }
    
    conn.first << response;
}

std::string get_resource(const std::string& uri)
{
    try
    {
        File resource{uri_to_path(uri)};
        return resource.to_string();
    }
    catch(is_dir&)
    {
        return get_resource(uri+"/index.html");
    }
}
std::string uri_to_path(const std::string& uri)
{
    //TODO: Fix for all types of uris
    std::cout << uri << std::endl;
    if(uri.front() == '.' || uri.find("/.") != uri.npos)
    {
        throw access_denied{};
    }
    auto path = webroot + uri;
    if(path.back() == '/')
    {
        path += "index.html";
    }
    return webroot + uri;
}
