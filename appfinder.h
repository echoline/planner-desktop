#ifndef __APPFINDER_H
#define __APPFINDER_H

#include <gtk/gtk.h>

typedef struct _GtkAppfinder		GtkAppfinder;
typedef struct _GtkAppfinderClass		GtkAppfinderClass;

struct _GtkAppfinder
{
	GtkGrid parent;

	/* private */
};

struct _GtkAppfinderClass
{
	GtkGridClass parent_class;
};

#define GTK_TYPE_APPFINDER		(gtk_appfinder_get_type ())
#define GTK_APPFINDER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_APPFINDER, GtkAppfinder))
#define GTK_APPFINDER_CLASS(obj)	(G_TYPE_CHECK_CLASS_CAST ((obj), GTK_APPFINDER,  GtkAppfinderClass))
#define GTK_IS_APPFINDER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_APPFINDER))
#define GTK_IS_APPFINDER_CLASS(obj)	(G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_APPFINDER))
#define GTK_APPFINDER_GET_CLASS	(G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_APPFINDER, GtkAppfinderClass))

GtkWidget* gtk_appfinder_new (void);

#endif // __APPFINDER_H
