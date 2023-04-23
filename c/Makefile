# OWD 2023

CC := clang

CFLAGS := -Weverything -Wno-dangling-else -Wno-vla  \
          -Wno-declaration-after-statement -MMD -MP \
          -Wno-padded -Wno-covered-switch-default 

SOURCES := $(wildcard *.c)

DEBUG_PATH    := debug/
DEBUG_OBJECTS := $(SOURCES:%.c=$(DEBUG_PATH)%.o)
DEBUG_DEPENDS := $(SOURCES:%.c=$(DEBUG_PATH)%.d)
DEBUG_TARGET  := $(DEBUG_PATH)calculator
DEBUG_CFLAGS  := -fsanitize=address -g -DDEBUG_VERBOSE

RELEASE_PATH    := release/
RELEASE_OBJECTS := $(SOURCES:%.c=$(RELEASE_PATH)%.o)
RELEASE_DEPENDS := $(SOURCES:%.c=$(RELEASE_PATH)%.d)
RELEASE_TARGET  := $(RELEASE_PATH)calculator
RELEASE_CFLAGS  := -O3

default: makedir all

.PHONY: makedir
makedir:
	@mkdir -p $(DEBUG_PATH) $(RELEASE_PATH)

.PHONY: debug
debug: $(DEBUG_TARGET)

.PHONY: debug-clean
debug-clean:
	$(RM) $(DEBUG_OBJECTS) $(DEBUG_DEPENDS) $(DEBUG_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) $^ -o $@

-include $(DEBUG_DEPENDS)

$(DEBUG_PATH)%.o: %.c Makefile
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -c $< -o $@

.PHONY: all
all: $(RELEASE_TARGET)

.PHONY: release-clean
release-clean:
	$(RM) $(RELEASE_OBJECTS) $(RELEASE_DEPENDS) $(RELEASE_TARGET)

$(RELEASE_TARGET): $(RELEASE_OBJECTS)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) $^ -o $@

-include $(RELEASE_DEPENDS)

$(RELEASE_PATH)%.o: %.c Makefile
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(DEBUG_OBJECTS) $(DEBUG_DEPENDS) $(DEBUG_TARGET) \
	      $(RELEASE_OBJECTS) $(RELEASE_DEPENDS) $(RELEASE_TARGET)

