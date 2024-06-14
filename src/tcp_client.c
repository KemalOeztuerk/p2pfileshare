
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "utils.h"
#include "tcp_client.h"
#define MAXDATASIZE 1024


int start_tcp_client(char *hostname, char* port, handler_t handler, void *args){
  
  int sockfd,rv;
  char s[INET_ADDRSTRLEN];
  struct addrinfo hints, *servinfo, *p;

  memset(&hints,0,sizeof(hints));
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_STREAM;


  if( (rv=getaddrinfo(hostname,port,&hints,&servinfo)) == -1){
    fprintf(stderr, "client: getaddrinfo, %s", gai_strerror(rv));
    return 1;
  }

  for(p=servinfo; p!=NULL; p=p->ai_next){
    if((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
      perror("client: error");
      continue;
    }
    if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1){
      perror("client: connect");
      close(sockfd);
      continue;
    }

    break;
  }

  if(p==NULL){
    perror("client failed to connect\n");
    return 2;
  }

  inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof(s));
  printf("client: connecting to %s\n",s);
  freeaddrinfo(servinfo);

  handler(sockfd, args);
  close(sockfd);
  
  return 0;
}

