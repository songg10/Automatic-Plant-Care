TARGET= auto-plant-care

SOURCES= source/*.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)/Automatic-Plant-Care/
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Iinclude

all: node
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/scripts
	cp -R scripts/* $(PUBDIR)/Automatic-Plant-Care/scripts
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) -lpthread -lm 

node:
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/
	mkdir -p $(PUBDIR)/Automatic-Plant-Care/web-server/
	cp -R web-server/* $(PUBDIR)/Automatic-Plant-Care/web-server/
	cd $(PUBDIR)/Automatic-Plant-Care/web-server/ && npm install

.PHONY = clean
clean:
	rm -rf $(OUTDIR)/$(TARGET)