/*************
正在测试与服务
 器的收发消息
**************/
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")
#define BUFFER_SIZE 256 // 定义缓冲区大小

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    SOCKET ClientSocket;
    struct sockaddr_in server;
    char* message;
    char server_reply[BUFFER_SIZE]; // 存储服务器回复的消息
    int recv_size;
    char ipAddress[16]; // 存储IP地址
    int portno; // 存储端口号
    int c = sizeof(struct sockaddr_in);

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    // 创建套接字
    if ((ClientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    // 输入服务器IP地址和端口号
    printf("Enter server IP address: ");
    scanf("%s", ipAddress);
    printf("Enter server port number: ");
    scanf("%d", &portno);
    getchar(); // 清除缓冲区中的换行符

    // 设置服务器地址
    server.sin_addr.s_addr = inet_addr(ipAddress);
    server.sin_family = AF_INET;
    server.sin_port = htons(portno);

    // 连接到服务器
    if (connect(ClientSocket, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }

    // 循环读取输入并发送给服务器，同时接收并打印服务器的消息
    while (1) {
        char buffer[BUFFER_SIZE];

        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin); // 使用fgets替代gets
        message = buffer;

        // 将输入的消息发送给服务器
        send(ClientSocket, message, strlen(message), 0);
        getchar(); // 清除缓冲区中的换行符
        // 接收并打印服务器的消息
        if ((recv_size = recv(ClientSocket, server_reply, BUFFER_SIZE, 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
        }

        server_reply[recv_size] = '\0';
        puts(server_reply);
    }

    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
