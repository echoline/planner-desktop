#include "tray.h"

#define SYSTEM_TRAY_REQUEST_DOCK    0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2

typedef struct _GtkTrayPrivate GtkTrayPrivate;

struct _GtkTrayPrivate
{
	Atom manager_atom;
	Atom selection_atom;
	Atom opcode_atom;
	Atom orientation_atom;
	gint rows;
};

#define GTK_TRAY_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_TRAY, GtkTrayPrivate))

G_DEFINE_TYPE (GtkTray, gtk_tray, GTK_TYPE_GRID);

void
remove_empty_frames (GtkWidget *frame, gpointer arg)
{
	GtkWidget *tray = GTK_WIDGET (arg);
	GList *children = gtk_container_get_children(GTK_CONTAINER (frame));

	if (g_list_length (children) == 0) {
		gtk_container_remove (GTK_CONTAINER (tray), frame);
	}
}

static GdkFilterReturn
gtk_tray_handle_xevent (GdkXEvent *xevent, GdkEvent *event, gpointer user_data)
{
	GtkWidget *tray = GTK_WIDGET (user_data);
	GtkTrayPrivate *priv = GTK_TRAY_GET_PRIVATE (tray);
	XClientMessageEvent *xcmev;
	XDestroyWindowEvent *xdwev;
	GtkWidget *socket;
	Window window;
	GdkRGBA bgcolor = { 1, 1, 1, 1 };
	GtkWidget *frame;

	switch (((XEvent*)xevent)->type) {
	case ClientMessage:
		xcmev = (XClientMessageEvent*)xevent;

		/*printf ("%ld:%s %ld:%s\n", xcmev->message_type,
			gdk_x11_get_xatom_name(xcmev->message_type),
			xcmev->data.l[1],
			gdk_x11_get_xatom_name(xcmev->data.l[1])); */

		if (xcmev->message_type == priv->manager_atom &&
		    xcmev->data.l[1] == priv->selection_atom)
		{
			return GDK_FILTER_REMOVE;
		}
		else if (xcmev->message_type == priv->opcode_atom &&
		    xcmev->data.l[1] == SYSTEM_TRAY_REQUEST_DOCK)
		{
			window = xcmev->data.l[2];

			frame = gtk_frame_new(NULL);
			gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);
			gtk_widget_set_size_request (frame, 24, 24);


			socket = gtk_socket_new ();
			gtk_widget_set_size_request (socket, 24, 24);
			gtk_widget_override_background_color (GTK_WIDGET (
						socket), GTK_STATE_FLAG_NORMAL,
						&bgcolor);

			gtk_container_add (GTK_CONTAINER (frame), socket);

			gtk_grid_attach (GTK_GRID (tray), frame, 0, priv->rows, 1, 1);
			priv->rows++;

			gtk_socket_add_id (GTK_SOCKET (socket), window);
			gtk_widget_set_size_request (tray, -1, 24);

			gtk_widget_show_all (frame);

			return GDK_FILTER_REMOVE;
		}
		break;
	case UnmapNotify:
	case DestroyNotify:
		gtk_container_foreach (GTK_CONTAINER (tray),
				(GtkCallback)remove_empty_frames,
				tray);

		return GDK_FILTER_REMOVE;

		break;
	default:
		break;
	}

	return GDK_FILTER_CONTINUE;
}

static void
gtk_tray_realize (GtkWidget *widget)
{
	GtkTray *tray = GTK_TRAY (widget);
	GtkTrayPrivate *priv = GTK_TRAY_GET_PRIVATE (tray);
	GdkWindow *window;

	XClientMessageEvent xevent;

	Display *display = XOpenDisplay (NULL);
	int screen_number = DefaultScreen (display);
	Window root_window = RootWindow (display, screen_number);

	gchar *selection_name = g_strdup_printf ("_NET_SYSTEM_TRAY_S%d",
						screen_number);
	priv->selection_atom = XInternAtom (display, selection_name, False);
	g_free (selection_name);
	priv->manager_atom = XInternAtom (display, "MANAGER", False);
	priv->opcode_atom = XInternAtom (display, "_NET_SYSTEM_TRAY_OPCODE",
					False);
	priv->orientation_atom = XInternAtom (display,
					"_NET_SYSTEM_TRAY_ORIENTATION", False);

	if (GTK_WIDGET_CLASS (gtk_tray_parent_class)->realize)
		GTK_WIDGET_CLASS (gtk_tray_parent_class)->realize (widget);

	window = gtk_widget_get_window (widget);

	XSetSelectionOwner (display, priv->selection_atom,
			GDK_WINDOW_XID (window), CurrentTime);

	if (XGetSelectionOwner (display, priv->selection_atom) != None)
	{
		xevent.type = ClientMessage;
		xevent.window = root_window;
		xevent.message_type = priv->manager_atom;
		xevent.format = 32;
		xevent.data.l[0] = CurrentTime;
		xevent.data.l[1] = priv->selection_atom;
		xevent.data.l[2] = GDK_WINDOW_XID (window);
		xevent.data.l[3] = 0;
		xevent.data.l[4] = 0;

		XSendEvent (display, root_window, False,
			StructureNotifyMask, (XEvent*)&xevent);

		gdk_window_add_filter (window, gtk_tray_handle_xevent, tray);
	}

	gdk_window_set_events (window, GDK_ALL_EVENTS_MASK);
}

static gboolean
gtk_tray_draw (GtkWidget *widget, cairo_t *cr)
{
	GTK_WIDGET_CLASS (gtk_tray_parent_class)->draw (widget, cr);

	return TRUE;
}

static void
gtk_tray_class_init (GtkTrayClass *klass)
{
	GtkWidgetClass *class = GTK_WIDGET_CLASS (klass);

	class->realize = gtk_tray_realize;
	class->draw = gtk_tray_draw;

	g_type_class_add_private (class, sizeof (GtkTrayPrivate));
}

static void
gtk_tray_init (GtkTray *tray)
{
}

GtkWidget*
gtk_tray_new (void)
{
	GtkWidget *ret = g_object_new (GTK_TYPE_TRAY, NULL);

	GtkTrayPrivate *priv = GTK_TRAY_GET_PRIVATE (ret);
	priv->rows = 0;

	return ret;
}
