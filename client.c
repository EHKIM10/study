#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#define PORT 9078
#define IPADDR "127.0.0.1"
struct data
{
	int dp;
	int mode; //if mode is 1, it means "do_kbd_input()".
		//if mode is 2, it means "do_read()" 
};
char	rBuffer[BUFSIZ];
char buffer[BUFSIZ];
void* do_(void * dat)
{
	struct data temp=*(struct data*)dat;
	
		printf("input : ");
		scanf("%s",buffer);
		printf("\n");				
		write(temp.dp, buffer, strlen(buffer));
		memset(buffer,'\0',sizeof(buffer));
	
	
}
/*
void *do_kbd_input(void *data)
{
		scanf("%s",buffer);				
		write(*(int *)data, buffer, strlen(buffer));
		memset(buffer,'\0',sizeof(buffer));
}

void *do_read(void *data)
{
	int n=0;
	memset(rBuffer,'\0',sizeof(rBuffer));
	n=read(*(int *)data,rBuffer,sizeof(rBuffer));
	if(n>0)
		printf("from server : %s\n",rBuffer);
}
*/

int main( )
{
	int	c_socket;
	struct sockaddr_in c_addr;
	struct data d[2];	
	int	len;
	int	n;
	//pid_t pid;
	pthread_t pthread[2];

	c_socket = socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);
	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr))==-1) {
				printf("Can not connect\n");
				close(c_socket);
				return -1;
			}	
	d[0].dp=c_socket;
	//d[1].dp=c_socket;
	d[0].mode=1;
	//d[1].mode=2;

	pthread_create(&pthread[0],NULL,do_,(void*)&d[0]);
	pthread_join(pthread[0],0);
		
	//pthread_create(&pthread[1],NULL,do_,(void*)&d[1]);
	
/*
pid= fork();
	while(1)
	{
	//pid= fork();
	switch(pid)
	{
		case 0:  //자식
			scanf("%s",buffer);				
			write(c_socket, buffer, strlen(buffer));
			break;
		case -1:
			fprintf(stderr," Error\n");
			break;
		default: //부모
			memset(rBuffer,'\0',sizeof(rBuffer));
			n=read(c_socket,rBuffer,sizeof(rBuffer));
			printf("p\n");
			if(n>0)
				printf("from server : %s\n",rBuffer);
			break;
		}

	}
*/
	close(c_socket);
	return 0;
}
