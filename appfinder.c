#include "appfinder.h"
#include <string.h>

typedef struct _GtkAppfinderPrivate GtkAppfinderPrivate;

struct _GtkAppfinderPrivate
{
	GtkWidget *entry;
};

#define GTK_APPFINDER_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_APPFINDER, GtkAppfinderPrivate))

G_DEFINE_TYPE (GtkAppfinder, gtk_appfinder, GTK_TYPE_GRID);

static void
gtk_appfinder_class_init (GtkAppfinderClass *klass)
{
	GtkWidgetClass *class = GTK_WIDGET_CLASS (klass);

	g_type_class_add_private (class, sizeof (GtkAppfinderPrivate));
}

static void
activate_cb (GtkEntry *entry, gpointer arg)
{
	const gchar *text = gtk_entry_get_text (entry);
	gint argc;
	gchar **argv;
	gchar *command;
	GError *error = NULL;
	GtkWidget *dialog;
	GtkWidget *contents;
	GtkWidget *label;

	if (!g_shell_parse_argv (text, &argc, &argv, &error) ||
		!g_spawn_async (NULL, argv, NULL, G_SPAWN_SEARCH_PATH |
			G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL, NULL, &error))
	{
        	command = g_strconcat ("xdg-open ", text, NULL);

		if (!g_shell_parse_argv (command, &argc, &argv, &error) ||
			!g_spawn_async (NULL, argv, NULL, G_SPAWN_SEARCH_PATH |
				G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL, NULL,
				&error))
		{
			dialog = gtk_dialog_new ();
			contents = gtk_dialog_get_content_area (
				       			GTK_DIALOG(dialog));
			label = gtk_label_new (error->message);
			gtk_container_add (GTK_CONTAINER (contents), label);
		}

		g_free (command);
	}

	gtk_entry_set_text (entry, "");
}

static void
gtk_appfinder_init (GtkAppfinder *appfinder)
{
	GtkAppfinderPrivate *priv = GTK_APPFINDER_GET_PRIVATE (appfinder);

	priv->entry = gtk_entry_new ();
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY (priv->entry),
					GTK_ENTRY_ICON_SECONDARY,
					"gtk-ok");
	gtk_widget_set_size_request (priv->entry, 640, -1);
	g_signal_connect (priv->entry, "activate", G_CALLBACK (activate_cb), NULL);

	gtk_grid_attach (GTK_GRID (appfinder), priv->entry, 0, 0, 1, 1);
}

GtkWidget*
gtk_appfinder_new (void)
{
	return g_object_new (GTK_TYPE_APPFINDER, NULL);
}
