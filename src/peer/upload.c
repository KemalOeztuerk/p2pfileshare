#include "upload.h"


int upload(){
  int sockfd, client_fd;
  struct addrinfo hints, *servinfo;
  

  memset(&hints, 0, sizeof(hints)); // for security
  
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my ip

  int rv;

  if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0){
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
  char s[INET6_ADDRSTRLEN];
  char buf[MAXDATASIZE];

  //file stuff
  FILE *fp;
  long filesize;
  char *b = malloc(sizeof(char));
  char filename[64];
  
  while(1){   
    sin_size = sizeof (client_addr);

    client_fd = accept ( sockfd, (struct sockaddr*)&client_addr, &sin_size);

    if ( client_fd == -1){
      perror( "server: accept");
      continue; // ???
    }

    if(recv(client_fd, filename, sizeof(filename)-1, 0 ) == -1){ //send command
    perror("server: send");
    continue;
    }
    printf("%s \n ", filename);

 
    inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr*)&client_addr), s, sizeof(s));
    
    //printf("Server got connection from %s\n", s);

    // set filesize
    fp = fopen(filename,"r");
    fseek(fp,0,SEEK_END);
    filesize = ftell(fp);
    rewind(fp);

    //send file
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, MAXDATASIZE, fp)) > 0) {
      if (send(client_fd, buf, bytes_read, 0) == -1) {
	perror("server: send");
	fclose(fp);
	close(client_fd);
	break;
      }
    }
    fclose(fp);
    printf("file has been sent\n");
   
    
   
   
    close(client_fd);
  }

  close(sockfd);
  
  return 0;
    
}

