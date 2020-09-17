CC=gcc
CCFlags=-g -Wall -Wextra
LDFLAGS=-g -Wall -Wextra
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=Shooter

all: $(TARGET)

$(TARGET): $(OBJECTS)
    $(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
    $(CC) $(CCFLAGS) -c $<

%.o: %.c
    $(CC) $(CCFLAGS) -c $<

clean:
    rm -f *.o $(TARGET)

