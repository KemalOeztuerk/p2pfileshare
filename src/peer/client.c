
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../tcp_client.h"
#include "../peer.h"
#include "../metainfo.h"
#include "talk_tracker.h"
#include "download.h"
#include "upload.h"
#include "../tcp_server.h"
#include <pthread.h>

#define DEFAULT_TRACKER_PORT "5555"
#define DEFAULT_OUTPUT_PATH "files/out.metainfo"
void print_usage(const char *prog_name) {
  fprintf(stderr, "Usage: %s [-a] [-q] -f <file_path> -n <file_name> -t <tracker_host> -p <tracker_port> -o <output_path>\n", prog_name);
  fprintf(stderr, "  -a  Announce file\n");
  fprintf(stderr, "  -q  Query peers\n");
  fprintf(stderr, "  -f  File path\n");
  fprintf(stderr, "  -n  File name\n");
  fprintf(stderr, "  -t  Tracker host\n");
  fprintf(stderr, "  -p  Tracker port\n");
  fprintf(stderr, "  -o  Output path (for announce) or metainfo path (for query)\n");
}

int main(int argc, char *argv[]) {

  
  // create a new process for upload 
  int pid = fork();
  if(pid == -1){
    perror("fork: ");
    return -1;
  }else if(pid == 0){
    tcp_server_args sv_args={"6666",upload_handler,NULL };
    start_tcp_server(&sv_args);
    return 0;
  }
  
  int opt;
  int announce_flag = 0, query_flag = 0;
  char *file_path = NULL, *file_name = NULL, *tracker_host = NULL, *tracker_port = DEFAULT_TRACKER_PORT, *output_path = DEFAULT_OUTPUT_PATH;

  while ((opt = getopt(argc, argv, "aqf:n:t:p:o:")) != -1) {
    switch (opt) {
    case 'a':
      announce_flag = PUBLISH;
      break;
    case 'q':
      query_flag = DOWNLOAD;
      break;
    case 'f':
      file_path = optarg;
      break;
    case 'n':
      file_name = optarg;
      break;
    case 't':
      tracker_host = optarg;
      break;
    case 'p':
      tracker_port = optarg;
      break;
    case 'o':
      output_path = optarg;
      break;
    default:
      print_usage(argv[0]);
      return EXIT_FAILURE;
    }
  }
 
  if ((!announce_flag && !query_flag) || !file_path || !file_name || !tracker_host ) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }
    
  if (announce_flag) {
    
    metainfo *mi = malloc(sizeof(metainfo));
    printf("file path:%s %s %s %s\n",file_path,file_name,tracker_host,tracker_port);
    if(( mi = init_metainfo(file_path, file_name, tracker_host,tracker_port)) == NULL){
      perror("client: metainfo: ");
      return EXIT_FAILURE;
    }
 
    if (create_metainfo_file(output_path, mi) != 0) {
      //free_metainfo(mi);
      return EXIT_FAILURE;
    }

    // announce to tracker server to publish file
    if (start_tcp_client(tracker_host, tracker_port, announce_file, mi) == NULL) {
      fprintf(stderr, "Failed to announce file\n");
      return EXIT_FAILURE;
    }
    printf("File announced successfully.\n");
  } else if (query_flag) { // query tracker server to get peer information of uploading peer
    metainfo *mi_query = malloc(sizeof(metainfo));
    read_metainfo_file(output_path, mi_query);	
    printf("Metainfo: %s %s %s %lu\n", mi_query->file_name, mi_query->tracker_host, mi_query->tracker_port, mi_query->file_size_in_bytes);

    //	printf("%s %lu %s %s\n ",mi->file_name,mi->file_size_in_bytes,mi->tracker_host,mi->tracker_port);
    if (!mi_query) {
      fprintf(stderr, "Failed to read metainfo file\n");
      return EXIT_FAILURE;
    }
    //get peer information from the tracker
    message *msg = (message *)start_tcp_client(mi_query->tracker_host, mi_query->tracker_port, request_peer, mi_query);
    if (msg) {
      printf("Peer found: %s:%d\n", msg->peer->ip, msg->peer->port);
      char port_str[6];
      sprintf(port_str,"%d",msg->peer->port);
      start_tcp_client(msg->peer->ip, port_str , download_handler, (void*)msg);
      //free(msg);
    } else {
      fprintf(stderr, "Failed to get peer information\n");
      return EXIT_FAILURE;
    }
  }

  //free_metainfo(mi);
  
  return EXIT_SUCCESS;
}
