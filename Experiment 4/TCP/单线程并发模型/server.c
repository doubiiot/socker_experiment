#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#define PORT 8887
#define BACKLOG 10	//�������г���
#define FILE_MAX_SIZE 1024
#define BUFFER_SIZE 1024
void *client_process(void *arg)
{
    char buffer[BUFFER_SIZE];
    bzero(buffer,sizeof(buffer));

    int sc = (int)arg;
    if(recv(sc, buffer, BUFFER_SIZE, 0) < 0)
    {
        perror("recv data ");
        exit(1);
    }

    char file_name[FILE_MAX_SIZE + 1];
    bzero(file_name, sizeof(file_name));
    strncpy(file_name, buffer, strlen(buffer) > FILE_MAX_SIZE ? FILE_MAX_SIZE : strlen(buffer));
    printf("file name is : %s\n",file_name);


    FILE *fp = fopen(file_name,"rb");
    if(fp == NULL)
    {
        printf("file not found!\n");
        exit(1);
    }
    else
    {
        bzero(buffer,BUFFER_SIZE);
        int file_block_length = 0;
        while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
        {

            // ����buffer�е��ַ�����new_server_socket,ʵ���Ͼ��Ƿ��͸��ͻ���
            if (send(sc, buffer, file_block_length, 0) < 0)
            {
                printf("Send File: %s Failed!\n", file_name);
                break;
            }
            bzero(buffer, sizeof(BUFFER_SIZE));
        }
        fclose(fp);
        printf("File: %s Transfer Finished!\n", file_name);
    }
    close(sc);

    return NULL;
}
int main(int argc, char *argv[])
{
	int ss;	                        //ssΪ��������socket��scΪ�ͻ���
	int i = 0;
	ss = socket(AF_INET, SOCK_STREAM, 0);
	pthread_t thread_id;
	if(ss < 0)
	{
		printf("socket error\n");
		exit(1);
	}

    struct sockaddr_in server_addr;	    //��������ַ�ṹ
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr =  htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

    //�󶨵�ַ�ṹ���׽���������
	if(bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("bind error");
		exit(1);
	}
	//��������
	if(listen(ss, BACKLOG) < 0){
		perror("listen error");
		exit(1);
	}
    printf("server start!\n");

	while(1)
	{
	    struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
        if(sc < 0)
        {
            perror("accept");
            break;
        }
        else
        {
            if(fork() > 0)  /*������*/
            {
                close(sc); /*�رո����̵Ŀͻ��������׽���*/
            }
            else
            {
                client_process(sc);/*������������*/
            }
        }
	}
	close(ss);
}
