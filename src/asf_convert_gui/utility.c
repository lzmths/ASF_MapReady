#include "asf_convert_gui.h"

void
message_box(gchar * message)
{
  GtkWidget *dialog, *label;

  dialog = gtk_dialog_new_with_buttons( "Message",
	NULL,
	GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	GTK_STOCK_OK,
	GTK_RESPONSE_NONE,
	NULL);

  label = gtk_label_new(message);

  g_signal_connect_swapped(dialog, 
			   "response", 
			   G_CALLBACK(gtk_widget_destroy),
			   dialog);

  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);

  gtk_widget_show_all(dialog);
}

gchar *
meta_file_name(gchar * data_file_name)
{
  char * p = strrchr(data_file_name, '.');
  if (!p)
  {
    /* no extension -- assume .L */
    gchar * ret = (gchar *) malloc (strlen(data_file_name) + 3);
    strcpy(ret, data_file_name);
    strcat(ret, ".L");
    return ret;
  }

  if (strcmp(p + 1, "D") == 0)
  {
    gchar * ret = strdup(data_file_name);
    ret[strlen(data_file_name) - 1] = 'L';
    return ret;
  }

  if (strcmp(p + 1, ".img") == 0)
  {
    gchar * ret = (gchar *) malloc (strlen(data_file_name) + 2);
    strcpy(ret, data_file_name);
    *(ret + (data_file_name - p + 1)) = '\0';
    strcat(ret, ".meta");    
    return ret;
  }

  return "";
}

