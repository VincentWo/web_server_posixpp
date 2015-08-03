#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
    printf("%s\n",strerror(EFAULT));
}
