CC      := clang
CFLAGS  := -Iinclude -Wall -Wextra -std=c11
LDFLAGS :=
LIBS    :=

MODE ?= debug
SANITIZE ?= 0

BUILD_DIR ?= build/$(MODE)
SRC := main.c $(shell [ -d src ] && find src -name '*.c')
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)
TARGET := $(BUILD_DIR)/main

ifeq ($(MODE),debug)
  CFLAGS  += -g3 -O0
  ifeq ($(SANITIZE),1)
    CFLAGS  += -fno-omit-frame-pointer -fsanitize=address,undefined
    LDFLAGS += -fsanitize=address,undefined
  endif
else ifeq ($(MODE),release)
  # -march=native, -fcf-protection=full - for other platforms need delete
  #  or use -mtune=generic
  CFLAGS  += -O3 -DNDEBUG -march=native -flto=thin \
             -fvisibility=hidden \
             -fstack-protector-strong -fstack-clash-protection \
             -fcf-protection=full \
             -fPIE \
             -D_FORTIFY_SOURCE=3
  LDFLAGS += -flto=thin -fuse-ld=lld \
             -Wl,-O1 -Wl,--as-needed \
             -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack \
             -pie
  POST_BUILD := strip --strip-all $(TARGET)
endif

.PHONY: all debug release clean

all: debug

debug:
	$(MAKE) MODE=debug _build

release:
	$(MAKE) MODE=release _build

_build: $(TARGET)
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)
	$(if $(POST_BUILD),$(POST_BUILD))

-include $(DEP)

clean:
	rm -rf build
