#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <iostream>
#include <arpa/inet.h>
#define PORT 9078

using namespace std;

char	buffer[BUFSIZ];
char rBuffer[BUFSIZ];
void* do_(void * dat);
class Thread
{
	public:
	int c_socket;
	void run()
	{
		thread thr(do_,&c_socket);
		thr.detach();
		//thr.join();
		printf("detach thread\n");
	}
};




int main( )
{
	int	 s_socket;
	struct sockaddr_in s_addr, c_addr;

	socklen_t	len;
	int	n;


	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) {
		printf("Can not Bind\n");
		return -1;
	}
	
	if(listen(s_socket, 5) == -1) 
	{
					printf("listen Fail\n");
					return -1;
	}


	
	vector<int> c_socket;
	vector<Thread> thr;
	int i=0;
	
	
	len = sizeof(c_addr);
	
	while(1)
	{
		printf("[main]before accepting c_socket\n");
		int temp=accept(s_socket, (struct sockaddr *) &c_addr, &len);
		c_socket.push_back(temp);
		printf("[main]after accepting c_socket\n");
		Thread temp_thr;
		temp_thr.c_socket=c_socket[i];
		printf("[main]making Thread class\n");
		thr.push_back(temp_thr);
		printf("[main]before running thread\n");
		thr[i].run();
		printf("[main]after running thread %d\n",i+1);
		i++;
	}


//close(c_socket);
	close(s_socket);
	return 0;
}

void* do_(void * dat)
{
	int csocket=*(int*)dat;
	int n=0;
	
		memset(rBuffer,'\0',sizeof(rBuffer));
		n=read(csocket,rBuffer,sizeof(rBuffer));
		if(n>0)
			printf("[thread]from client : %s\n",rBuffer);
		close(csocket);
		printf("[thread] close c socket\n");

}
