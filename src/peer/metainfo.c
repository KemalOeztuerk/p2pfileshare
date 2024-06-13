
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "metainfo.h"
#include "utils.h"

metainfo* create_metainfo(char* file_path,char* file_name, char* tracker_host){

  unsigned char buffer[1024]; // buffer to read
  
  FILE *fp = fopen(file_path,"rb");
  if(fp == NULL){
    perror("fopen");
    return NULL; 
  }

  SHA_CTX sha1;
  SHA1_Init(&sha1);
  unsigned char output[SHA_DIGEST_LENGTH];

  size_t bytes_read;
  while((bytes_read = fread(buffer,1,sizeof(buffer),fp))!=0){
    SHA1_Update(&sha1, buffer, bytes_read);
  }
  
  SHA1_Final(output,&sha1);
  fclose(fp);

  metainfo *mi = (metainfo*)malloc(sizeof(metainfo));

  if(mi == NULL){
    perror("malloc");
    return NULL;
  }

  strncpy(mi->file_name, file_name, sizeof(mi->file_name)-1);
  mi->file_name[sizeof(mi->file_name)-1] = '\0';
  
  memcpy(mi->sha1_info,output,SHA_DIGEST_LENGTH);
  mi->file_size_in_bytes = get_file_size(file_path);

  strncpy(mi->tracker_host, tracker_host, sizeof(mi->tracker_host));
  
  return mi;
  
}

void free_metainfo(metainfo *mi){
  if(mi){
    free(mi);
  }
}
