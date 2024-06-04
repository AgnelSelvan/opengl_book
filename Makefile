CURRENT_DIR=.
INCLUDE=-I$(CURRENT_DIR)/dependencies/include
LIBRARY=-L$(CURRENT_DIR)/dependencies/library
SDL2_LIBRARY=$(CURRENT_DIR)/dependencies/library/libSDL2-2.0.0.dylib
GLAD=glad.c
ADDITIONAL=-fcolor-diagnostics -fansi-escape-codes  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation -Wno-deprecated

INCLUDES=$(INCLUDE)
LIBRARIES=$(LIBRARY)

build:
	g++ -std=c++20 *.cpp -I. -g -o bin/app.out $(GLAD) $(SDL2_LIBRARY) $(INCLUDES) $(LIBRARIES) $(ADDITIONAL)

all: build
	clear
	./bin/app.out