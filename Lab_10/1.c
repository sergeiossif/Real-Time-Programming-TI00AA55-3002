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

void handler1(int sig) {
	if (sig == SIGINT){
		
		printf("writer: error detected, i shall die\n");
		close(socket_fd);
		exit(0);
	}
}


int main(void){
	
	int  client_socket; 
	char buf[4];
	char sbuf;
	int i;
	char letter;
	int	delay;
	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	pid_t pid;
	int status;
	
	struct sockaddr_un saddru;
	signal(SIGINT, handler1);
	//memset(&saddru, 0, sizeof(saddru));
	
	saddru.sun_family = AF_UNIX;
	strcpy(saddru.sun_path, "server.sock");
	
	bind(socket_fd, (const struct sockaddr*)&saddru, sizeof(saddru));
	
	listen(socket_fd, 5);
	printf("waiting yo\n");
	
	
	printf("reading yo\n");
	
	while(1){
		
		client_socket = accept(socket_fd, NULL, 0);
		printf("client accepted \n");
		pid = fork();
		
		if(pid > 0){
			printf("parent: closing client socket \n");
			close(client_socket);
			while(waitpid(-1, &status, WNOHANG));
			continue;
		}
		else if(pid==0){
			printf("fork server is created\n");
			if(read(client_socket, buf, 4) > 0) {
				//buf[4] = '\0';
				printf("fork: received %s \n", buf);
				sscanf(buf, "%c %d", &letter, &delay);
				//printf("formatted %c %d\n", letter, delay);
				printf("writing %c with delay %d\n", letter, delay);
				for(i=0;i<10;i++){
					sbuf = letter;
					//printf("fork: sending %c\n", letter);
					write(client_socket, &sbuf, 1);
					sleep(delay);
				}
				close(client_socket);
				exit(0);
			} else {
				printf("error");
				close(client_socket);
				exit(0);
			}
		}
		
		/*
		if(read(client_socket, buf, 3)==3)
			buf[4] = '\0';
			printf("received %s \n", buf);
			sscanf(buf, "%c %d", &letter, &delay);
			printf("formatted %c %d\n", letter, delay);
			
		for(i=0;i<10;i++){
			sbuf = 'd';
			write(client_socket, &sbuf, 1);
			sleep(4);
		}*/
	}
	
	
	
	
	return 0;
}