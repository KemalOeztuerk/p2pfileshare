
#include "utils.h"
#include <stdio.h>
void *get_in_addr(struct sockaddr *sa){
  if(sa->sa_family==AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr); //ipv4
  }

  return &(((struct sockaddr_in*)sa)->sin_addr); //ipv6

}

size_t get_file_size(char* file_path){
  FILE *fp = fopen(file_path,"rb");
  if(fp == NULL){
    perror("fopen");
    return 0;
  }
  fseek(fp,0,SEEK_END);
  size_t size = ftell(fp);
  return size;
}
