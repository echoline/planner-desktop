#include <gtk/gtk.h>
#include "clock.h"
#include "tray.h"
#include "background.h"
#include "bubble.h"
#include "appfinder.h"
#include "planner.h"

int
main (int argc, char **argv)
{
	GtkWidget *bubble;
	GtkWidget *appfinder;
	GtkWidget *clock;
	GtkWidget *tray;
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

	fixed = gtk_background_new ();
	gtk_container_add (GTK_CONTAINER (root), fixed);
	
	clock = gtk_clock_new ();
	gtk_widget_set_size_request (clock, 150, 150);
	gtk_fixed_put (GTK_FIXED (fixed), clock, width - 175, height - 175);

	bubble = gtk_bubble_new ();
	tray = gtk_tray_new ();
	gtk_widget_set_size_request (bubble, 150, 150);
	gtk_widget_set_size_request (tray, 25, -1);
	gtk_widget_set_margin_left (tray, 50);
	gtk_widget_set_margin_top (tray, 50);
	gtk_widget_set_margin_right (tray, 50);
	gtk_widget_set_margin_bottom (tray, 50);
	gtk_container_add (GTK_CONTAINER (bubble), tray);
	gtk_fixed_put (GTK_FIXED (fixed), bubble, width - 175, 25);

	bubble = gtk_bubble_new ();
	planner = gtk_planner_new ();
	gtk_widget_set_size_request (planner, 800, 350);
	gtk_widget_set_margin_left (planner, 50);
	gtk_widget_set_margin_top (planner, 50);
	gtk_container_add (GTK_CONTAINER (bubble), planner);
	gtk_fixed_put (GTK_FIXED (fixed), bubble, 25, 25);

	bubble = gtk_bubble_new ();
	appfinder = gtk_appfinder_new ();
	gtk_widget_set_size_request (appfinder, 800, -1);
	gtk_widget_set_margin_left (appfinder, 50);
	gtk_widget_set_margin_top (appfinder, 25);
	gtk_widget_set_margin_bottom (appfinder, 25);
	gtk_container_add (GTK_CONTAINER (bubble), appfinder);
	gtk_fixed_put (GTK_FIXED (fixed), bubble, 25, height - 150);


	g_signal_connect (root, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_type_hint (GTK_WINDOW (root),
				GDK_WINDOW_TYPE_HINT_DESKTOP);

	gtk_widget_set_size_request (root, width, height);

	gtk_widget_show_all (root);

	gtk_main ();
}
