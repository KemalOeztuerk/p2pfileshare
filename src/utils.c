
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <expat.h>
#include "message.h"

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

char* message_to_xml(message *msg) {
    // Allocate memory for the XML string
    size_t xml_size = 1024;
    char *xml_str = (char *)malloc(xml_size);
    if (xml_str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Initialize the XML string
    snprintf(xml_str, xml_size,
             "<message>\n"
             "  <metainfo>\n"
             "    <file_name>%s</file_name>\n"
             "    <sha1_info>%s</sha1_info>\n"
             "    <file_size_in_bytes>%zu</file_size_in_bytes>\n"
             "    <tracker_host>%s</tracker_host>\n"
             "    <tracker_port>%s</tracker_port>\n"
             "  </metainfo>\n"
             "  <peer>\n"
             "    <ip>%s</ip>\n"
             "    <port>%d</port>\n"
             "  </peer>\n"
             "  <flag>%d</flag>\n"
             "</message>\n",
             msg->metainfo->file_name,
             msg->metainfo->sha1_info,
             msg->metainfo->file_size_in_bytes,
             msg->metainfo->tracker_host,
             msg->metainfo->tracker_port,
             msg->peer->ip,
             msg->peer->port,
             msg->flag);

    printf("%s\n",xml_str);
    return xml_str;
}

typedef struct {
    message *msg;
    char current_element[256];
} parse_data;

void XMLCALL start_element(void *data, const char *element, const char **attr) {
    parse_data *pd = (parse_data *)data;
    strncpy(pd->current_element, element, sizeof(pd->current_element) - 1);
}

void XMLCALL end_element(void *data, const char *element) {
    parse_data *pd = (parse_data *)data;
    pd->current_element[0] = '\0';
}

void XMLCALL handle_data(void *data, const char *content, int length) {
    parse_data *pd = (parse_data *)data;
    char *current_element = pd->current_element;
    message *msg = pd->msg;

    char value[256];
    strncpy(value, content, length);
    value[length] = '\0';

    if (strcmp(current_element, "file_name") == 0) {
        strncpy(msg->metainfo->file_name, value, sizeof(msg->metainfo->file_name) - 1);
    } else if (strcmp(current_element, "sha1_info") == 0) {
        strncpy(msg->metainfo->sha1_info, value, sizeof(msg->metainfo->sha1_info) - 1);
    } else if (strcmp(current_element, "file_size_in_bytes") == 0) {
        msg->metainfo->file_size_in_bytes = strtoull(value, NULL, 10);
    } else if (strcmp(current_element, "tracker_host") == 0) {
        strncpy(msg->metainfo->tracker_host, value, sizeof(msg->metainfo->tracker_host) - 1);
    } else if (strcmp(current_element, "tracker_port") == 0) {
        strncpy(msg->metainfo->tracker_port, value, sizeof(msg->metainfo->tracker_port) - 1);
    } else if (strcmp(current_element, "ip") == 0) {
        strncpy(msg->peer->ip, value, sizeof(msg->peer->ip) - 1);
    } else if (strcmp(current_element, "port") == 0) {
        msg->peer->port = atoi(value);
    } else if (strcmp(current_element, "flag") == 0) {
        msg->flag = atoi(value);
    }
}

void xml_to_message(const char *xml_str, message *msg) {
    XML_Parser parser = XML_ParserCreate(NULL);
    parse_data pd;
    pd.msg = msg;
    pd.current_element[0] = '\0';

    msg->metainfo = (metainfo *)malloc(sizeof(metainfo));
    msg->peer = (peer *)malloc(sizeof(peer));
    if (msg->metainfo == NULL || msg->peer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    XML_SetUserData(parser, &pd);
    XML_SetElementHandler(parser, start_element, end_element);
    XML_SetCharacterDataHandler(parser, handle_data);

    if (XML_Parse(parser, xml_str, strlen(xml_str), XML_TRUE) == XML_STATUS_ERROR) {
        fprintf(stderr, "XML Parse error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));
        XML_ParserFree(parser);
        exit(EXIT_FAILURE);
    }

    XML_ParserFree(parser);
}
