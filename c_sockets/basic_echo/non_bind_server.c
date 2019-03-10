/*
    bind()とは名前をつけること

    bind()を利用せずにlisten()を行ってみる
*/

#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>


void print_my_port_num(int sock);

int main(){
    int s0, sock;
    struct sockaddr_in peer;
    socklen_t peerlen;
    int n;
    char buf[1024];

    /*
        ソケットを作成していきなりlisten()する
    */
    s0 = socket(AF_INET, SOCK_STREAM, 0);

    /*
        普通であればbind()して、ソケットの設定を行う。
    */
    // peer.sin_family = AF_INET;
    // peer.sin_port = htons(12345);
    // peer.sin_addr.s_addr = INADDR_ANY;
    // bind(
    //     s0,
    //     (struct sockaddr *)&peer,
    //     sizeof(peer)
    // );

    /*
        listen()をして、
        TCPクライアントからの接続要求を待てる状態にする
    */
    if (listen(s0, 5) != 0) {
        perror("listen");
        return 1;
    }

    /*
        listen()すると自動的に未使用ポートを
        割り当てられることを確認
    */
    print_my_port_num(s0);

    /*
        TCPコネクションを受付

        sockaddr_in type: peer

        accept(int sockfd, struct scokaddr *addr, int *addrlen)
        - 第一引数sockfdはソケット番号
        - 第二引数addrは sockaddr構造体へのポインタ。
        インターネットでは、sockaddr_in構造体をsockaddr構造体にキャストしたものを与える
        この第二引数には接続してきたクライアントのアドレス等が格納される。

        sockaddr_in構造体として、その中身を取り出すと、
        クライアント側のIPアドレスやポート番号などの情報を調べることができる。

        なお、第二引数にNULLを与えた場合は、
        クライアント側の情報を返すことはない。
    */
    peerlen = sizeof(peer);
    sock = accept(s0, (struct sockaddr *)&peer, &peerlen);
    if (sock < 0) {
        perror("accept");
        return 1;
    }

    /*
        相手に文字列を送信して終了
    */
    char write_str[] = "HOGE\n";
    int write_str_len = strlen(write_str);
    write(sock, write_str, write_str_len);

    close(sock);
    close(s0);

    return 0;
}

/*
    ポート番号とbindされたアドレスを表示する関数
*/
void print_my_port_num(int sock){
    char buf[48];
    struct sockaddr_in s;
    socklen_t sz;

    sz = sizeof(s);

    /*
        ソケットの「名前」を取得
    */
    if (getsockname(sock, (struct sockaddr *)&s, &sz) != 0) {
        perror("getsockname");
        return;
    }

    /*
        bindされているIPアドレスを文字列へ変換
    */
    inet_ntop(AF_INET, &s.sin_addr, buf, sizeof(buf));

    /*
        結果を表示
    */
    printf("%s : %d\n", buf, ntohs(s.sin_port));
}

/*
    Client:

    $ telnet 127.0.0.1 50767
    >Trying 127.0.0.1...
    >Connected to localhost.
    >Escape character is '^]'.
    >HOGE
    >Connection closed by foreign host.

*/