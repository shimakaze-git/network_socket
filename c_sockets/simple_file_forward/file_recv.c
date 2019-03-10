#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>


/*
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};
*/


int main(int argc, char *argv[]){
    int sock0;
    struct sockaddr_in client;
    socklen_t len;
    int sock;
    struct addrinfo hints, *res;

    int err;
    int fd;
    int n, ret;
    char buf[65536];

    /*
        受信側サンプルプログラムは、
        実行時に受信したファイルを保存するファイルパスを指定

        指定されたファイルパスにファイルが存在していなければ
        新たにファイルが作成され、ファイルのパーミッションは作成者のみが読み書きできるものになる。
        
        例えば、受信側サンプルプログラムがfile_recv.out
        というファイル名の場合、以下のように実行します。

        $ ./file_recv.out recv.txt
    */
    if (argc != 2) {
        fprintf(stderr, "Usage : %s outputfilename\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT, 0600);
    if (fd < 0) {
        perror("open");
        return 1;
    }


    /*
        ネットワークからファイルデータを受信するためのソケットを用意し、
        bind(),listen(),accept()を行っています。
    */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    err = getaddrinfo(NULL, "12345", &hints, &res);
    if (err != 0) {
        printf("getaddrinfo : %s\n", gai_strerror(err));
        return 1;
    }

    /*
        ソケットの作成
    */
    sock0 = socket(res->ai_family, res->ai_socktype, 0);
    if (sock0 < 0) {
        perror("socket");
        return 1;
    }
    printf("res->ai_flags %d\n", res->ai_flags);
    printf("res->ai_family %d\n", res->ai_family);
    printf("res->ai_socktype %d\n", res->ai_socktype);
    printf("res->ai_addrlen %d\n", res->ai_addrlen);
    printf("res->ai_canonname %s\n", res->ai_canonname);

    /*
        bind()処理
    */
    if (bind(sock0, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    /* addrinfo構造体を解放 */
    freeaddrinfo(res);

    /*
        TCPクライアントからの接続要求を待てる状態にする
    */
    if (listen(sock0, 5) != 0) {
        perror("listen");
        return 1;
    }

    /*
        TCPクライアントからの接続要求を受け付ける
    */
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);
    if (sock < 0) {
        perror("accept");
        return 1;
    }


    /*
        TCP接続を確立した後に、ネットワークから
        データを読み込みながらファイルへと書き込んでいます。
        
        whileループは、送信側がファイルデータをすべて送信し終わってclose()を行い、
        read()がEOFを意味する0を返すか、エラーによって-1を返すまで繰り返される。

        whileループを抜けると、ファイル受信側プログラムはソケットを閉じて終了します。
        このサンプルプログラムは複数回accept()するようには実装されておらず、
        ひとつのTCP接続が終了するとともにプロセスも終了する。
        
        ここで紹介したファイル転送プログラムは、ファイルの中身のみを転送しています。
        ファイル名や、ファイルパーミッションなどの付属情報も転送するには、
        何らかのプロトコルを規定することによって、送信側から受信側にそれらの情報を伝えなければならない。
        
        次に紹介するHTTPでは、最初にヘッダ情報が送信されたあとにデータ本体が送信される
        プロトコルになっています。

        このように、データ本体と付属情報という視点で通信プロトコルをみていくと、
        いろいろと面白い発見があると思います。
    */
    while ((n = read(sock, buf, sizeof(buf))) > 0) {
        ret = write(fd, buf, n);
        if (ret < 1) {
            perror("write");
            break;
        }
    }

    /* TCPセッションの終了 */
    close(sock);

    /*
        listen するsocketの終了
    */
    close(sock0);
    return 0;
}