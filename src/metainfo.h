#ifndef METAINFO_H
#define METAINFO_H

#include <stdio.h>
#include <openssl/sha.h>

#define SHA1_STR_SIZE (SHA_DIGEST_LENGTH * 2+1)
#define PUBLISH 1
#define DOWNLOAD 2
#define SUCCESS 3
#define FAILURE 4
typedef struct metainfo_t{
  char file_name[256];
  char sha1_info[SHA1_STR_SIZE];
  size_t file_size_in_bytes;
  char tracker_host[256];
  char tracker_port[6];
  
}metainfo;

metainfo *init_metainfo(char* file_path,char* filename, char* tracker_host, char *tracker_port);
int create_metainfo_file(char* dest_path, metainfo* mi);
int read_metainfo_file(char *metainfo_path, metainfo* mi_out);

void free_metainfo(metainfo *mi);

#endif
