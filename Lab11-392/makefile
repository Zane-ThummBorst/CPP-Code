
#/*******************************************************************************
# * Name        : mpsumarray.c
# * Author      : Zane ThummBorst
# * Date        :
# * Description : use multiprocessing to sum an array
# * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
# ******************************************************************************/
CC      = gcc
TARGET  = mpsumarray
C_FILES = $(filter-out $(TARGET).c, $(wildcard *.c))
OBJS    = $(patsubst %.c,%.o,$(C_FILES))
CFLAGS  = -g -Wall -Werror -pedantic-errors

all: $(TARGET)
$(TARGET): $(OBJS) $(TARGET).c
	$(CC) $(OBJS) -o $(TARGET) $(TARGET).c
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
