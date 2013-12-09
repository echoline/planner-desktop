#ifndef __PLANNER_H
#define __PLANNER_H

#include <gtk/gtk.h>
#include <math.h>

typedef struct _GtkPlanner		GtkPlanner;
typedef struct _GtkPlannerClass		GtkPlannerClass;

struct _GtkPlanner
{
	GtkGrid parent;

	/* private */
};

struct _GtkPlannerClass
{
	GtkGridClass parent_class;
};

#define GTK_TYPE_PLANNER		(gtk_planner_get_type ())
#define GTK_PLANNER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PLANNER, GtkPlanner))
#define GTK_PLANNER_CLASS(obj)	(G_TYPE_CHECK_CLASS_CAST ((obj), GTK_PLANNER,  GtkPlannerClass))
#define GTK_IS_PLANNER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PLANNER))
#define GTK_IS_PLANNER_CLASS(obj)	(G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_PLANNER))
#define GTK_PLANNER_GET_CLASS	(G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PLANNER, GtkPlannerClass))

GtkWidget* gtk_planner_new (void);

#endif // __PLANNER_H
