BUILD_DIR=./bin
APP_NAME=app.out
CURRENT_DIR=.
IMGUI_DIR=$(DIR_INCLUDE)/imgui

DIR_INCLUDE=$(CURRENT_DIR)/dependencies/includes
GLFW_LIBRARY=$(CURRENT_DIR)/dependencies/lib/GLFW/libglfw.3.3.dylib

ADDITIONAL=-fcolor-diagnostics -fansi-escape-codes -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated

INCLUDES=-I$(DIR_INCLUDE)
IMGUI_INCLUDES=-I$(IMGUI_DIR)
LIBRARIES=$(GLFW_LIBRARY)

GLAD=glad.c

SOURCES = *.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/imgui_impl_glfw.cpp $(IMGUI_DIR)/imgui_impl_opengl3.cpp

build:
	g++ -std=c++20 $(SOURCES) -I. -g -o $(BUILD_DIR)/$(APP_NAME) $(GLAD) $(INCLUDES) $(LIBRARIES) $(ADDITIONAL) $(IMGUI_INCLUDES)

all: build
	clear
	./bin/app.out