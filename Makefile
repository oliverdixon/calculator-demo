# TODO: Add release, debug, and IWYU targets.

CC := clang
CFLAGS := -Wall -Wextra -Wpedantic

SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
DEPENDS := $(patsubst %.c,%.d,$(SOURCES))

TARGET := demo

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(TARGET)

# Linking the executable from the object files
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

