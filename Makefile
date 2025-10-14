
CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = minimat

SRCS = main.c vector.c storage.c ui.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)