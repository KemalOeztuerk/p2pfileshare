#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sqlite3.h>

#include "../tcp_server.h"
#include "track.h"

// Function to initialize the database
sqlite3* init_database();

// Tracker handler function
void tracker_handler(int client_fd, void *args);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize the database
    sqlite3 *db = init_database();
    if (!db) {
        fprintf(stderr, "Failed to initialize the database\n");
        exit(EXIT_FAILURE);
    }

    // Start the TCP server
    if (start_tcp_server(argv[1], tracker_handler, NULL) != 0) {
        fprintf(stderr, "Failed to start the server on port %s\n", argv[1]);
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    // Close the database when the server stops
    sqlite3_close(db);
    return 0;
}
