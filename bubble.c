#include "bubble.h"

typedef struct _GtkBubblePrivate GtkBubblePrivate;

struct _GtkBubblePrivate
{
	unsigned char closed;
};

#define GTK_BUBBLE_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_BUBBLE, GtkBubblePrivate))

G_DEFINE_TYPE (GtkBubble, gtk_bubble, GTK_TYPE_BIN);


static gboolean
gtk_bubble_draw (GtkWidget *bubble, cairo_t *cr)
{
	gint width = gtk_widget_get_allocated_width (bubble);
	gint height = gtk_widget_get_allocated_height (bubble);
	gdouble cx = width / 2.0;
	gdouble cy = height / 2.0;
	gdouble radius = MIN (75.0, MIN (cx, cy)) - 5;
	//cairo_pattern_t *pat;

	cairo_arc (cr, MAX (cx, width-75.0), MAX (cy, height-75.0), radius, 0, M_PI / 2.0);
	cairo_arc (cr, MIN (cx, 75.0), MAX (cy, height-75.0), radius, M_PI / 2.0, M_PI);
	cairo_arc (cr, MIN (cx, 75.0), MIN (cy, 75.0), radius, M_PI, 3.0 * M_PI / 2.0);
	cairo_arc (cr, MAX (cx, width-75.0), MIN (cy, 75.0), radius, 3.0 * M_PI / 2.0, 2.0 * M_PI);

	cairo_close_path (cr);

	cairo_set_source_rgba (cr, 1, 1, 1, 0.5);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_stroke (cr);

	GTK_WIDGET_CLASS (gtk_bubble_parent_class)->draw (bubble, cr);
/*
	cairo_arc (cr, MAX (cx, width-75.0), MAX (cy, height-75.0), radius, 0, M_PI / 2.0);
	cairo_arc (cr, MIN (cx, 75.0), MAX (cy, height-75.0), radius, M_PI / 2.0, M_PI);
	cairo_arc (cr, MIN (cx, 75.0), MIN (cy, 75.0), radius, M_PI, 3.0 * M_PI / 2.0);
	cairo_arc (cr, MAX (cx, width-75.0), MIN (cy, 75.0), radius, 3.0 * M_PI / 2.0, 2.0 * M_PI);

	cairo_close_path (cr);

	pat = cairo_pattern_create_radial (MAX (cx, width-75.0),
					MAX (cy, height-75.0),
					MAX (cx, cy), 0,  0,
					MAX (cx, cy));

	cairo_pattern_add_color_stop_rgba (pat, 0, 0, 0, 0, 0.4);
	cairo_pattern_add_color_stop_rgba (pat, 1, 1, 1, 1, 0.2);
	cairo_set_source (cr, pat);
	cairo_fill (cr);

	cairo_pattern_destroy (pat); */

	return FALSE;
}

static void
gtk_bubble_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
	/*allocation->x -= 25;
	allocation->y -= 25;*/
		
	allocation->width += 50.0;
	allocation->height += 50.0;

	gtk_widget_set_allocation (widget, allocation);

	GTK_WIDGET_CLASS (gtk_bubble_parent_class)->size_allocate (widget,
				allocation);
}

static void
gtk_bubble_class_init (GtkBubbleClass *klass)
{
	GtkWidgetClass *class = GTK_WIDGET_CLASS (klass);

	g_type_class_add_private (class, sizeof (GtkBubblePrivate));

	class->draw = gtk_bubble_draw;
	class->size_allocate = gtk_bubble_size_allocate;
}

static void
gtk_bubble_init (GtkBubble *bubble)
{
	gtk_widget_set_has_window (GTK_WIDGET (bubble), FALSE);
}

GtkWidget*
gtk_bubble_new (void)
{
	return g_object_new (GTK_TYPE_BUBBLE, NULL);
}
