#include <gtk/gtk.h>
#include <stdio.h>
#include <regex.h>

#include "tcp_client.h"
#include "tcp_server.h"
#include "metainfo.h"
#include "download.h"
#include "upload.h"
#include "talk_tracker.h"
#include "utils.h"

GtkWidget *window, *btn1, *btn2, *entry_name, *entry_ip, *entry_port, *tree_view;
GtkTreeStore *tree_store;
GtkTreeViewColumn *col1, *col2;
GtkCellRenderer *renderer1, *renderer2;

gboolean validate_ip(const char *ip) {
  regex_t regex;
  int reti;

  // Regular expression for IP address validation
  reti = regcomp(&regex, "^([0-9]{1,3}\\.){3}[0-9]{1,3}$", REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    return FALSE;
  }

  // Execute regular expression
  reti = regexec(&regex, ip, 0, NULL, 0);
  regfree(&regex);

  if (!reti) {
    return TRUE;
  } else {
    return FALSE;
  }
}

gboolean validate_port(const char *port) {
  int port_num = atoi(port);
  if (port_num > 0 && port_num <= 65535) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void show_error_message(GtkWindow *parent, const gchar *message) {
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(parent,
				  GTK_DIALOG_DESTROY_WITH_PARENT,
				  GTK_MESSAGE_ERROR,
				  GTK_BUTTONS_CLOSE,
				  "%s", message);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void add_row_to_tree_store(GtkTreeStore *tree_store, const gchar *name, const gchar *data) {
  GtkTreeIter iter;

  gtk_tree_store_append(tree_store, &iter, NULL);
  gtk_tree_store_set(tree_store, &iter, 0, name, 1, data, -1);
}

char *button_clicked() {
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Open File",
				       GTK_WINDOW(window),
				       action,
				       "_Cancel",
				       GTK_RESPONSE_CANCEL,
				       "_Open",
				       GTK_RESPONSE_ACCEPT,
				       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    g_print("File selected: %s\n", filename);
    gtk_widget_destroy(dialog);
    return filename; // Return the selected file path
  } else {
    gtk_widget_destroy(dialog);
    return NULL;
  }
}

void on_button1_clicked(GtkButton *button, gpointer user_data) {
  g_print("Button 1 clicked\n");

  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Open File",
				       GTK_WINDOW(window),
				       action,
				       "_Cancel",
				       GTK_RESPONSE_CANCEL,
				       "_Open",
				       GTK_RESPONSE_ACCEPT,
				       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    g_print("File selected: %s\n", filename);
    metainfo *mi_query = malloc(sizeof(metainfo));
    read_metainfo_file(filename, mi_query);
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
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}

void on_button2_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *dialog;
  GtkWidget *grid;
  GtkWidget *label1, *label2, *label3, *button_open_file;
  gint res;

  dialog = gtk_dialog_new_with_buttons("Input and File Selection",
				       GTK_WINDOW(window),
				       GTK_DIALOG_MODAL,
				       "_Cancel",
				       GTK_RESPONSE_CANCEL,
				       "_OK",
				       GTK_RESPONSE_ACCEPT,
				       NULL);

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
  gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
  gtk_container_add(GTK_CONTAINER(content_area), grid);

  /*label1 = gtk_label_new("Name:");
  entry_name = GTK_ENTRY(gtk_entry_new());
  gtk_grid_attach(GTK_GRID(grid), label1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(entry_name), 1, 0, 1, 1);*/

  label2 = gtk_label_new("IP Address:");
  entry_ip = GTK_ENTRY(gtk_entry_new());
  gtk_grid_attach(GTK_GRID(grid), label2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(entry_ip), 1, 1, 1, 1);

  label3 = gtk_label_new("Port:");
  entry_port = GTK_ENTRY(gtk_entry_new());
  gtk_grid_attach(GTK_GRID(grid), label3, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(entry_port), 1, 2, 1, 1);

  button_open_file = gtk_button_new_with_label("Select File");
  g_signal_connect(button_open_file, "clicked", G_CALLBACK(button_clicked), NULL);
  gtk_grid_attach(GTK_GRID(grid), button_open_file, 0, 3, 2, 1);

  gtk_widget_show_all(dialog);
  res = gtk_dialog_run(GTK_DIALOG(dialog));

  if (res == GTK_RESPONSE_ACCEPT) {
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const gchar *tracker_ip = gtk_entry_get_text(GTK_ENTRY(entry_ip));
    const gchar *tracker_port = gtk_entry_get_text(GTK_ENTRY(entry_port));
    gchar *file_path = button_clicked();
    g_print("Name: %s, IP Address: %s, Port: %s, Path: %s\n", name, tracker_ip, tracker_port, file_path);

    gboolean valid_ip = validate_ip(tracker_ip);
    gboolean valid_port = validate_port(tracker_port);

    if (!valid_ip) {
      show_error_message(GTK_WINDOW(dialog), "Invalid IP address!");
      g_print("Invalid IP address: %s\n", tracker_ip);
    }
    if (!valid_port) {
      show_error_message(GTK_WINDOW(dialog), "Invalid port!");
      g_print("Invalid port: %s\n", tracker_port);
    }

    if (valid_ip && valid_port) {
      add_row_to_tree_store(tree_store, name, "123");
    }

    metainfo *mi = init_metainfo(file_path, file_path, (char*)tracker_ip, (char*)tracker_port);
    if (create_metainfo_file("files/out.metainfo", mi) != 0) {
      //free_metainfo(mi);
      return EXIT_FAILURE;
     }
    if (start_tcp_client(tracker_ip, tracker_port, announce_file, mi) == NULL) {
      fprintf(stderr, "Failed to announce file\n");
      return EXIT_FAILURE;
    }


    
   
	
    g_free(file_path);
  }

  gtk_widget_destroy(dialog);
}

void setup_ui(GtkApplication *app, gpointer user_data) {
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
  
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "File Selector and Form");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  btn1 = gtk_button_new_with_label("Open File");
  g_signal_connect(btn1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), btn1, FALSE, FALSE, 5);

  btn2 = gtk_button_new_with_label("Create new Metainfo");
  g_signal_connect(btn2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), btn2, FALSE, FALSE, 5);

  tree_store = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_store));
  gtk_box_pack_start(GTK_BOX(vbox), tree_view, TRUE, TRUE, 5);

  col1 = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col1, "Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col1);

  col2 = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col2, "Data");
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col2);

  renderer1 = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col1, renderer1, TRUE);
  gtk_tree_view_column_add_attribute(col1, renderer1, "text", 0);

  renderer2 = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col2, renderer2, TRUE);
  gtk_tree_view_column_add_attribute(col2, renderer2, "text", 1);

  gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.example.file_selector_form", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(setup_ui), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
