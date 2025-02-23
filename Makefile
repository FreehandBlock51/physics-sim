TARGET_EXEC ?= phy-sim

# 17 is 11 + fixes & deprecations; would use 23 but my extensions don't like it
C_STANDARD ?= 17

BUILD_DIR ?= ./build
SRC_DIR ?= ./src

# libraries -- we use them, but we don't make or edit them
LIB_DIR ?= ./lib

SRCS := $(wildcard $(SRC_DIR)/*.c)
SRCS += $(wildcard $(SRC_DIR)/**/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)
DEBUG_OBJS := $(OBJS:%.o=%-debug.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := ./include ./lib-include ./build-include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

SHADER_DIR := ./shaders
SHADER_INCLUDE_DIR := ./build-include/shaders

# the -MMs are for dependency generation, so header updates trigger the right rebuilds
CFLAGS_ESSENTIAL ?= --std=c$(C_STANDARD) $(INC_FLAGS) -MMD -MP
# turn on all the warnings and mark them as errors.  Don't take any chances
# (except -pedantic; don't include it because then glad doesn't compile)
CFLAGS_WARNINGS ?= -Wall -Werror -Wextra

CFLAGS := $(CFLAGS_ESSENTIAL) $(CFLAGS_WARNINGS)

CHECKTODOS_SCRIPT_ARGS ?=

# uncomment below for textmode:
# CFLAGS += -D USE_TEXT

# if for some reason we want to work with windows as well, future-proof this makefile
DEPENDENCIES := glfw3
ifeq ($(OS),Windows_NT)
	STATIC_LIB_PATHS := $(patsubst %, $(LIB_DIR)/%.lib, $(DEPENDENCIES))
else
	STATIC_LIB_PATHS := $(patsubst %, $(LIB_DIR)/lib%.a, $(DEPENDENCIES))
endif
LIBS := $(patsubst $(LIB_DIR)/%, %, $(STATIC_LIB_PATHS))
LDFLAGS += -L$(LIB_DIR) $(addprefix -l:, $(LIBS)) -lGL -lm -pie

ENV_VARS :=
# use if your computer doesn't support OpenGL 3.3
# ENV_VARS += LIBGL_ALWAYS_SOFTWARE=1

# default target (I think make sets the first target as the default)
# runs if no target is passed to make
.PHONY: default
default: build

export DEPENDENCIES
export STATIC_LIB_PATHS
export LIB_DIR
include dependencies/dependencies.mk
unexport LIB_DIR
unexport STATIC_LIB_PATHS
unexport DEPENDENCIES

.PHONY: build run build-debug debug

build: build-dependencies $(BUILD_DIR)/$(TARGET_EXEC)
	@echo build done :D

run: build
	$(ENV_VARS) $(BUILD_DIR)/$(TARGET_EXEC)

build-debug: build-dependencies $(BUILD_DIR)/$(TARGET_EXEC)-debug
	@echo debug build done :D

debug: build-debug
	$(ENV_VARS) gdb $(BUILD_DIR)/$(TARGET_EXEC)-debug

$(BUILD_DIR)/$(TARGET_EXEC): build_shaders check_todos $(OBJS) $(STATIC_LIB_PATHS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(TARGET_EXEC)-debug: build_shaders check_todos $(DEBUG_OBJS) $(STATIC_LIB_PATHS)
	$(CC) $(DEBUG_OBJS) -o $@ $(LDFLAGS)


$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.E: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -E -c $< -o $@

$(BUILD_DIR)/%.c-debug.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -D DEBUG=1 --debug -c $< -o $@


.PHONY: check_todos
check_todos:
	buildscripts/check_for_todos.sh ./src $(CHECKTODOS_SCRIPT_ARGS)
	buildscripts/check_for_todos.sh ./include $(CHECKTODOS_SCRIPT_ARGS)
.PHONY: build_shaders
$(SHADERS): build_shaders
build_shaders:
	./buildscripts/build_shaders.sh $(SHADER_DIR) $(SHADER_INCLUDE_DIR) true

$(SHADER_INCLUDE_DIR)/%.h: build_shaders

.PHONY: clean

clean: clean-dependencies
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
