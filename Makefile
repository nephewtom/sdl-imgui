#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
#
# You will need SDL2 (http://www.libsdl.org)
#
#   apt-get install libsdl2-dev  # Linux
#   brew install sdl2            # Mac OS X
#   pacman -S mingw-w64-i686-SDL # MSYS2
#

CXX = g++
CXXFLAGS = -g -I./imgui/ `sdl2-config --cflags`
CXXFLAGS += -Wall -Wformat

SRCS=$(wildcard *.cpp)
IMGUI_SRCS=$(wildcard imgui/*.cpp)
OBJDIR=./obj

OBJS=$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
IMGUI_OBJS=$(addprefix $(OBJDIR)/, $(IMGUI_SRCS:.cpp=.o))

EXE = sdl-imgui

LIBS = -lGLEW -lGL -ldl `sdl2-config --libs` -lSDL2_image

all: $(EXE)

$(EXE): $(OBJDIR) $(IMGUI_OBJS) $(OBJS)
	@echo Building $(EXE)
	$(CXX) -o $(EXE) $(OBJS) $(IMGUI_OBJS) $(LIBS)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/imgui

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) *~ $(EXE)
