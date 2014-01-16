#include "duoyi.h"

void duoyi_quit(GtkWidget *widget,gpointer data)
{
	gtk_main_quit();
}

void duoyi_hide_to_tray(GtkWidget *widget,GdkEventWindowState *event,
		gpointer data)
{
	if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && event->new_window_state == GDK_WINDOW_STATE_ICONIFIED)
		gtk_widget_hide(widget);
}

void duoyi_hide_window(GtkWidget *widget,TrayData *data)
{
	gtk_widget_hide(data->widget);

	gtk_widget_set_sensitive(widget,FALSE);
	gtk_widget_set_sensitive(data->item,TRUE);
}
