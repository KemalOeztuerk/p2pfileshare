
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "../tcp_client.h"
#include "../peer.h"
#include "../metainfo.h"
#include "../message.h"
#include "../utils.h"

void *announce_file(int sockfd, void *args) {
  printf("announce!\n");
  metainfo *mi = (metainfo *)args;
  //TODO fix port and ip
  peer *p = init_peer("127.0.0.1", 6666);
  message *msg = init_message(mi, p, PUBLISH);
  // ??
  
  char *xml_str = message_to_xml(msg);
  if (send(sockfd, xml_str, strlen(xml_str), 0) == -1) {
    perror("talk_tracker: send");
    return NULL;
  }
  
  message testmsg;
  xml_to_message(xml_str, &testmsg);
  printf("test message flag: %d \n",testmsg.flag);
  printf("test message metainfo: filename:%s size:%lu hash:%s ip:%s :port%s \n",testmsg.metainfo->file_name,
	 testmsg.metainfo->file_size_in_bytes,
	 testmsg.metainfo->sha1_info,
	 testmsg.metainfo->tracker_host,
	 testmsg.metainfo->tracker_port);
  
  printf("test message peerinfo:%s %d\n", testmsg.peer->ip, testmsg.peer->port);

  char buf[128];
  if (recv(sockfd, buf, sizeof(buf), 0) == -1) {
    perror("talk_tracker: recv");
    return NULL;
  }
  printf("talk_tracker %s\n", buf);
  return (void*)1;
}

void *request_peer(int sockfd, void *args) {
  printf("request!\n");
  metainfo *mi = (metainfo*)args;
  peer *p = init_peer("127.0.0.1", 5555);
  message *msg = init_message(mi, p, DOWNLOAD);
  
  if (p == NULL) {
    perror("client: malloc");
    //free(msg);
    return NULL;
  }
  char *xml_str = message_to_xml(msg);
  if (send(sockfd, xml_str, strlen(xml_str), 0) == -1) {
    perror("client: send");
    free(msg);
    //free(p);
    return NULL;
  }

  printf("Sent request to server\n");
  size_t buffer_size = 10000;
  char *buffer = malloc(buffer_size);
  if (recv(sockfd, buffer, buffer_size, 0) == -1) {
    perror("talk_tracker: recv");
    //free(msg);
    //free(p);
    return NULL;
  }
  //printf("response_message\n %s\n",buffer);
  message *res_msg = malloc(sizeof(message));
  xml_to_message(buffer,res_msg);
  printf("Received peer: %s:%d\n", res_msg->peer->ip, res_msg->peer->port);
  //free(msg);
  return (void*)res_msg;
}
