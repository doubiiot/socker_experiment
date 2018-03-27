#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 8887	/*�����˿ڵ�ַ*/
#define BUFFER_SIZE 1024
#define FILE_MAX_SIZE 512
int main(int argc, char *argv[])
{
	int s;		//sΪsocket������
	s = socket(AF_INET, SOCK_STREAM, 0); //����һ����ʽ�׽���
	if(s < 0)
    {
		perror("socket error");
		exit(1);
	}

	struct sockaddr_in server_addr;	//��������ַ�ṹ
    //���÷�������ַ
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;//Э����
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//���ص�ַ
    server_addr.sin_port = htons(PORT);

	//���ӷ�����
	if(connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect error");
        exit(1);
    }

    char file_name[FILE_MAX_SIZE + 1];
    bzero(file_name,sizeof(file_name));
    printf("Please Input File Name On Server:\n");
    scanf("%s", file_name);

    char buffer[BUFFER_SIZE];
    bzero(buffer,sizeof(buffer));
    strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));
    send(s, buffer, BUFFER_SIZE, 0);

//    if(send(s, buffer, BUFFER_SIZE, 0) == -1);
//    {
//        printf("%d\n",errno);
//        perror("send");
//        exit(1);
//    }

    FILE *fp = fopen(file_name,"w");
    if(fp == NULL)
    {
        printf("file %s can not open to write!\n",file_name);
        exit(1);
    }

    bzero(buffer,sizeof(buffer));
    int length = 0;
    while(length = recv(s,buffer,BUFFER_SIZE,0))
    {
        int write_length = fwrite(buffer,sizeof(char),length,fp);
        if(write_length < length)
        {
            printf("write failed!\n");
            break;
        }
        bzero(buffer,BUFFER_SIZE);
    }
    printf("recv file finished!\n");
    fclose(fp);
	close(s);
	return 0;
}


