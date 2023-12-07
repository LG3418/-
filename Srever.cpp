/*斗地主test阶段*/

/*****************
  目前为服务器测试
******************/

//加载头文件
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"time.h"
#include <sys/types.h>
#include <winsock2.h>

//定义一些全局变量
#pragma comment(lib,"ws2_32.lib")
#define MAX_CLIENTS 3 // 定义最大客户端连接数
#define BUFFER_SIZE 256 // 定义缓冲区大小
int owncard[21];//手牌
int lastcard[21];//上一次出牌
int lastlength = 0;//上一次出牌的张数
int length = 2;//手牌数，2无意义，大于等于2皆可，后面会覆盖
int pass = 0;//跳过计数
int turn;//出牌顺序
int player;//玩家编号
int flag;
int totalCards = 54;
int sendCrads = 51;

void sendCrads(int *Player)//将1~54随机分成有17个数字的三份，分别存入三个数组，剩余三个数字存入另一个数组。以实现随机发牌的效果
{
    int numbers[54], group1[17], group2[17], group3[17], remaining[3];
    int i, j, temp;

    // 初始化数组
    for (i = 0; i < 54; i++) {
        numbers[i] = i + 1;
    }

    // 随机排列数组
    srand(time(NULL));
    for (i = 53; i > 0; i--) {
        j = rand() % (i + 1);
        temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }

    // 分配数字到四个数组
    for (i = 0; i < 17; i++) {
        group1[i] = numbers[i];
        group2[i] = numbers[i + 17];
        group3[i] = numbers[i + 34];
    }
    for (i = 0; i < 3; i++) {
        remaining[i] = numbers[i + 51];
    }

    // 打印四个数组以供测试
    printf("第一组:\n");
    for (i = 0; i < 17; i++) {
        printf("%d ", group1[i]);
    }
    printf("\n\n第二组:\n");
    for (i = 0; i < 17; i++) {
        printf("%d ", group2[i]);
    }
    printf("\n\n第三组:\n");
    for (i = 0; i < 17; i++) {
        printf("%d ", group3[i]);
    }
    printf("\n\n剩余的数字:\n");
    for (i = 0; i < 3; i++) {
        printf("%d ", remaining[i]);
    }
    printf("\n");
}

/*********************
 main函数实现服务器功能
**********************/

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    SOCKET ServerSocket, ClientSocket[MAX_CLIENTS];
    struct sockaddr_in server, client;
    char* message;
    int portno = 8888; // 设置端口号
    int c = sizeof(struct sockaddr_in);
    char clientNames[MAX_CLIENTS] = { 'A', 'B', 'C' }; // 客户端名称

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    // 创建套接字
    if ((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    // 设置服务器地址
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portno);

    // 绑定套接字
    if (bind(ServerSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }

    // 监听
    listen(ServerSocket, MAX_CLIENTS);

    // 接受客户端连接
    for (int i = 0; i < MAX_CLIENTS; i++) {
        ClientSocket[i] = accept(ServerSocket, (struct sockaddr*)&client, &c);
        if (ClientSocket[i] == INVALID_SOCKET)
        {
            printf("accept failed with error code : %d", WSAGetLastError());
            return 1;
        }
    }

    // 循环读取输入并发送给指定客户端
    while (1) 
    {
        char buffer[BUFFER_SIZE];
        char targetClient;
        int targetIndex = -1;

        printf("Enter target client (A, B, or C): ");
        scanf("%c", &targetClient);
        getchar(); // 清除缓冲区中的换行符

        // 查找目标客户端
        for (int i = 0; i < MAX_CLIENTS; i++) 
        {
            if (clientNames[i] == targetClient) {
                targetIndex = i;
                break;
            }
        }

        // 如果找不到目标客户端，打印错误消息
        if (targetIndex == -1) 
        {
            printf("Invalid client name.\n");
            continue;
        }

        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin); // 使用fgets替代gets
        message = buffer;

        // 将输入的消息发送给指定客户端
        send(ClientSocket[targetIndex], message, strlen(message), 0);

        // 接收并打印客户端的消息
        memset(buffer, 0, BUFFER_SIZE); // 清空缓冲区
        recv(ClientSocket[targetIndex], buffer, BUFFER_SIZE, 0);
        printf("Message from client %c: %s\n", clientNames[targetIndex], buffer);
    }

    closesocket(ServerSocket);
    WSACleanup();

    return 0;
}
