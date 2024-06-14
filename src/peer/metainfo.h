#ifndef METAINFO_H
#define METAINFO_H

#include <stdio.h>

#define SHA1_SIZE 20
typedef struct metainfo_t{
  char file_name[128];
  char sha1_info[20];
  size_t file_size_in_bytes;
  char tracker_host[128];
  
}metainfo;

metainfo *init_metainfo(char* file_path,char* filename, char* tracker_host);
int create_metainfo_file(char* dest_path, metainfo* mi);
int read_metainfo_file(char *metainfo_path, metainfo* mi_out);

void free_metainfo(metainfo *mi);

#endif
