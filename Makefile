
OS := $(shell uname -s)

ifeq ($(OS), Linux)
  FLAGS = -I/usr/include -L/usr/lib -L/usr/lib/nvidia-375 -lglut -lGLEW -lGL -lGLU -lX11 -Wno-write-strings
endif

ifeq ($(OS), Darwin)
  FLAGS = -framework Cocoa -framework OpenGL -framework GLUT -I/opt/X11/include -L/opt/X11/lib -lGLEW -lGLUT -Wno-write-strings
endif

all:	
	g++ main.cpp -o modelviewer $(FLAGS)
