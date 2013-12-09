#include <gtk/gtk.h>
#include "clock.h"
#include "planner.h"

int
main (int argc, char **argv)
{
	GtkWidget *clock;
	GtkWidget *planner;
	GtkWidget *fixed;
	GtkWidget *root;
	gint width;
	gint height;

	gtk_init (&argc, &argv);

	width = gdk_screen_width ();
	height = gdk_screen_height ();

	root = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_resizable (GTK_WINDOW (root), FALSE);

	fixed = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (root), fixed);
	
	clock = gtk_clock_new ();
	gtk_widget_set_size_request (clock, 150, 150);
	gtk_fixed_put (GTK_FIXED (fixed), clock, width - 175, height - 175);

	planner = gtk_planner_new ();
	gtk_widget_set_size_request (planner, 800, 480);
	gtk_fixed_put (GTK_FIXED (fixed), planner, 25, 25);

	g_signal_connect (root, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_type_hint (GTK_WINDOW (root),
				GDK_WINDOW_TYPE_HINT_DESKTOP);

	gtk_widget_set_size_request (root, width, height);

	gtk_widget_show_all (root);

	gtk_main ();
}
