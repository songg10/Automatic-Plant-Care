HOST := gcc
TARGET := arm-linux-gnueabihf-gcc

CC = $(TARGET)
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread
INCLUDE = -I include
EXE = utils.c light_sensor.c pump_control.c

all: main

main:
	$(CC) $(CFLAGS) $(INCLUDE) $(EXE) -o main
	cp main $(HOME)/cmpt433/public/myApps/

.PHONY = clean
clean:
	rm -f light_sampler
	rm -f noworky