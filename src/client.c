#include "client.h"

int download(char *hostname, char *filename){
  
  int sockfd, numbytes,rv;
  char buf[MAXDATASIZE], s[INET_ADDRSTRLEN];
  struct addrinfo hints, *servinfo, *p;

  memset(&hints,0,sizeof(hints));
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype=SOCK_STREAM;


  if( (rv=getaddrinfo(hostname,PORT,&hints,&servinfo)) == -1){
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

  if(send(sockfd, filename, sizeof(filename), 0 ) == -1){ //send command
    perror("client: send");
    exit(1);
  }

  //char *b = malloc(sizeof(char));


  FILE *fp = fopen("filefromserver","w");


  do {
    // Receive data into buffer
    numbytes = recv(sockfd, buf, MAXDATASIZE, 0);
    if (numbytes == -1) {
      perror("client: recv");
      exit(1);
    } else if (numbytes == 0) {
      // Connection closed by server
      break;
    }

    // Write received data to file
    size_t bytes_written = fwrite(buf, 1, numbytes, fp);
    if (bytes_written != numbytes) {
      if (ferror(fp)) {
	perror("fwrite");
	exit(1);
      }
    }
  } while (numbytes > 0);

  printf("recieved the file.\n");

  
  fclose(fp);
  close(sockfd);
  
  return 0;
}

