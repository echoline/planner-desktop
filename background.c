#include <gtk/gtk.h>
#include <string.h>
#include "background.h"
#include "background.xpm.h"

G_DEFINE_TYPE (GtkBackground, gtk_background, GTK_TYPE_FIXED);

typedef struct _GtkBackgroundPrivate GtkBackgroundPrivate;

struct _GtkBackgroundPrivate
{
	GdkPixbuf *pixbuf;
};

#define GTK_BACKGROUND_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_BACKGROUND, GtkBackgroundPrivate))

static gboolean
gtk_background_draw (GtkWidget *background, cairo_t *cr)
{
	gint width = gdk_screen_width ();
	gint height = gdk_screen_height ();

	GtkBackgroundPrivate *priv = GTK_BACKGROUND_GET_PRIVATE (background);

	gdk_cairo_set_source_pixbuf (cr, priv->pixbuf, 0, 0);
	cairo_rectangle (cr, 0, 0, width, height); // TODO
	cairo_fill (cr);

	GTK_WIDGET_CLASS (gtk_background_parent_class)->draw(background, cr);

	return TRUE;
}

static void
gtk_background_class_init (GtkBackgroundClass *klass)
{
	GtkWidgetClass *class = GTK_WIDGET_CLASS (klass);

	class->draw = gtk_background_draw;

	g_type_class_add_private (class, sizeof (GtkBackgroundPrivate));
}

static void
gtk_background_init (GtkBackground *background)
{
	GtkBackgroundPrivate *priv = GTK_BACKGROUND_GET_PRIVATE (background);
}

GtkWidget*
gtk_background_new (void)
{
	GtkWidget *ret = g_object_new (GTK_TYPE_BACKGROUND, NULL);
	gchar *conf;
	gchar *image_name;
	GdkPixbufLoader *loader;
	gchar *data;
	gint length;

	conf = g_strconcat (g_get_user_config_dir (), "/planner-desktop/background.conf", NULL);

	if (g_file_get_contents (conf, &image_name, NULL, NULL))
	{
		image_name[strcspn(image_name, "\r\n")] = '\0';

		if (g_file_get_contents (image_name, &data, &length, NULL))
		{
			loader = gdk_pixbuf_loader_new ();
			gdk_pixbuf_loader_write (loader, data, length, NULL);
			GTK_BACKGROUND_GET_PRIVATE(ret)->pixbuf =
				gdk_pixbuf_loader_get_pixbuf (loader);
		}

		g_free (image_name);

		goto cleanup;
	}

	GTK_BACKGROUND_GET_PRIVATE(ret)->pixbuf =
		gdk_pixbuf_new_from_xpm_data (background_xpm);

cleanup:
	g_free(conf);

	gtk_widget_set_has_window (ret, FALSE);

	return ret;
}

