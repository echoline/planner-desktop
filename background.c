#include <gtk/gtk.h>
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
	gint width;
	gint height;

	GtkBackgroundPrivate *priv = GTK_BACKGROUND_GET_PRIVATE (background);

	width = gdk_screen_width ();
	height = gdk_screen_height ();

	gdk_cairo_set_source_pixbuf (cr, priv->pixbuf, 0, 0);
	cairo_rectangle (cr, 0, 0, width, height);
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

	GTK_BACKGROUND_GET_PRIVATE(ret)->pixbuf =
		gdk_pixbuf_new_from_xpm_data (background_xpm);

	gtk_widget_set_has_window (ret, FALSE);

	return ret;
}

