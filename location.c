#define GWEATHER_I_KNOW_THIS_IS_UNSTABLE
#include <libgweather/gweather.h>
#include <gtk/gtk.h>

void entry_activate_cb (GWeatherLocationEntry *entry, gpointer __unused)
{
	GWeatherLocation *loc = gweather_location_entry_get_location (entry);
	GVariant *ser;
	gchar *data;
	gchar *fname = g_strconcat (g_get_user_config_dir (), "/planner-desktop/location.conf", NULL);

	ser = gweather_location_serialize (loc);
	data = g_variant_print (ser, TRUE);

	g_file_set_contents (fname, data, -1, NULL);

	printf ("city: %s\n", gweather_location_get_city_name (loc));

	gtk_main_quit();
}

int main(int argc, char **argv)
{
	gtk_init (&argc, &argv);

	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	GWeatherLocationEntry *entry = GWEATHER_LOCATION_ENTRY(gweather_location_entry_new (gweather_location_get_world()));
	g_signal_connect (entry, "activate", G_CALLBACK (entry_activate_cb), NULL);

	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (entry));

	gtk_widget_show_all (window);
	gtk_main();
}
