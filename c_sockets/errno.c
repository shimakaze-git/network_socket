#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
/*
    socket()システムコールで発生する
    代表的なエラーとerrno

    EACCES: 指定されたタイプまたはプロトコルのソケットを作成する
    許可が与えられていない

    EAFNOSUPPORT: 指定されたアドレスファミリーがサポートされていない

    EINVAL: 知らないプロコトル、又は利用できないプロトコルファミリ

    EMFILE: プロセスのファイルテーブルが溢れている

    ENFILE: カーネルに新しいソケット構造体に割り当てる
    ための十分なメモリがない

    ENOBUFS or ENOMEM: 十分なメモリが無い。十分な資源が解放される
    まではソケットを作成できない。

    EPROTONOSUPPORT: このドメインでは指定されたプロトコル又は
    プロトコルタイプがサポートされていない
*/


int main(){
    int sock;

    sock = socket(3000, 4000, 5000);
    if (sock < 0) {
        perror("socket");
        printf(":%d\n", errno);
        return 1;
    }

    return 0;
}