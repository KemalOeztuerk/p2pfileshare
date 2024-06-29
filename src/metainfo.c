
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "metainfo.h"
#include "utils.h"

metainfo *init_metainfo(char* file_path,char* file_name, char* tracker_host,char *tracker_port){

  unsigned char buffer[1024]; // buffer to read
  
  FILE *fp = fopen(file_path,"rb");
  if(fp == NULL){
    perror("metainfo fopen");
    return NULL; 
  }

  //  calculate hash of the file in byte level
  SHA_CTX sha1;
  SHA1_Init(&sha1);
  unsigned char hash[SHA_DIGEST_LENGTH]; // byte 
  char hash_str[SHA_DIGEST_LENGTH*2+1];// hexadecimal

  size_t bytes_read;
  while((bytes_read = fread(buffer,1,sizeof(buffer),fp))!=0){
    SHA1_Update(&sha1, buffer, bytes_read);
  }
  
  SHA1_Final(hash,&sha1);
  fclose(fp);

  for(int i = 0; i<SHA_DIGEST_LENGTH; i++){
    sprintf(&hash_str[i*2],"%02x",hash[i]);
  }

  

  

  metainfo *mi = (metainfo*)malloc(sizeof(metainfo));

  if(mi == NULL){
    perror("metainfo malloc");
    return NULL;
  }

  strncpy(mi->file_name, file_name, sizeof(mi->file_name)-1);
  mi->file_name[sizeof(mi->file_name)-1] = '\0';
  
  memcpy(mi->sha1_info,hash_str,SHA_DIGEST_LENGTH*2+1);
  mi->file_size_in_bytes = get_file_size(file_path);

  strncpy(mi->tracker_host, tracker_host, sizeof(mi->tracker_host));
  strncpy(mi->tracker_port, tracker_port, sizeof(mi->tracker_port));

  return mi;
  
}

int create_metainfo_file(char *dest_path, metainfo* mi){
  FILE *fp;
  fp = fopen(dest_path,"wb");
  if(fp == NULL){
    perror("metainfo fopen");
    return -1;
  }

  if(fwrite(mi,sizeof(metainfo),1,fp)!= 1){
    perror("metainfo: fwrite");
    fclose(fp);
    return -1;
  }

  fclose(fp);
  
  return 0;
}

int read_metainfo_file(char *metainfo_path, metainfo* mi_out) {
  FILE *fp = fopen(metainfo_path, "rb");
  if (fp == NULL) {
    perror("metainfo fopen");
    return -1;
  }

  if (fread(mi_out, sizeof(metainfo), 1, fp) != 1) {
    perror("metainfo fread");
    fclose(fp);
    return -1;
  }

  fclose(fp);
  return 0;
}

void free_metainfo(metainfo *mi){
  if(mi){
    free(mi);
  }
}
