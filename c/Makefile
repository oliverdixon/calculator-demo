CC     := clang
CFLAGS := -Weverything -Wno-dangling-else -Wno-vla     \
          -Wno-declaration-after-statement -g -MMD -MP \
          -fsanitize=address -Wno-padded               \
	  -Wno-covered-switch-default

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
	$(CC) $(CFLAGS) -c $< -o $@

