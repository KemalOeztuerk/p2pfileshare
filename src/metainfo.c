
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "metainfo.h"
#include "utils.h"



metainfo *init_metainfo(char* file_path,char* file_name, char* tracker_host,char *tracker_port){

  metainfo *mi = (metainfo*)malloc(sizeof(metainfo));

  if(mi == NULL){
    perror("init_metainfo malloc");
    return NULL;
  }

  char *hash_str = hash_file(file_path);
  printf("metainfo hash: %s\n",hash_str);
  strncpy(mi->file_name, file_name, sizeof(mi->file_name));
  //  mi->file_name[sizeof(mi->file_name)-1] = '\0';
  
  memcpy(mi->sha1_info,hash_str,SHA1_STR_SIZE);
  mi->file_size_in_bytes = get_file_size(file_path);

  strncpy(mi->tracker_host, tracker_host, sizeof(mi->tracker_host));
  strncpy(mi->tracker_port, tracker_port, sizeof(mi->tracker_port));

  
  
  return mi;
  
}

int create_metainfo_file(char *dest_path, metainfo* mi){
  FILE *fp;
  fp = fopen(dest_path,"wb");
  if(fp == NULL){
    perror("create_metainfo_file: fopen");
    return -1;
  }

  if(fwrite(mi,sizeof(metainfo),1,fp)!= 1){
    perror("create_metainfo_file: fwrite");
    fclose(fp);
    return -1;
  }

  fclose(fp);
  
  return 0;
}

int read_metainfo_file(char *metainfo_path, metainfo* mi_out) {
  FILE *fp = fopen(metainfo_path, "rb");
  if (fp == NULL) {
    perror("init_metainfo fopen");
    return -1;
  }

  if (fread(mi_out, sizeof(metainfo), 1, fp) != 1) {
    perror("init_metainfo fread");
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
