#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <cstring>

#include "../Socket/Socket.hpp"
#include "../Socket/error.hpp"

int main()
{
        Socket sock{PF_INET, SOCK_STREAM, 0};
        sock.setsockopt(SOL_SOCKET, SO_REUSEADDR);
        sock.bind(Ipv4{80});
        sock.listen(20);

        while(true)
        {
            Connection conn = sock.accept();
            try
            {
            while(true)
            {
                std::cout << conn.first.receive_line() << '\n';
            }
            } catch(...)
            {
            }
        }
}
