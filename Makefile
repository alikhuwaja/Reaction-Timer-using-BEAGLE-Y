# ===== Build config =====
# Cross-compile by default for BeagleY-AI (ARM64). On the Beagle itself, run: make TOOLCHAIN=
TOOLCHAIN ?= aarch64-linux-gnu-
CC       := $(TOOLCHAIN)gcc
AR       := $(TOOLCHAIN)ar
STRIP    := $(TOOLCHAIN)strip

APP      := reaction_timer
BUILD    := build
NFS_DIR  ?= $(HOME)/ensc351/public/myApps

# POSIX macro exposes clock_gettime/nanosleep/usleep; keep first in CFLAGS.
# Expose legacy + POSIX APIs (usleep, clock_gettime, nanosleep)
CFLAGS   := -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700 -O2 -pipe -std=c11 -Wall -Wextra -Wno-unused-parameter
# Temporarily force-include unistd.h so usleep() is declared even if sources forgot it
CPPFLAGS := -Ihal/include -Iapp/include -include unistd.h
LDFLAGS  :=
LDLIBS   := -lm

# ===== Sources =====
# Edit APP_SRCS if your main file has a different name.
HAL_SRCS := $(wildcard hal/src/*.c)
APP_SRCS := app/src/reaction_timer.c

HAL_OBJS := $(patsubst hal/src/%.c,$(BUILD)/hal/%.o,$(HAL_SRCS))
APP_OBJS := $(patsubst app/src/%.c,$(BUILD)/app/%.o,$(APP_SRCS))

HAL_LIB  := $(BUILD)/libhal.a
APP_BIN  := $(BUILD)/$(APP)

# ===== Phony targets =====
.PHONY: all clean install run file strip

# ===== Rules =====
all: $(APP_BIN)

$(BUILD):
	mkdir -p $(BUILD)/hal $(BUILD)/app

$(HAL_LIB): $(BUILD) $(HAL_OBJS)
	$(AR) rcs $@ $(HAL_OBJS)

$(BUILD)/hal/%.o: hal/src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/app/%.o: app/src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(APP_BIN): $(BUILD) $(HAL_LIB) $(APP_OBJS)
	$(CC) $(CFLAGS) $(APP_OBJS) $(HAL_LIB) $(LDFLAGS) $(LDLIBS) -o $@
	@echo "Built $@"
	@file $@ || true

strip: $(APP_BIN)
	$(STRIP) $(APP_BIN)

install: $(APP_BIN)
	mkdir -p "$(NFS_DIR)"
	cp -v $(APP_BIN) "$(NFS_DIR)/$(APP)"

run: install
	@echo ""
	@echo "Now run on the Beagle:"
	@echo "  Beagle$ cd /mnt/remote/myApps && ./$(APP)"
	@echo ""

file: $(APP_BIN)
	@file $(APP_BIN)

clean:
	rm -rf $(BUILD)

