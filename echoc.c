#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ADDR "127.0.0.1"
#define PORT 31337

int main()
{
    char buf[256];

    struct sockaddr_in sock_addr;
    sock_addr.sin_family        = AF_INET;
    sock_addr.sin_addr.s_addr   = inet_addr(ADDR);
    sock_addr.sin_port          = htons(PORT);

    int connect_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(connect(connect_sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) exit(1);

    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        write(connect_sock, buf, strlen(buf) + 1);
        read(connect_sock, buf, strlen(buf) + 1);
        printf("%s\n", buf);
    }

    close(connect_sock);
    return 0;
}