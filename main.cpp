#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <cstring>
#include <fstream>
#include <cstdio>

#include "posixpp/socket.hpp"
#include "posixpp/file.hpp"
#include "posixpp/error.hpp"
#include "http.hpp"

constexpr int port = 80;
extern void handle_connection(Connection&, std::ostream& log = std::clog);

using std::cout;
using std::cerr;
using std::string;
using std::endl;
int main(void)
{
    Socket sock{Socket::ipv4, Socket::stream};

    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR);
    //sock.set_option(Socket::REUSEADDR);

    sock.bind(Ipv4{80}); 
    sock.listen(20);

    cout << "Accepting web request on port " << port << '\n';    
    while(true)
    {
        Connection conn = sock.accept();
        handle_connection(conn);
    }
}
