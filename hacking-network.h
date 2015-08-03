#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

/* This function accepts a socket FD and a ptr to the null terminated
 * string to send. The function will make sure, that all the bytes
 * of the string are sent. Returns 1 on success and 0 on failure.
 */

int send_string(int sockfd, unsigned char* buffer)
{
    int sent_bytes;
    int bytes_to_send;
    bytes_to_send = strlen(buffer);
    while(bytes_to_send > 0)
    {
        sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
        if(sent_bytes == -1)
            return 0;
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return 1;
}

/* This functio naccepts a socket FD and a ptr to a destination
 * buffer. It will receive fomr the socket until the EOL byte
 * sequence is seen. The EOL bytes are read from the socket, but
 * the destination buffer is terminated before these bytes.
 * Returns the size of the read line (without EOL bytes).
 */
int recv_line(int sockfd, unsigned char *dest_buffer)
{
#define EOL "\r\n" // End-of-line byte sequence
#define EOL_SIZE 2
    unsigned char *ptr;
    int eol_matched = 0;

    ptr = dest_buffer;
    while(recv(sockfd, ptr, 1, 0) == 1)
    {
        if(*ptr == EOL[eol_matched])
        {
            eol_matched++;
            if(eol_matched == EOL_SIZE)
            {
                *(ptr+1-EOL_SIZE) = '\0';
                return strlen(dest_buffer);
            }
        } else
        {
            eol_matched = 0;
        }
        ++ptr;
    }
    return 0;
}

