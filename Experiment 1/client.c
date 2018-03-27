#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8887	/*�����˿ڵ�ַ*/
#define BUFFER_SIZE 1024
int main(int argc, char *argv[])
{
	int s;		//sΪsocket������
	struct sockaddr_in server_addr;	//��������ַ�ṹ

	char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];

	s = socket(AF_INET, SOCK_STREAM, 0); //����һ����ʽ�׽���
	if(s < 0)
    {
		perror("socket error");
		return -1;
	}
    //���÷�������ַ
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;//Э����
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//���ص�ַ
	server_addr.sin_port = htons(PORT);

	//���ӷ�����
	if((connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))) == -1)
    {
        perror("connect error");
        return -1;
    }
    printf("connect success!\n");
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        printf("send to server:%s\n",sendbuf);
        send(s, sendbuf, strlen(sendbuf),0); //����
        if(strcmp(sendbuf,"exit\n")==0)
            break;
        recv(s, recvbuf, sizeof(recvbuf),0); //����
        printf("recv from server:%s\n",recvbuf);

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
	close(s);
	return 0;
}


