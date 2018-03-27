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
    while(1)
    {
        char buffer[BUFFER_SIZE];
        bzero(buffer,sizeof(buffer));

        char input_buffer[FILE_MAX_SIZE + 1];
        bzero(input_buffer,sizeof(input_buffer));
        printf("send:\n");
        scanf("%s", input_buffer);

        if(send(s, input_buffer, BUFFER_SIZE, 0)<0)
        {
            perror("send");
            exit(1);
        }
        if(recv(s,buffer,BUFFER_SIZE,0)<0)
        {
            perror("recv");
            exit(1);
        }
        printf("recv from server:%s\n",buffer);

    }
}







