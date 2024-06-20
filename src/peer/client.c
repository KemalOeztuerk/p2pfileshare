
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../tcp_client.h"
#include "../peer.h"
#include "../metainfo.h"
#include "talk_tracker.h"

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
    int opt;
    int announce_flag = 0, query_flag = 0;
    char *file_path = NULL, *file_name = NULL, *tracker_host = NULL, *tracker_port = NULL, *output_path = NULL;

    while ((opt = getopt(argc, argv, "aqf:n:t:p:o:")) != -1) {
        switch (opt) {
            case 'a':
                announce_flag = 1;
                break;
            case 'q':
                query_flag = 1;
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

    if ((!announce_flag && !query_flag) || !file_path || !file_name || !tracker_host || !tracker_port || !output_path) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    metainfo *mi = malloc(sizeof(metainfo));
    if (announce_flag) {
      mi = init_metainfo(file_path, file_name, tracker_host,tracker_port);
        create_metainfo_file(output_path, mi);
		printf("%s %lu %s %s %d\n ",mi->file_name,mi->file_size_in_bytes,mi->tracker_host,mi->tracker_port,mi->flag);

        if (start_tcp_client(tracker_host, tracker_port, announce_file, mi) == NULL) {
            fprintf(stderr, "Failed to announce file\n");
            return EXIT_FAILURE;
        }
        printf("File announced successfully.\n");
    } else if (query_flag) {
        metainfo *mi_query = malloc(sizeof(metainfo));
        read_metainfo_file(output_path, mi_query);
	printf("%s %lu %s %s %d ",mi->file_name,mi->file_size_in_bytes,mi->tracker_host,mi->tracker_port,mi->flag);
        if (!mi_query) {
            fprintf(stderr, "Failed to read metainfo file\n");
            return EXIT_FAILURE;
        }
        peer *p = (peer *)start_tcp_client(tracker_host, tracker_port, request_peer, mi_query);
        if (p) {
            printf("Peer found: %s:%d\n", p->ip, p->port);
            free(p);
        } else {
            fprintf(stderr, "Failed to get peer information\n");
            return EXIT_FAILURE;
        }
    }

    free_metainfo(mi);
    return EXIT_SUCCESS;
}
