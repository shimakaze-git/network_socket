/*
    ファイル転送用のclientプログラム
    
    write()されたデータは直接ネットワークへと送信されるわけではなく、
    カーネル内の'ソケットバッファ'と呼ばれるバッファへとコピーされる。

    ソケットバッファへ格納されたデータは、
    ネットワークの形態に合わせたサイズへと小分けにされ、
    パケットとして送信されていきます。

    TCPには、途中ネットワークで喪失したパケットを検知して
    再送信する仕組みや、ネットワーク上の混雑を回避する輻輳制御機構がありますが、
    それらの仕組みが動作しながらパケット化されたソケットバッファ内のデータが送信されていく。


    listen（）を行っているファイル受信側は、最初に保存用のファイルを作成。
    次に、ネットワークを通じたファイル受信用にソケットが用意される。
    
    ファイル送信側からのconnect()が行われ、ファイル受信側でlisten()している
    ソケットからaccept()が完了した後に、ファイル受信側はファイル送信側からの
    ファイルデータをread()しつつ、その結果をファイルへとwrite()します。
    
    ファイル送信側からのパケットは、パケットとして直接read()されるわけではなく、一度
    ソケットバッファに格納されてからread()される。
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>


int main(int argc, char *argv[]){

    char *service = "12345";
    struct addrinfo hints, *res0, *res;
    int err, sock, fd;

    char buf[65536];
    int n, ret;

    /*
        送信側サンプルプログラムは、
        実行時に接続先と送信するファイルパスを指定
    */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s hostname filename\n", argv[0]);
        return 1;
    }

    /*
        ファイルを読み込み用に開く
    */
    fd = open(argv[2], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }


    /*
        ファイル受信側と接続
    */
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = PF_UNSPEC;

    if ((err = getaddrinfo(argv[1], service, &hints, &res0)) != 0) {
        printf("error %d : %s\n", err, gai_strerror(err));
        return 1;
    }

    /*
        socketの生成が成功するまで繰り返す
    */
    for (res=res0; res!=NULL; res=res->ai_next) {
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        // printf("res->ai_family: %s\n", res->ai_family);

        if (sock < 0) {
            continue;
        }

        if (connect(sock, res->ai_addr, res->ai_addrlen) != 0) {
            close(sock);
            continue;
        }

        break;
    }

    /*
        解放
    */
    freeaddrinfo(res0);

    if (res == NULL) {
        /*
            有効な接続が出来なかった場合
        */
        printf("failed\n");
        return 1;
    }

    /*
        while loopは、ファイルの終端まで読み込みが終わり、
        read()がファイルの終わり（EOF）を意味する0を返すか、
        エラーによって-1を返すまで繰り返されます。
    */
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        ret = write(sock, buf, n);
        
        if (ret < 1) {
            perror("write");
            break;
        }
    }

    close(sock);

    return 0;
}