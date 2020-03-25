#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
    //初始化WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        return 0;
    }

    //创建套接字
    // 首先看服务端，它首先创建了一个套接字，用于客户端的连接：
    // socket函数有三个参数，都是socket默认定义的宏，
    // 第一个参数AF_INET基本是固定的值，意思是使用INTERNET address family，就是我们平常使用的标准的IPV4格式的IP地址，比如192.168.0.40.
    // 第二个参数是选择socket类型，常用的就两种参数，Socket_stream对应TCP?socket_dgram对应UDP ，本例中，自然指的是TCP了。
    // 第三个也是指TCP，如果你使用前两个参数中的一个，也可以把它置为0，只有第二个参数为原始socket才需要指出具体类型。

    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    //绑定IP和端口
    // 然后是定义了一个sockt地址：socketaddr_in  sin。这也基本上是标准写法了，它是一个internet下的结构体，定义如下：
    sockaddr_in sin;
    sin.sin_family = AF_INET;    //一般就是AF_INET
    sin.sin_port = htons(8888);    //指定端口
    // htons则是将本机的字符流转换成网络字符流，这里涉及到一些大小端的问题，扯开来就比较长了。有兴趣可以自己百度下
    sin.sin_addr.S_un.S_addr = INADDR_ANY;  // INADDR_ANY指的是任何ip都可以连入
    // 接着开始绑定socket的端口和ip。bind函数也是标准的socket函数，如果发起失败，则返回socket_error值。
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    //开始监听
    // 接着监听端口，此时服务端正式工作
    if(listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }

// 在这个死循环里，服务端不断的监听是否有客户端发来的链接请求，accept和recv都是标准的socket函数，
// 当accept接收到连接请求时，就建立连接，recv则接受客户端发来的请求，把请求内容放到revData数组里面。
// 本例中客户端发来的就是一串字符串，服务端把它打印到屏幕上。最后服务端调用closesocket函数关闭连接。
    //循环接收数据
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255];
    while (true)
    {
        printf("等待连接...\n");
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
        if(sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            continue;
        }
        printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

        //接收数据
        int ret = recv(sClient, revData, 255, 0);
        if(ret > 0)
        {
            revData[ret] = 0x00;
            printf(revData);
        }

        //发送数据
        char * sendData = "你好，TCP客户端！\n";
        send(sClient, sendData, strlen(sendData), 0);
        closesocket(sClient);
    }

    closesocket(slisten);
    WSACleanup();
    return 0;
}
