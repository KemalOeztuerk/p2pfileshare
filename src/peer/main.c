
#include <stdio.h>
#include <pthread.h>

#include "../tcp_server.h"
#include "../tcp_client.h"
#include "download.h"
#include "upload.h"
#include "metainfo.h"

//gcc -o ../../bin/client ../tcp_client.c ../tcp_server.c download.c upload.c metainfo.c ../utils.c main.c -lssl -lcrypto

int main(int argc, char *argv[]){

  if(argc !=5){
    printf("usage: main <file_path> <file_name> <tracker_host> <output_path>\n");
    return -1;
  }
  
  metainfo *mi = init_metainfo(argv[1],argv[2],argv[3]);
  create_metainfo_file(argv[4],mi);

  metainfo *mi2 = malloc(sizeof(metainfo));
  read_metainfo_file(argv[4],mi2);
 //printf("name:%s\n hash:%s \n size:%lu \nhost %s\n", mi2->file_name,mi2->sha1_info,mi2->file_size_in_bytes,mi2->tracker_host);
  // start_tcp_server("5555",upload_handler);
  //  start_tcp_client(argv[1],);
  //download(argv[1],argv[2]);

  pid_t pid = fork();

  if(pid == -1){
    perror("fork");
    return -1;
  }
  else if(pid == 0){
        printf("server has started\n");

    start_tcp_server("5555",upload_handler,NULL);
  }
   
  else{
        printf("client has started\n");

     start_tcp_client("127.0.0.1","5555",download_handler,mi2);
  }
    //  start_tcp_client("127.0.0.1","5555",download_handler,mi2);w
  
  return 0;
}
