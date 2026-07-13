# =============================================================================
# NusaCore GUI Framework — Makefile
# ICDeC RISC-V Wearable Project
#
# Usage:
#   make PLATFORM=linux        Build for Linux/SDL2  (default)
#   make PLATFORM=riscv        Build for RISC-V / pulp-runtime
#   make clean
#
# Linux prerequisites:
#   sudo apt install libsdl2-dev   (Ubuntu/Debian)
#   sudo pacman -S sdl2            (CachyOS/Arch)
#
# RISC-V prerequisites:
#   . ../env.sh    (run once per terminal session)
# =============================================================================

PLATFORM ?= linux

BUILD_DIR = build

.PHONY: default_target
default_target: all

# =============================================================================
# LVGL dependency check
# LVGL is a git submodule and must be initialised before building.
# =============================================================================
LVGL_DIR  = libs/lvgl
LVGL_HEADER = $(LVGL_DIR)/lvgl.h

$(LVGL_HEADER):
	$(error LVGL not found. Run:  git submodule update --init  OR  ./setup.sh)

# =============================================================================
# Sources (shared between both targets)
# =============================================================================
APP_SRCS = \
    main.c \
    src/display_driver.c \
    src/input_driver.c \
    src/gesture.c \
    src/display_layer.c \
    $(wildcard src/ui/*.c) \
    $(wildcard libs/lvgl/src/stdlib/*.c) 

RISCV_ONLY_SRCS = \
    src/lv_mem_custom.c \
    src/lv_global_wrapper.c \
    src/lv_mem_wrapper.c \
    src/trap_debug.c

LVGL_SRCS := $(shell find $(LVGL_DIR)/src -type f -name '*.c')
ALL_SRCS := $(APP_SRCS) $(LVGL_SRCS)

COMMON_CFLAGS = \
    -I. \
    -Isrc \
    -Isrc/ui \
    -Ilibs \
    -I$(LVGL_DIR) \
    -DLV_CONF_INCLUDE_SIMPLE \
    -DLV_LVGL_H_INCLUDE_SIMPLE

# =============================================================================
# LINUX target
# =============================================================================
ifeq ($(PLATFORM), linux)

TARGET  = $(BUILD_DIR)/gui_app
CC      = gcc
CFLAGS  = -O0 -g -Wall -Wextra \
          $(COMMON_CFLAGS) \
          -DPLATFORM_LINUX \
          $(shell sdl2-config --cflags)

LDFLAGS = $(shell sdl2-config --libs) -lm -lpthread

.PHONY: all clean run

all: $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Update rule to depend on build directory and output to build/
$(TARGET): $(LVGL_HEADER) $(ALL_SRCS) | $(BUILD_DIR)
	@echo "[CC] Linking $(TARGET) (Linux/SDL2)..."
	$(CC) $(CFLAGS) $(ALL_SRCS) -o $@ $(LDFLAGS)
	@echo "[OK] ./$(TARGET)"

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

# =============================================================================
# RISC-V target (pulp-runtime)
# =============================================================================
else ifeq ($(PLATFORM), riscv)

PULP_APP        = gui_app
PULP_APP_C_SRCS = $(ALL_SRCS) $(RISCV_ONLY_SRCS)
# For max optimization use -Os -flto (debug -O0 -fno-lto)
PULP_CFLAGS     = -g -Os -flto -falign-functions=4 -falign-labels=4 -falign-loops=4 -mstrict-align \
                  $(COMMON_CFLAGS) \
		          		-DPLATFORM_RISCV \
									-DDEBUG_BUILD
 
# Verify LVGL is present before handing off to pulp.mk
ifeq ($(wildcard $(LVGL_HEADER)),)
$(error LVGL not found at $(LVGL_DIR). Run: ./setup.sh)
endif
 
# Override default compilation optimizations for ALL files, forcing byte-by-byte safe alignment
CFLAGS    += -O0 -mstrict-align -falign-functions=4
CXXFLAGS  += -O0 -mstrict-align -falign-functions=4
 
# --- Stack-usage diagnostics (defense-in-depth, not the fix for the ---
# --- lv_display_t crash -- that struct is heap-allocated via         --
# --- lv_malloc, not stack-allocated, so it was never actually the    --
# --- thing overflowing the 2KB stack. These flags just make GCC warn --
# --- at compile time if *any* function frame gets dangerously large, --
# --- which is worth knowing given how tight your stack budget is.    --
CFLAGS    += -Wstack-usage=512 -Wframe-larger-than=512
CXXFLAGS  += -Wstack-usage=512 -Wframe-larger-than=512
 
# NOTE on -Os per-file: pulp.mk's build rules are opaque from here (they
# live in $(PULPRT_HOME), which you can't edit), so a clean per-object
# CFLAGS override for just lv_init.o isn't guaranteed to work depending
# on how pulp.mk defines its compile rule. If you want -Os applied
# narrowly, try uncommenting the two lines below and confirm with
# `make PLATFORM=riscv V=1` that the flag actually lands on that one
# object's compile line. If it doesn't take effect, fall back to
# swapping the blanket -O0 above to -Os for the whole RISC-V build --
# less convenient for debugging, but guaranteed to apply everywhere.
# $(BUILD_DIR)/libs/lvgl/src/core/lv_init.o: CFLAGS += -Os
# $(BUILD_DIR)/libs/lvgl/src/core/lv_init.o: CFLAGS := $(filter-out -O0,$(CFLAGS))

# Intercept the illegal-instruction trap without touching pulp-runtime.
# __rt_handle_illegal_instr is a STRONG symbol there (confirmed via
# `nm ... | grep handle_illegal` -> type 'T', not 'W'), so simply
# defining a function with the same name in our own workspace would be
# a link-time multiple-definition error, not an override. --wrap
# reroutes every CALL to __rt_handle_illegal_instr to
# __wrap___rt_handle_illegal_instr instead -- this works on strong
# symbols and needs no pulp-runtime edits. __real___rt_handle_illegal_instr
# remains available if we want to fall through to the original behavior.
#
# NOTE: plain `LDFLAGS +=` does NOT work here -- pulp.mk's actual link
# recipe (in $(PULPRT_HOME)/pulpos/default_rules.mk) builds its command
# line from PULP_APP_C_LDFLAGS (rolled into PULP_APP_C_LDFLAGS_$(1)),
# not the generic Make LDFLAGS variable. Confirmed by grepping that file
# directly -- LDFLAGS silently went nowhere, no error, just never used.
PULP_APP_C_LDFLAGS += -Wl,--wrap=__rt_handle_illegal_instr

include $(PULPRT_HOME)/rules/pulp.mk

else
$(error Unknown PLATFORM="$(PLATFORM)". Use PLATFORM=linux or PLATFORM=riscv)
endif
