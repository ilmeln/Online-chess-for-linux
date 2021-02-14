#include "Window.h"
#include <gtkmm/application.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
	int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	if(argc<3)
		return 1;
	portno = atoi(argv[2]); // порт сервера
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return 2;
	server = gethostbyname(argv[1]); //ip адрес сервера
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return 3;
    }
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        return 4;
	char ch;
	read(sockfd,&ch,sizeof(ch));
  	auto app = Gtk::Application::create();

  	Window window(ch,sockfd);

  	return app->run(window);
}
