# This Makefile will generate an executable file named pgm

# Adapt this path to point to the CGP library depending on the current directory
PATH_TO_EXTERNAL_LIB = third_party/

TARGET ?= pgm #name of the executable
SRC_DIRS ?= .
CXX = g++ #Or clang++

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

PATH_TO_GLM = /opt/homebrew/Cellar/glm/1.0.1/include
PATH_TO_ASSIMP = /opt/homebrew/Cellar/assimp/5.4.1/include
PATH_TO_STB = third_party/src/stb/include
PATH_TO_SOIL = /Users/universalproperty/SOIL2/src/SOIL2

INC_DIRS  := . $(PATH_TO_EXTERNAL_LIB) $(PATH_TO_GLM) $(PATH_TO_ASSIMP) $(PATH_TO_STB) $(PATH_TO_SOIL)
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) $(shell pkg-config --cflags glfw3)

CPPFLAGS += $(INC_FLAGS) -MMD -MP -DIMGUI_IMPL_OPENGL_LOADER_GLAD -g -O2 -std=c++14 -Wall -Wextra -Wfatal-errors -Wno-sign-compare -Wno-type-limits -Wno-pragmas # Adapt these flags to your needs

ASSIMP_LIB = /opt/homebrew/Cellar/assimp/5.4.1/lib

LDLIBS += $(shell pkg-config --libs glfw3) -L$(ASSIMP_LIB) -ldl -lm -lassimp# Adapt this lib depending on your system (lib glfw is usually at -lglfw)

$(TARGET): $(OBJS)
	echo $(CURDIR)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)