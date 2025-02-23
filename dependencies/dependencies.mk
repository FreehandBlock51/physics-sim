# A makefile for building all dependencies required for this project
# included by top-level Makefile, so all paths are relative to ..

.PHONY: build-dependencies clean-dependencies

LIB_DIR ?=./lib

DEPENDENCIES ?= glfw3

ifeq ($(OS),Windows_NT)
	STATIC_LIB_PATHS ?= $(patsubst %, $(LIB_DIR)/%.lib, $(DEPENDENCIES))
else
	STATIC_LIB_PATHS ?= $(patsubst %, $(LIB_DIR)/lib%.a, $(DEPENDENCIES))
endif

build-dependencies: $(STATIC_LIB_PATHS)
	@echo all dependencies built! :D

$(LIB_DIR)/glfw3.lib $(LIB_DIR)/libglfw3.a:
	cmake -S dependencies/glfw -B dependencies/build/glfw -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF
	$(MAKE) -C dependencies/build/glfw
	cp dependencies/build/glfw/src/libglfw3.a lib/libglfw3.a || true
	cp dependencies/build/glfw/src/glfw3.lib lib/glfw3.lib || true

clean-dependencies:
	$(RM) lib/*
	$(RM) -r dependencies/build
