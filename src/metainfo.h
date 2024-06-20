#ifndef METAINFO_H
#define METAINFO_H

#include <stdio.h>

#define SHA1_SIZE 20
#define PUBLISH 1
#define DOWNLOAD 2
typedef struct metainfo_t{
  char file_name[128];
  char sha1_info[20];
  size_t file_size_in_bytes;
  char tracker_host[128];
  char tracker_port[8];
  int flag;
  
}metainfo;

metainfo *init_metainfo(char* file_path,char* filename, char* tracker_host, char *tracker_port);
int create_metainfo_file(char* dest_path, metainfo* mi);
int read_metainfo_file(char *metainfo_path, metainfo* mi_out);

void free_metainfo(metainfo *mi);

#endif
