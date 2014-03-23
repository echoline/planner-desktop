#include "weather.h"
#define GWEATHER_I_KNOW_THIS_IS_UNSTABLE
#include <libgweather/gweather.h>

typedef struct _GtkWeatherPrivate GtkWeatherPrivate;

struct _GtkWeatherPrivate
{
	GWeatherInfo *info;
	GtkWidget *place;
	GtkWidget *sunrise;
	GtkWidget *icon;
	GtkWidget *sunset;
	GtkWidget *temp;
	GtkWidget *wind;
};

#define GTK_WEATHER_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_WEATHER, GtkWeatherPrivate))

G_DEFINE_TYPE (GtkWeather, gtk_weather, GTK_TYPE_GRID);

static gboolean
gtk_weather_update (gpointer data)
{
	GtkWeather *weather = GTK_WEATHER (data);
	GtkWeatherPrivate *priv = GTK_WEATHER_GET_PRIVATE (weather);

	gweather_info_update (priv->info);

	return TRUE;
}

static void
gtk_weather_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	GTK_WIDGET_CLASS (gtk_weather_parent_class)->size_allocate (widget,
                                allocation);
}

static void
gtk_weather_class_init (GtkWeatherClass *klass)
{
	GtkWidgetClass *class = GTK_WIDGET_CLASS (klass);

//	class->size_allocate = gtk_weather_size_allocate;

	g_type_class_add_private (class, sizeof (GtkWeatherPrivate));
}

void
gtk_weather_updated (gpointer data, gpointer user_data)
{
	GWeatherInfo *info = GWEATHER_INFO (data);
	GtkWeather *weather = GTK_WEATHER (user_data);
	GtkWeatherPrivate *priv = GTK_WEATHER_GET_PRIVATE (weather);
	GdkRGBA color;

	gdk_rgba_parse (&color, "black");

	//calc_sun (info);
	//calc_moon (info);

	const gchar *icon_name = gweather_info_get_icon_name (info);

	gtk_label_set_text (GTK_LABEL (priv->place),
				gweather_info_get_location_name (info));
	gtk_widget_override_color (priv->place, GTK_STATE_FLAG_NORMAL,
				&color);
	gtk_label_set_text (GTK_LABEL (priv->temp),
				gweather_info_get_temp (info));
	gtk_widget_override_color (priv->temp, GTK_STATE_FLAG_NORMAL,
				&color);
	gtk_label_set_text (GTK_LABEL (priv->sunrise),
				gweather_info_get_sunrise (info));
	gtk_widget_override_color (priv->sunrise, GTK_STATE_FLAG_NORMAL,
				&color);
	gtk_image_set_from_icon_name (GTK_IMAGE (priv->icon), icon_name,
					 GTK_ICON_SIZE_DIALOG);
	gtk_label_set_text (GTK_LABEL (priv->sunset),
				gweather_info_get_sunset (info));
	gtk_widget_override_color (priv->sunset, GTK_STATE_FLAG_NORMAL,
				&color);
	gtk_label_set_text (GTK_LABEL (priv->wind),
				gweather_info_get_wind (info));
	gtk_widget_override_color (priv->wind, GTK_STATE_FLAG_NORMAL,
				&color);
}

static void
gtk_weather_init (GtkWeather *weather)
{
	GWeatherLocation *loc;
	GtkWeatherPrivate *priv = GTK_WEATHER_GET_PRIVATE (weather);
	gchar *data, *fname = g_strconcat (g_get_user_config_dir (), "/planner-desktop/location.conf", NULL);
	GVariant *ser;

	if (!g_file_get_contents (fname, &data, NULL, NULL)) {
		fprintf (stderr, "%s failed to load.  please run ./location from source directory\n", fname);
		gtk_main_quit ();
	}

	ser = g_variant_parse (NULL, data, NULL, NULL, NULL);
	g_free (fname);
	g_free (data);

	priv->info = gweather_info_new (gweather_location_deserialize (gweather_location_get_world (), ser), 0);

	g_timeout_add (30 * 60 * 1000, gtk_weather_update, weather);
	g_signal_connect (priv->info, "updated",
			G_CALLBACK (gtk_weather_updated), weather);

	gtk_weather_update (weather);
}

GtkWidget*
gtk_weather_new (void)
{
	GtkWidget *ret = g_object_new (GTK_TYPE_WEATHER, NULL);
	GtkWeatherPrivate *priv = GTK_WEATHER_GET_PRIVATE (GTK_WEATHER (ret));

	priv->place = gtk_label_new ("");
	priv->sunrise = gtk_label_new ("");
	priv->icon = gtk_image_new ();
	priv->sunset = gtk_label_new ("");
	priv->temp = gtk_label_new ("");
	priv->wind = gtk_label_new ("");

	gtk_grid_attach (GTK_GRID (ret), priv->place, 0, 0, 3, 1);
	gtk_grid_attach (GTK_GRID (ret), priv->sunrise, 0, 1, 1, 1);
	gtk_grid_attach (GTK_GRID (ret), priv->icon, 1, 1, 1, 1);
	gtk_grid_attach (GTK_GRID (ret), priv->sunset, 2, 1, 1, 1);
	gtk_grid_attach (GTK_GRID (ret), priv->temp, 0, 2, 3, 1);
	gtk_grid_attach (GTK_GRID (ret), priv->wind, 0, 3, 3, 1);

	return ret;
}
