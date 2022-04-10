TARGET    = auto-plant-care

SOURCES = source/*.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)/Automatic-Plant-Care
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

GUI_DEP = source/lcd.c
LIBRARY += gtk-3
LIBRARY += gdk-3
LIBRARY += atk-1.0
LIBRARY += gio-2.0
LIBRARY += pangocairo-1.0
LIBRARY += gdk_pixbuf-2.0
LIBRARY += cairo-gobject
LIBRARY += pango-1.0
LIBRARY += cairo
LIBRARY += gobject-2.0
LIBRARY += glib-2.0

LIBRARYDIR += $(HOME)/BBB/lib/arm-linux-gnueabihf
LIBRARYDIR += $(HOME)/BBB/usr/lib/arm-linux-gnueabihf
LIBRARYDIR += $(HOME)/BBB/lib
LIBRARYDIR += $(HOME)/BBB/usr/lib

XLINK_LIBDIR += $(HOME)/BBB/lib/arm-linux-gnueabihf
XLINK_LIBDIR += $(HOME)/BBB/usr/lib/arm-linux-gnueabihf

INCLUDEDIR += $(HOME)/BBB/usr/include/gtk-3.0
INCLUDEDIR += $(HOME)/BBB/usr/include/pango-1.0
INCLUDEDIR += $(HOME)/BBB/usr/include/gio-unix-2.0/
INCLUDEDIR += $(HOME)/BBB/usr/include/atk-1.0
INCLUDEDIR += $(HOME)/BBB/usr/include/cairo
INCLUDEDIR += $(HOME)/BBB/usr/include/gdk-pixbuf-2.0
INCLUDEDIR += $(HOME)/BBB/usr/include/freetype2
INCLUDEDIR += $(HOME)/BBB/usr/include/glib-2.0
INCLUDEDIR += $(HOME)/BBB/usr/lib/arm-linux-gnueabihf/glib-2.0/include
INCLUDEDIR += $(HOME)/BBB/usr/include/pixman-1
INCLUDEDIR += $(HOME)/BBB/usr/include/libpng12

INCDIR  = $(patsubst %,-I%,$(INCLUDEDIR))
LIBDIR  = $(patsubst %,-L%,$(LIBRARYDIR))
LIB    = $(patsubst %,-l%,$(LIBRARY))
XLINKDIR = $(patsubst %,-Xlinker -rpath-link=%,$(XLINK_LIBDIR))

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Iinclude

all: node
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/tmp
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/scripts
	cp -R scripts/* $(PUBDIR)/Automatic-Plant-Care/scripts
	$(CC_C) $(CFLAGS)  $(SOURCES) -o $(OUTDIR)/$(TARGET) -lpthread -lm 
	$(CC_C) $(CFLAGS) $(LIBDIR) -pthread $(INCDIR) $(XLINKDIR) $(LIB) -DGUI gui/plant_gui.c -o $(OUTDIR)/plant-gui
	cp -R screens $(PUBDIR)/Automatic-Plant-Care/

node:
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/web-server/
	cp -R web-server/* $(PUBDIR)/Automatic-Plant-Care/web-server/
	cd $(PUBDIR)/Automatic-Plant-Care/web-server/ && npm install

# gui: $(GUI_DEP)
# 	cp plant_gui gui
# 	echo 'Please only do this on the target'
# 	gcc $(CFLAGS) `pkg-config --cflags gtk+-3.0` pump_control.o utils.o gui/*.c -o plant_gui `pkg-config --libs gtk+-3.0`

# $(GUI_DEP): 
# 	gcc $(CFLAGS) -c source/pump_control.c

.PHONY = clean
clean:
	rm -f $(TARGET) *.o plant_gui
	rm -rf $(OUTDIR)