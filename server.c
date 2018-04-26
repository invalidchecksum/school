#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
int main (int, char *[]);

int main(int argc, char* argv[]){
	
	if (argc != 2){
		printf ("Usage: %s <port #>\n",argv[0]);
		return 1;
	}

	//get port from args
	int port = atoi (argv[1]);
	//printf ("Using Port: %d\n",port);

	int   n;
	char *p;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char buff[1025];

	// set up
	memset (&serv_addr, '0', sizeof (serv_addr));
	memset (buff, '0', sizeof (buff));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (port);//set port

	// create socket, bind, and listen
	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	bind (listenfd, (struct sockaddr*)&serv_addr, sizeof (serv_addr));
	listen (listenfd, 10);	

	while (1){
		connfd = accept (listenfd, (struct sockaddr*) NULL, NULL);
		//receive data
		char outfile[0];//output file name

		while ( (n = read(connfd, buff,sizeof(buff))) > 0 ){
			//printf ("1: %s\n",buff);
			//printf ("Buffer: %s\n",buff);
			p = buff;
			int c = 0;
			while (*p != '\0'){
				outfile[c++] = *p;//read filename until \0
				p++;
			}
			if (*p == '\0'){//break when the \0 is detected
				break;
			}
		}
		//printf("outfile: %s",outfile);
		FILE *out = fopen(outfile,"w");
		size_t n;
		while ( (n = read(connfd, buff,sizeof(buff))) > 0 ){
			fwrite(buff,1,n,out);//write to file
			//printf("2: %s,%d\n",buff,abc);
		}
		fclose(out);
		close (connfd);
		printf ("\n");
	}
	
	return 0;
}
