#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>



void createServer(int & sockfd, const int portno) {

    socklen_t serlen;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       return;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    serlen = sizeof(serv_addr);
    int err =   bind( sockfd,
                  (struct sockaddr *) &serv_addr,
                  serlen );
   if(err < 0)
        printf("ERROR on binding, trying to bind another port..\n");
   serv_addr.sin_port = 0;
   if(err < 0)
       err =   bind( sockfd,
                     (struct sockaddr *) &serv_addr,
                     serlen );
   if (err < 0)
        perror("ERROR on binding socket\n");
   err = getsockname(sockfd,
             (struct sockaddr *) &serv_addr,
             &serlen);
   if (err < 0)
       perror("ERROR on receiving info about server socket\n");
   printf("Server port is: %d\n", ntohs(serv_addr.sin_port) );
}


void* connect(void* arg)
{
	int *m = static_cast<int*>(arg);
	char buf[6];
	while(true) {
		read(m[0],(char*)&buf,sizeof(buf));
		write(m[1],buf,sizeof(buf));
		if(buf[0]=='E')
			pthread_exit(0);
	}
}



int main(int argc, char *argv[])
{

    if (argc != 2) {
		return 1;
    }
	int portno = atoi(argv[1]); // порт сервера
    int sockfd = 0;
    createServer(sockfd, portno);
    printf("Waiting for connection...\n");

    listen(sockfd,5);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (newsockfd < 0)
         return 1;
	write(newsockfd,"W",sizeof("W"));
	struct sockaddr_in cli_addr1;
    socklen_t clilen1 = sizeof(cli_addr1);
    int newsockfd1 = accept(sockfd,
                (struct sockaddr *) &cli_addr1,
                &clilen1);
    if (newsockfd1 < 0)
         return 2;

	write(newsockfd1,"B",sizeof("B"));


	int m1[2] = {newsockfd,newsockfd1};
	int m2[2] = {newsockfd1,newsockfd};
	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1,0,connect,m1);
	pthread_create(&tid2,0,connect,m2);
	pthread_join(tid1,0);
	pthread_join(tid2,0);
	return 0;
}


