#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

gboolean validate_ip(const char *ip);
gboolean validate_port(const char *port);
void show_message(GtkWindow *parent, const gchar *message);
void on_destroy(GtkWidget *widget, gpointer data);
void add_rows_to_tree_store(GtkTreeStore *tree_store, const char *name, char *size);
void on_button1_clicked(GtkButton *button, gpointer user_data);
char *button_clicked();
void on_button2_clicked(GtkButton *button, gpointer user_data);


#endif /* GUI_H */
