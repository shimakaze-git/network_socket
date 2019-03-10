#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>


int listen(
    int sockfd,
    int backlog
);

/*
    listen(int sockfd, int backlog);

    int sockfd
    - SOCK_STREAM型のソケット

    int backlog
    - 接続保留状態を保持できる数

    listen()システムコールの第二引数であるbacklogは、
    accept()されていないTCPコネクションを保持できる最大数

    listen()の開始によってクライアントからTCPコネクションを受付可能
    
    accept()は、カーネル内に保持された確立済みTCPセッションを、
    ソケットという形でユーザアプリケーションに渡すことが主目的であり、
    accept()そのものがTCPセッション確立を行っているわけでは無い。
    
    listen()システムコールは成功時に0を、失敗時に-1を返す。
    このとき、エラー内容はerrnoに設定さる。
*/

/*
    errnoの値は以下の内容が設定される可能性があります。
    - EADDRINUSE 別のソケットがすでに同じポートをlisten()している
    - EBADF 引数sockfdが有効なディスクリプタではない
    - ENOTSOCK 引数sockfdがソケットではない
    - EOPNOTSUPP ソケットはlisten()がサポートしている型ではない
*/

/*
    listen()の2番目の引数は、確立されていない不完全なTCPセッション数ではなく、
    確立されたTCPセッション数を表している。

    ちなみに、古い設計ではlisten()の第二引数は不完全なTCPセッション数を表現していた。
    しかし、SYN floodingという偽TCP接続要求パケットの大量送信による
    サービス不能攻撃が多発したことなどが要因で変更された。

    確立されていないTCPセッション数は、
    以下のコマンドを実行すると
    IPv4 TCPのtcp_max_syn_backlogを知ることができます。

    $ sysctl net.ipv4.tcp_max_syn_backlog

    net.ipv4.tcp_syncookiesを有効にすると、
    tcp_max_syn_backlogの値は利用されなくなり、
    論理的な上限はなくなる。
*/