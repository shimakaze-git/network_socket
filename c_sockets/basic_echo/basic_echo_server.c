/*
    インターネットは信頼性のない通信路。
    信頼性がないインターネットでは、通信中にパケットロス、ビットエラー、順番の入れ替えなどが発生する可能性がある。
    TCPは、そのような通信路上の障害を隠蔽してくれ、
    具体的には、パケットロスが発生したら再送を行ったり、
    順番が入れ替わると正しい順番に直したりなどを行う
*/

/*
    サーバはクライアントからの接続要求を待つ。
    接続要求を待つには、どのような待ち方をするかを設定しなければいけない。

    設定する情報としては、例えば接続待ちをするTCPポート番号など。

    一度接続が出来上がってしまえば、
    サーバとクライアントで通信方法に違いは無い
    ・ソケットを作る
    ・接続待ちをするIPアドレスとポートを設定する
    ・ソケットに名前をつける（bindする）
    ・接続待ちする(listen())
    ・クライアントからの接続を受け付ける(accept())
    ・通信を行う(read(), write())
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void){
    int sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    int len;
    int sock;

    /*
        ソケットの作成
    */
    sock0 = socket(AF_INET, SOCK_STREAM, 0);

    /*
        ソケットの設定
    */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(
        sock0,
        (struct sockaddr *)&addr,
        sizeof(addr)
    );
    
    /*
        TCPクライアントからの接続要求を待てる状態にする
    */
    listen(sock0, 5);

    /*
        TCPクライアントからの接続要求を受け付ける
    */
    len = sizeof(client);
    sock = accept(
        sock0,
        (struct sockaddr *)&client,
        &len
    );

    /*
        5文字送信
    */
    write(sock, "HELLO", 5);

    /*
        TCPセッションの終了
    */
    close(sock);

    /*
        listen するsocketの終了
    */
    close(sock0);

    return 0;
}