/*

*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void print_my_port_num(int sock);

int main(){
    int sock0;
    struct sockaddr_in client;
    socklen_t len;

    int sock;
    struct addrinfo hints, *res;
    int err;

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

    if (bind(sock0, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind");
        return 1;
    }

    /*
        addrinfo構造体を解放
    */
    freeaddrinfo(res);

    /*
        TCPクライアントから接続要求を待つ
    */
    if (listen(sock0, 5) != 0) {
        perror("listen");
        return 1;
    }


    print_my_port_num(sock0);

    /*
        TCPクライアントからの接続要求を受け付ける
    */
    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr *)&client, &len);


    /*
        5文字送信
    */
    char write_str[] = "HOGE\n";
    int write_str_len = strlen(write_str);
    write(sock, write_str, write_str_len);

    /*
        TCPセッションの終了
    */
    close(sock);

    /*
        listenするsocketの終了
    */
    close(sock0);
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