#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
int main (int, char *[]);

//argv[1] = infile
//argv[2] = outfile
//argv[3] = server IP
//argv[4] = server port #
int main (int argc, char* argv[]){
	if (argc != 5){
		printf("Usage: %s <infile> <outfile> <ipAddr> <port #>\n", argv[0]);
		return 1;
	}
	int port = atoi (argv[4]);
	int ipaddr = atoi (argv[3]);
	
	int i;
	int sockfd = 0, n = 0;
	char buff[10];
	char *p;
	struct sockaddr_in serv_addr;
	
	// set up
	memset (buff, '0', sizeof (buff));
	memset (&serv_addr, '0', sizeof (serv_addr));
	
	// open socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
		printf ("Error : Could not create socket \n");
		return 1;
	}

	// set address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons (port);
	if (inet_pton (AF_INET, argv[3], &serv_addr.sin_addr) <= 0){
		printf ("inet_pton error occured\n");
		return 1;
	}

	// connect
	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0){
		printf ("Error : Connect Failed \n");
		return 1;
	}
	
	//Connection open, transfer file

	FILE *in;
	in = fopen(argv[1],"r");//open src file
	size_t data;
	//while ( scanf("%s", buff) == 1 ){write (sockfd, buff, strlen (buff) + 1);}
	
	//send outfile name
	//argv[2] = strcat(argv[2], '\0');//concat \0 to end of outfile name
	write (sockfd, argv[2], strlen(argv[2]));//send filename
	write (sockfd, '\0',1);//send \0

	//send file in 10 byte increments
	while ( (data = fread(buff,1,10,in)) > 0 ){
		//fwrite(buffer,1,data,out);//write the 10 bytes to file
		//printf ("%d\n",data);
		//write (sockfd, buff, strlen (buff) + 1);
		write (sockfd, buff, data);//send file contents
	}

	fclose(in);
	close (sockfd);

	return 0;
}

