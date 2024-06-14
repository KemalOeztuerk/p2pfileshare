

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "utils.h"
#include "tcp_server.h"

#define BACKLOG 10

int start_tcp_server(char* port, handler_t handler,void *args){
  int sockfd, client_fd;
  struct addrinfo hints, *servinfo;

  memset(&hints, 0, sizeof(hints)); // for security
  
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my ip

  int rv;

  
  if((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0){
    fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(rv));
    exit(1);
  }

  struct addrinfo *p;
  for (p = servinfo; p!=NULL; p=p->ai_next ){ // set the socket

    if( (sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1 ){ //!!
      perror("server: socket");
      continue;
    }
    
    int yes = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1){ //!!
      perror("server: setsockopt");
      exit(1);
    }

    if(bind(sockfd,p->ai_addr,p->ai_addrlen)==-1){ //!!
      perror("server: bind");
      continue;
    }
   
    break;
  }

  freeaddrinfo(servinfo);

  if(p == NULL){
    fprintf(stderr,"server failed to bind\n");
    exit(1);
  }

  if(listen(sockfd,BACKLOG)==-1){
    perror("server: listen");
    exit(1);
  }

  

  printf("waiting for connections...\n");

  struct sockaddr_storage client_addr;
  socklen_t sin_size;
  // char s[INET6_ADDRSTRLEN];
  
  while(1){   
    sin_size = sizeof (client_addr);

    client_fd = accept ( sockfd, (struct sockaddr*)&client_addr, &sin_size);

    if ( client_fd == -1){
      perror( "server: accept");
      continue; // ???
    }

    handler(client_fd, args);
        
    close(client_fd);
  }

  close(sockfd);
  
  return 0;
    
}
