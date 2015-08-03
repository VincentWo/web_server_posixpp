#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <cstring>
#include <fstream>
#include <cstdio>

#include "../Socket/Socket.hpp"
#include "../Socket/File.hpp"
#include "../Socket/error.hpp"
#include "http.hpp"

constexpr int port = 80;
constexpr char const * const web_root =  "/home/vincent/www/";
void handle_connection(Connection&); // Handle web requests
std::string get_content(std::ifstream&); // Get file content as string
bool starts_with(const std::string&, const std::string&);
using std::cout;
using std::cerr;
using std::string;
using std::endl;
int main(void)
{
    try{

        cout << "Accepting web request on port " << port << '\n';    

        Socket sock{PF_INET, SOCK_STREAM, 0};

        sock.setsockopt(SOL_SOCKET, SO_REUSEADDR);

        sock.bind(Ipv4{INADDR_ANY, 80}); 

        sock.listen(20);

        while(true)
        {
            Connection conn = sock.accept();
            handle_connection(conn);
        }
    }catch(socket_error& e)
    {
        cout << e.what() << std::endl;
        cout << e.err_num() << ": " << strerror(e.err_num());
    }
    return 0;
}

/* This function handles the connection on the passed socket from the 
 * passed client address. The connection is processed as a web request,
 * and this function replies over the connected socket. Finally, the
 * passed socket is closed at the end of the function.
 */

void handle_connection(Connection& conn)
{
    using std::cout;


    std::string request_line{conn.first.receive_line()};
    
    
    std::string::size_type index = request_line.find(" HTTP/");
    if(index == string::npos)
    {
        return;
    }
    request_line.erase(index);


    http::method request_type{};

    using namespace http;

    if(starts_with(request_line, "GET "))
    {
        request_type = method::GET;
        index = 4;
    }
    else if(starts_with(request_line, "HEAD "))
    {
        request_type = method::HEAD;
        index = 5;
    }
    else
    {
        request_type = method::UNKNOWN;
    }

    std::string relative_path{request_line.substr(index)};
    if(relative_path.find("../") != string::npos)
    {
        conn.first << header(status::forbidden);
        conn.first << error_body(status::forbidden);
    }
    std::string path{web_root + request_line.substr(index)};
    if(path.back() == '/')
    {
        path += "index.html";
    }

    try
    {
        File resource{path, File::RDONLY};

        conn.first << header(status::ok);
        conn.first << resource;
        conn.first << "\r\n\r\n";
    }
    catch(file_error& fe)
    {

        status http_error;
        switch(fe.err_num())
        {
            case EACCES: http_error = status::forbidden;
                         break;
            case EINTR: //TODO: Try again.
                         http_error = status::internal_server_error;
                         break;

            case EOVERFLOW: //TODO: This is probably the Consequence
                         // of some bugs on the client side, that
                         // allow them to load up Files of arbitrary sizes.
            case EIO:
            case ELOOP:  //TODO: Log some infos so the problem can
                         // be fixed. unknown
            case ENAMETOOLONG: http_error = status::internal_server_error;
                               break;
            case ENFILE: //TODO: Try to close files.
            case EMFILE: http_error = status::internal_server_error;

            case ENOENT: http_error = status::not_found;
                         break;

            case ENXIO: //TODO: This should point to a bigger error
                         // Probably some sort of access escalation.
            case ENOSR:  //TODO: Is this possible? It shouldn't.
                         http_error = status::internal_server_error;
                         break;
        }
        std::cout << strerror(fe.err_num()) << '\n';
        conn.first << header(http_error);
        if(request_type != method::HEAD)
        {
            conn.first << error_body(http_error);
            conn.first << "\r\n";
        }
    }
}

bool starts_with(const std::string& str_1, const std::string& str_2)
{
    return (std::strncmp(str_1.c_str(), str_2.c_str(), str_2.size()) == 0);
}
