#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#include "lcd.h"

static GtkBuilder *builder = NULL;

static void updateParameters(void);
static void PumpClicked(GtkButton* self, gpointer user_data);

int main (int argc, char *argv[]) {
	GtkWidget *window; 
	GtkWidget *widget;

	gtk_init (&argc, &argv);

	// Initialize gtk builder
	builder = gtk_builder_new();

	// Try loading in the glade file
	if (gtk_builder_add_from_file(builder, "screens/auto_plant_care.glade", NULL) == 0){
		printf("Unable to load the UI\n");
	}

	window = GTK_WIDGET(gtk_builder_get_object(builder, "PlantWindow"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_widget_destroyed), &window);

	// Connecting event handler for pump button
	widget = GTK_WIDGET(gtk_builder_get_object(builder, "PumpButton"));
	g_signal_connect(GTK_WIDGET(widget), "clicked", G_CALLBACK(PumpClicked), NULL);

	// Update the parameters once every 15 seconds
	g_timeout_add_seconds(1, (GSourceFunc)updateParameters, NULL);

	gtk_window_fullscreen(GTK_WINDOW(window));

	// Display our window
	gtk_widget_show_all(window);

	// Start gtk main loop
	gtk_main ();

	return 0;
}

static void updateParameters(void) {
	GtkWidget *widget;
	GtkWidget *button;
	char label[10];
     
    // Read the data from the FIFO
	LCD_Response res;
	memset(&res, 0, sizeof(LCD_Response));
	LCD_getResponse(&res);

	// Update the VMC value
	widget = GTK_WIDGET(gtk_builder_get_object(builder, "MoistureValue"));
	snprintf(label, 10, "%d%%", res.vmc);
	gtk_label_set_label(GTK_LABEL(widget), label);

	// Update the lux value
	widget = GTK_WIDGET(gtk_builder_get_object(builder, "LightValue"));
	snprintf(label, 10, "%d", res.lux);
	gtk_label_set_label(GTK_LABEL(widget), label);

	// Update the temperature value
	widget = GTK_WIDGET(gtk_builder_get_object(builder, "TemperatureValue"));
	snprintf(label, 10, "%.1f", res.temperature);
	gtk_label_set_label(GTK_LABEL(widget), label);

	// Update the pump status
	widget = GTK_WIDGET(gtk_builder_get_object(builder, "PumpStatus"));
	button = GTK_WIDGET(gtk_builder_get_object(builder, "PumpButton"));
	if (res.pumpOn) {
		gtk_label_set_label(GTK_LABEL(widget), "On");
		gtk_button_set_label(GTK_BUTTON(button), "Deactivate");
	} else {
		gtk_label_set_label(GTK_LABEL(widget), "Off");
		gtk_button_set_label(GTK_BUTTON(button), "Activate");
	}

	return;
}

static void PumpClicked(GtkButton* self, gpointer user_data) {
	LCD_Request req = {1};
	LCD_generateRequest(&req);

	return;
}