#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>

#include "lcd.h"

static GtkBuilder *builder = NULL;
static LCD_Response res;
static LCD_Request req;
static int req_fifo;
static int res_fifo;

static gboolean updateParameters(void);
static void PumpClicked(GtkButton* self, gpointer user_data);
static int DataRead();
static void openRequestFIFO();

int main (int argc, char *argv[]) {
	GtkWidget *window; 
	GtkWidget *widget;

	gtk_init (&argc, &argv);

	// Initialize gtk builder
	builder = gtk_builder_new();

	// Initialize data
	memset(&res, 0, sizeof(res));
	memset(&req, 0, sizeof(req));
	
	// Open the LCD response FIFO (have to open read end first)
	res_fifo = open(LCD_RESPONSE,O_RDONLY|O_NONBLOCK);
	if (res_fifo == -1) {
		printf("plant_gui: Unable to access response FIFO\n");
	}

	// Mark the LCD request FIFO as unopen so we can open it later
	req_fifo = UNOPEN;

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

	close(req_fifo);
	close(res_fifo);

	return 0;
}

static gboolean updateParameters(void) {
	GtkWidget *widget;
	GtkWidget *button;
	char label[10];
     
    // Read the data from the FIFO
	int ok = DataRead();
	if (!ok) {
		return G_SOURCE_CONTINUE;
	}

	if (req_fifo < 0) {
		openRequestFIFO();
	}

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

	return G_SOURCE_CONTINUE;
}

static void PumpClicked(GtkButton* self, gpointer user_data) {
	if(req_fifo < 0) {
		openRequestFIFO();
	}

	req.togglePump = 1;
	write(req_fifo, &req, sizeof(req));

	return;
}

static int DataRead() {
	struct pollfd fifo_poll;
	fifo_poll.fd         = res_fifo;
	fifo_poll.events     = POLLIN;

	int ok = 0;
	for(int retry = 0; retry < 3 && !ok; retry++) {
		int ready = poll(&fifo_poll,1,1000);
		if (ready>0 && (fifo_poll.revents&POLLIN)) {
			ssize_t inbytes = read(res_fifo, &res, sizeof(res));	
			if (inbytes>0) {
				ok = true;
			}
		}
	}
		
	return ok;
}

static void openRequestFIFO() {
	req_fifo = open(LCD_REQUEST,O_WRONLY|O_NONBLOCK);
	if (req_fifo < 0) {
		printf("plant_gui: Unable to access request FIFO\n");
	}

	return;
}