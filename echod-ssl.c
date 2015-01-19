#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <openssl/ssl.h>

#define PORT 31337

int main()
{
    char buf[256];

    struct sockaddr_in sock_addr;
    sock_addr.sin_family        = AF_INET;
    sock_addr.sin_addr.s_addr   = htonl(INADDR_ANY);
    sock_addr.sin_port          = htons(PORT);

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(listen_sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr));

    while(1)
    {
        listen(listen_sock, SOMAXCONN);
        int data_sock = accept(listen_sock, NULL, NULL);

        SSL_library_init();
        SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
        SSL* ssl = SSL_new(ctx);
        SSL_set_fd(ssl, data_sock);

        SSL_use_certificate_file(ssl, "public.key", SSL_FILETYPE_PEM);
        SSL_use_PrivateKey_file(ssl, "private.crt", SSL_FILETYPE_PEM);

        SSL_accept(ssl);

        while(1)
        {
            // read(data_sock, buf, sizeof(buf));
            SSL_read(ssl, buf, sizeof(buf));

            buf[sizeof(buf) - 1] = '\0';
            printf("%s\n", buf);

            // write(data_sock, buf, strlen(buf));
            SSL_write(ssl, buf, strlen(buf));
        }
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);

        close(data_sock);
    }
    return 0;
}
