#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>

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

    SSL_library_init(); 
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
    SSL* ssl = SSL_new(ctx);

    SSL_set_fd(ssl, connect_sock);
    SSL_connect(ssl);

    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        // write(connect_sock, buf, strlen(buf) + 1);
        SSL_write(ssl, buf, strlen(buf) + 1);
        // read(connect_sock, buf, strlen(buf) + 1);
        SSL_read(ssl, buf, strlen(buf) + 1);
        printf("%s\n", buf);
    }
    SSL_shutdown(ssl); 
    SSL_free(ssl); 
    SSL_CTX_free(ctx);

    close(connect_sock);
    return 0;
}