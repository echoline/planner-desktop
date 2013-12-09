#include "planner.h"
#include <string.h>

typedef struct _GtkPlannerPrivate GtkPlannerPrivate;

struct _GtkPlannerPrivate
{
	GtkWidget *calendar;
	GtkWidget *notes;
	gchar *path;
};

#define GTK_PLANNER_GET_PRIVATE(obj)	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_PLANNER, GtkPlannerPrivate))

G_DEFINE_TYPE (GtkPlanner, gtk_planner, GTK_TYPE_GRID);

static void
gtk_planner_class_init (GtkPlannerClass *klass)
{
	GtkWidgetClass *class = GTK_WIDGET_CLASS (klass);

	g_type_class_add_private (class, sizeof (GtkPlannerPrivate));
}

gchar*
details(GtkCalendar *calendar, guint year, guint month, guint day, gpointer user_data) {
	gchar *text;
	gchar *path = g_strdup_printf("%s/.plans/%d/%02d/%02d/index.md", g_getenv("HOME"), year, month + 1, day);

	g_file_get_contents(path, &text, NULL, NULL);
	g_free(path);

	return text;
}

static void
load(GtkPlanner *planner) {
	GtkTextView *widget;
	GtkTextBuffer *buffer;
	gchar *text;
	gsize length;

	GtkPlannerPrivate *priv = GTK_PLANNER_GET_PRIVATE (planner);

	if (priv->path == NULL) {
		return;
	}

	if (g_file_get_contents(priv->path, &text, &length, NULL) == FALSE) {
		length = 0;
		text = g_strdup("");
	}

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(priv->notes));
	gtk_text_buffer_set_text(buffer, text, length);

	g_free(text);
}

static void
on_calendar1_day_selected(GtkCalendar *widget, gpointer planner) {
	guint year, month, day;
	gchar *errstr;

	GtkPlannerPrivate *priv = GTK_PLANNER_GET_PRIVATE (planner);

	gtk_calendar_get_date(widget, &year, &month, &day);

	if (priv->path != NULL)
		g_free(priv->path);

	priv->path = g_strdup_printf("%s/.plans/%d/%02d/%02d/index.md", g_getenv("HOME"), year, month + 1, day);
	load(planner);

}

static void
save(GtkPlanner *planner) {
	GtkTextView *widget;
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	gchar *text, *errstr;

	GtkPlannerPrivate *priv = GTK_PLANNER_GET_PRIVATE (planner);

	gchar *dir = g_strdup(priv->path);
	gchar *ptr = strrchr(dir, '/');
	if (++ptr)
		ptr = '\0';

	if (g_mkdir_with_parents(dir, 0700) != 0) {
		errstr = g_strdup_printf("unable to make directory %s", dir);
		error(errstr);
		g_free(errstr);

	}
	g_free(dir);

	widget = GTK_TEXT_VIEW(priv->notes);
	buffer = gtk_text_view_get_buffer(widget);
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	text = gtk_text_buffer_get_slice(buffer, &start, &end, TRUE);

	if (g_file_set_contents(priv->path, text, -1, NULL) == FALSE) {
		errstr = g_strdup_printf("unable to save to %s", priv->path);
		error(errstr);
		g_free(errstr);
	}

	g_free(text);
}

G_MODULE_EXPORT void
on_notes_paste_clipboard(GtkTextView *widget, gpointer planner) {
	save(planner);
}

G_MODULE_EXPORT void
on_notes_key_release_event(GtkTextView *widget, gpointer planner) {
	save(planner);
}

static void
gtk_planner_init (GtkPlanner *planner)
{
	GtkTextBuffer *buffer;
	GtkPlannerPrivate *priv = GTK_PLANNER_GET_PRIVATE (planner);

	priv->calendar = gtk_calendar_new ();
	gtk_calendar_set_detail_func(GTK_CALENDAR(priv->calendar), &details, NULL, NULL);
	gtk_calendar_set_detail_width_chars(GTK_CALENDAR(priv->calendar), 3);
	gtk_calendar_set_detail_height_rows(GTK_CALENDAR(priv->calendar), 1);
	g_signal_connect(priv->calendar, "day-selected", (GCallback)on_calendar1_day_selected, planner);

	gtk_grid_attach(GTK_GRID(planner), priv->calendar, 0, 0, 1, 1);

	buffer = gtk_text_buffer_new(gtk_text_tag_table_new ());
	priv->notes = gtk_text_view_new_with_buffer(buffer);
	gtk_widget_set_size_request(priv->notes, 400, -1);
	g_signal_connect(priv->notes, "paste-clipboard", (GCallback)on_notes_paste_clipboard, planner);
	g_signal_connect(priv->notes, "key-release-event", (GCallback)on_notes_key_release_event, planner);

	gtk_grid_attach(GTK_GRID(planner), priv->notes, 1, 0, 1, 1);

	on_calendar1_day_selected(GTK_CALENDAR(priv->calendar), planner);

}

GtkWidget*
gtk_planner_new (void)
{
	return g_object_new (GTK_TYPE_PLANNER, NULL);
}
