# TODO: Add release, debug, and IWYU targets.

CC := clang
CFLAGS := -Weverything -Wno-dangling-else -Wno-vla \
	  -Wno-declaration-after-statement -g

SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
DEPENDS := $(patsubst %.c,%.d,$(SOURCES))

TARGET := calculator

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

