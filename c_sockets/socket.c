#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <errno.h>

int main(){

    /*
        Linuxでは、ソケットは「int」で表現されるファイルディスクリプタ。

        ファイルをopenしたときに出来るファイルディスクリプタと同様に、
        ソケットもファイルディスクリプタ
    */
    int sock;

    /*
        全ての通信は、socket()が返すファイルディスクリプタを使って行われる。
        socket()は、失敗すると-1を返す。

        socket()は、成功すると0を返す。0が正常な値。
        例えば、0番でopenされているファイルディスクリプタが無い状態で、
        socketシステムコールを利用した場合にはsocketは0を返します。
    */

    /*
        socket()関数は3つの引数を取る。

        通信路で使われるプロトコルは、
        ・アドレスファミリ
        ・ソケットタイプ
        ・プロトコル
        ３つの組み合わせにより決定。
        
        一つ目の引数がアドレスファミリ
        二つ目の引数がソケットタイプ
        三つ目の引数はプロトコル
        
        ここでは、とりあえずAF_INET(IPv4)とSOCK_STREAM(TCP)という
        組み合わせでソケットを作成
    */
    /*
        POSIXでは、各プロセスが以下のファイルディスクプリタを予め保持している事を定義

        0,stdin,標準入力
        1,stdin,標準出力
        2,stdin,標準エラー出力

        以下の処理では3以上からファイルディスクプリタを開始する
    */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        printf("socket failed\n");
        return 1;
    }
    printf("%d\n", sock);

    /*
        socket()が返すファイルディスプリタを

        close(0)で、標準入力をcloseしている
    */
    printf("fileno(stdin) = %d\n", fileno(stdin));
    close(0);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    printf("sock=%d\n", sock);

    return 0;
}
