#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define UNIXEPOCH 2208988800UL
#define BUFFER_SIZE 1024
#define MSG "What time is it ?\n" 

extern int errno;

int connectsock(const char *host, const char *service, const char *transport)
{
    struct hostent * phe;
    struct servent *pse;
    struct protoent *ppe;
    struct sockaddr_in sin;
    int s,type;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    if(pse = getservbyname(service, transport))
    {
        sin.sin_port = pse->s_port;
    }
    else if((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
    {
        printf("can not get \" %s \" service entry . \n", service);
        return -1;
    }
    if(phe = gethostbyname(host))
    {
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    }
    else if((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
    {
        printf("can not get \" %s \" host entry .", host);
        return -1;
    }
    if((ppe = getprotobyname(transport)) == 0)
    {
        printf("can not get \" %s \" protocol entry . \n", transport);
        return -1;
    }
    if(strcmp(transport, "udp") == 0)
    {
        type= SOCK_DGRAM;
    }
    else
    {
        type= SOCK_STREAM;
    }
    s=socket(PF_INET, type, ppe->p_proto);
    if(s < 0)
     {
    printf("can not create socket : %s \n", strerror(errno));
        return -1;
     }
    if(connect(s,(struct sockaddr *)&sin, sizeof(sin))<0)
     {
        printf("can not connect to %s .\n",host, service);
        return -1;
     }
      return s;
}
int connectUDP(const char *host, const char * service)
{
    return connectsock(host,service,"udp");
}
int connectTCP(const char *host, const char * service)
{
    return connectsock(host, service, "tcp");
}

int main(int argc, char *argv[])
{
    char *host="localhost";
    char *service="time";
    char buf[BUFFER_SIZE];
    time_t now;
    int sock_fd,length;
    
    switch(argc)
    {
    case 1:
        host = "localhost";
        break;
    case 3:
        service = argv[2];
    case 2:
        host = argv[1];
        break;
    default:
        printf("port or host is not valid . \n");
        return -1;
    }

    sock_fd = connectUDP(host, service);
    write(sock_fd,MSG,strlen(MSG));
    length = read(sock_fd,buf,sizeof(buf));
    if(length < 0)
     {
        printf("read fail: %s .\n",strerror);
        return -1;
     }
    now = ntohl((unsigned long)strtoul(buf,NULL,10)-UNIXEPOCH);
    printf("time is %s",ctime(&now));
    return 0;
}
