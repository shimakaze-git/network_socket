#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
// #include "sock.h"
// #include "ether.h"
// #include "arp.h"
// #include "ip.h"
// #include "icmp.h"
// #include "udp.h"
// #include "tcp.h"
// #include "dhcp.h"
// #include "param.h"
// #include "cmd.h"

/*
    EndFlagは終了関連のシグナルを受けた際に、1として終了処理に進むための変数
    DeviceSocは送受信するPF_PACKETのディスクプリタを格納
    Pramは設定を保持するための構造体、定義はparam.hにある
*/
int EndFlag = 0;
int DeviceSoc;
// PARAM Param;


void *MyEthThread(void *arg) {
    
}