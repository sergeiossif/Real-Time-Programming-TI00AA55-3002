#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <aio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string.h>
#include <sys/resource.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>

static int socket_fd;
void* tf(void*);
static 	int  client_socket; 
static	int nclient = 0;





void handler1(int sig) {
	if (sig == SIGINT){
		
		printf("writer: error detected, i shall die\n");
		close(socket_fd);
		exit(0);
	}
}


int main(void){
	
	int i;
	

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	pid_t pid;
	int status;
	
	pthread_t id[10];
	
	struct sockaddr_un saddru;
	signal(SIGINT, handler1);
	//memset(&saddru, 0, sizeof(saddru));
	
	saddru.sun_family = AF_UNIX;
	strcpy(saddru.sun_path, "server.sock");
	
	bind(socket_fd, (const struct sockaddr*)&saddru, sizeof(saddru));
	
	listen(socket_fd, 5);
	printf("waiting yo\n");
	
	
	printf("reading yo\n");
	/*
	while(1){
		
		if(nclient == 0)
		exit(0);
		client_socket = accept(socket_fd, NULL, 0);
		printf("client accepted \n");
		pthread_create(&id[nclient++], NULL, tf, (void*)&client_socket);
		

		
		sleep(1);
		
		
	}*/
	
	do {
		if(nclient == 0)
		exit(0);
		client_socket = accept(socket_fd, NULL, 0);
		printf("client accepted \n");
		pthread_create(&id[nclient++], NULL, tf, (void*)&client_socket);
		

		
		sleep(1);
		} while(nclient != 0)
	
	
	
	
	return 0;
}



void* tf(void* p) {
	
	char buf[4];
	int	delay;
	char letter;
	char sbuf;
	int i;
	int client_socket = *(int*)p;
	printf("thread server is created, socket is %d\n", client_socket);
	if(read(client_socket, buf, 4) > 0) {
		//buf[4] = '\0';
		printf("fork: received %s \n", buf);
		sscanf(buf, "%c %d", &letter, &delay);
		//printf("formatted %c %d\n", letter, delay);
		printf("writing %c with delay %d\n", letter, delay);
		for(i=0;i<10;i++){
			sbuf = letter;
			printf("fork: sending %c\n", letter);
			write(client_socket, &sbuf, 1);
			sleep(delay);
		}
		nclient--;
		printf("Closing fork %d, %d remaining\n", client_socket, nclient);
		close(client_socket);
		
		return NULL;
	} else {
		printf("error");
		close(client_socket);
		return NULL;
	}


}