BUILD_DIR=./bin
APP_NAME=app.out
CURRENT_DIR=.

DIR_INCLUDE=-I$(CURRENT_DIR)/dependencies/includes
GLFW_LIBRARY=$(CURRENT_DIR)/dependencies/lib/GLFW/libglfw.3.3.dylib


ADDITIONAL=-fcolor-diagnostics -fansi-escape-codes -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated

INCLUDES=$(DIR_INCLUDE)
LIBRARIES=$(GLFW_LIBRARY)

GLAD=glad.c

build:
	g++ -std=c++20 *.cpp -I. -g -o $(BUILD_DIR)/$(APP_NAME) $(GLAD) $(INCLUDES) $(LIBRARIES) $(ADDITIONAL)

all: build
	clear
	./bin/app.out