#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8887
#define BACKLOG 2	//�������г���

int main(int argc, char *argv[])
{
	int ss,sc;	//ssΪ��������socket��scΪ�ͻ���
	struct sockaddr_in server_addr;	//��������ַ�ṹ
	struct sockaddr_in client_addr;	//�ͻ��˵�ַ�ṹ
	pid_t pid;				/*�ֲ�Ľ���ID*/
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0)
	{
		printf("socket error\n");
		return -1;
	}

	//���÷�������ַ
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;	//Э����
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//���ص�ַ
	server_addr.sin_port = htons(PORT);	//�������˿�

    //�󶨵�ַ�ṹ���׽���������
	if(bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("bind error");
		return -1;
	}

	//��������
	if(listen(ss, BACKLOG) < 0){
		perror("listen error\n");
		return -1;
	}
	printf("Server started!\n");
	socklen_t addrlen = sizeof(struct sockaddr);
    sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
    if(sc < 0){
        perror("accept:");
    }
    printf("client connected!");
	while(1)
	{
		pid = fork();
		if( pid == 0 )
        {
            /*�ӽ�����*/
            while(1)
            {
                char buffer[1024];
                int len = recv(sc, buffer, sizeof(buffer),0);
                if(strcmp(buffer,"exit\n")==0 || len<=0)
                    break;
                printf("data from client:%s\n",buffer);
                send(sc, buffer, len, 0);
                printf("data send to client:%s\n",buffer);
            }
                close(ss);	/*���ӽ����йرշ�����������*/

		}
		else
        {
            close(sc);	/*�ڸ������йرտͻ��˵�����*/
		}
	}
}
