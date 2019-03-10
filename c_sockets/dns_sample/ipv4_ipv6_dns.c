#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int main(){
    char *hostname = "localhost";
    char *service = "http";
    struct addrinfo hints, *res0, *res;
    int err;
    int sock;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = PF_UNSPEC;
    if ((err = getaddrinfo(hostname, service, &hints, &res0)) != 0) {
        printf("error %d\n", err);
        return 1;
    }

    for(res=res0; res!=NULL; res=res->ai_next){
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock < 0) {
            continue;
        }

        if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
            close(sock);
            continue;
        }

        break;
    }

    freeaddrinfo(res0);

    if (res == NULL) {
        /*
            有効な接続ができなかった場合
        */
        printf("failed\n");
        return 1;
    }
    /*
        ここ以降にsockを使った通信を行うプログラムを書く
    */


    return 0;
}   