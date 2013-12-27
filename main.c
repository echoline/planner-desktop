#include <gtk/gtk.h>
#include "clock.h"
#include "tray.h"
#include "bubble.h"
#include "appfinder.h"
#include "background.h"
#include "planner.h"

static gchar *background_name = NULL;
static gboolean tray = FALSE;
static gboolean launcher = FALSE;
static gboolean noplanner = FALSE;
static gboolean noclock = FALSE;

static GOptionEntry entries[] =
{
	{ "background", 'b', 0, G_OPTION_ARG_STRING, &background_name, "Background image name", NULL },
	{ "tray", 't', 0, G_OPTION_ARG_NONE, &tray, "System tray for icons", NULL},
	{ "launcher",'l', 0, G_OPTION_ARG_NONE, &launcher, "System launcher input box", NULL},
	{ "no-planner",'p', 0, G_OPTION_ARG_NONE, &noplanner, "Do not display planner", NULL},
	{ "no-clock",'c', 0, G_OPTION_ARG_NONE, &noclock, "Do not display clock", NULL},
	{ NULL }
};

int
main (int argc, char **argv)
{
	GtkWidget *bubble;
	GtkWidget *appfinder;
	GtkWidget *clock;
	GtkWidget *systray;
	GtkWidget *planner;
	GtkWidget *fixed;
	GtkWidget *root;
	gint width;
	gint height;

	GOptionContext *context = g_option_context_new (" - display planner in root window");
	g_option_context_add_main_entries (context, entries, NULL);
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
      	g_option_context_parse (context, &argc, &argv, NULL);

	gtk_init (&argc, &argv);

	width = gdk_screen_width ();
	height = gdk_screen_height ();

	root = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_resizable (GTK_WINDOW (root), FALSE);

	fixed = gtk_background_new (background_name);
	gtk_container_add (GTK_CONTAINER (root), fixed);
	
	if (!noclock) {
		clock = gtk_clock_new ();
		gtk_widget_set_size_request (clock, 150, 150);
		gtk_fixed_put (GTK_FIXED (fixed), clock, width - 175, height - 175);
	}

	if (tray) {
		bubble = gtk_bubble_new ();
		systray = gtk_tray_new ();
		gtk_widget_set_size_request (GTK_WIDGET (systray), 64, -1);
		gtk_widget_set_margin_left (systray, 50);
		gtk_widget_set_margin_top (systray, 50);
		gtk_container_add (GTK_CONTAINER (bubble), systray);
		gtk_fixed_put (GTK_FIXED (fixed), bubble, width - 175, 25);
	}

	if (!noplanner) {
		bubble = gtk_bubble_new ();
		planner = gtk_planner_new ();
		gtk_widget_set_margin_left (planner, 50);
		gtk_widget_set_margin_top (planner, 50);
		gtk_container_add (GTK_CONTAINER (bubble), planner);
		gtk_fixed_put (GTK_FIXED (fixed), bubble, 50, 25);
	}

	if (launcher) {
		bubble = gtk_bubble_new ();
		appfinder = gtk_appfinder_new ();
		gtk_widget_set_margin_left (appfinder, 75);
		gtk_widget_set_margin_right (appfinder, 25);
		gtk_widget_set_margin_top (appfinder, 50);
		gtk_container_add (GTK_CONTAINER (bubble), appfinder);
		gtk_fixed_put (GTK_FIXED (fixed), bubble, 50, height - 175);
	}


	g_signal_connect (root, "destroy",
			G_CALLBACK (gtk_main_quit), NULL);

	gtk_window_set_type_hint (GTK_WINDOW (root),
				GDK_WINDOW_TYPE_HINT_DESKTOP);

	gtk_widget_set_size_request (root, width, height);

	gtk_widget_show_all (root);

	gtk_main ();
}
