#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
    //��ʼ��WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        return 0;
    }

    //�����׽���
    // ���ȿ�����ˣ������ȴ�����һ���׽��֣����ڿͻ��˵����ӣ�
    // socket��������������������socketĬ�϶���ĺ꣬
    // ��һ������AF_INET�����ǹ̶���ֵ����˼��ʹ��INTERNET address family����������ƽ��ʹ�õı�׼��IPV4��ʽ��IP��ַ������192.168.0.40.
    // �ڶ���������ѡ��socket���ͣ����õľ����ֲ�����Socket_stream��ӦTCP?socket_dgram��ӦUDP �������У���Ȼָ����TCP�ˡ�
    // ������Ҳ��ָTCP�������ʹ��ǰ���������е�һ����Ҳ���԰�����Ϊ0��ֻ�еڶ�������Ϊԭʼsocket����Ҫָ���������͡�

    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    //��IP�Ͷ˿�
    // Ȼ���Ƕ�����һ��sockt��ַ��socketaddr_in  sin����Ҳ�������Ǳ�׼д���ˣ�����һ��internet�µĽṹ�壬�������£�
    sockaddr_in sin;
    sin.sin_family = AF_INET;    //һ�����AF_INET
    sin.sin_port = htons(8888);    //ָ���˿�
    // htons���ǽ��������ַ���ת���������ַ����������漰��һЩ��С�˵����⣬�������ͱȽϳ��ˡ�����Ȥ�����Լ��ٶ���
    sin.sin_addr.S_un.S_addr = INADDR_ANY;  // INADDR_ANYָ�����κ�ip����������
    // ���ſ�ʼ��socket�Ķ˿ں�ip��bind����Ҳ�Ǳ�׼��socket�������������ʧ�ܣ��򷵻�socket_errorֵ��
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    //��ʼ����
    // ���ż����˿ڣ���ʱ�������ʽ����
    if(listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }

// �������ѭ�������˲��ϵļ����Ƿ��пͻ��˷�������������accept��recv���Ǳ�׼��socket������
// ��accept���յ���������ʱ���ͽ������ӣ�recv����ܿͻ��˷��������󣬰��������ݷŵ�revData�������档
// �����пͻ��˷����ľ���һ���ַ���������˰�����ӡ����Ļ�ϡ�������˵���closesocket�����ر����ӡ�
    //ѭ����������
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255];
    while (true)
    {
        printf("�ȴ�����...\n");
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
        if(sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            continue;
        }
        printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

        //��������
        int ret = recv(sClient, revData, 255, 0);
        if(ret > 0)
        {
            revData[ret] = 0x00;
            printf(revData);
        }

        //��������
        char * sendData = "��ã�TCP�ͻ��ˣ�\n";
        send(sClient, sendData, strlen(sendData), 0);
        closesocket(sClient);
    }

    closesocket(slisten);
    WSACleanup();
    return 0;
}
