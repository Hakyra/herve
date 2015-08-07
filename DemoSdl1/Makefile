# Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
# This work is free. You can redistribute it and/or modify it under the
# terms of the Do What The Fuck You Want To Public License, Version 2,
# as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

PACKAGES = gl glu glew xinerama libudev libpng x11 sdl alure libvlc
CXXFLAGS = -std=c++11 -Wall -O2 -fPIC
CXXFLAGS += -I../libHerve/include
CXXFLAGS +=  -I../libHerveOvr/include -I../libHerveOvr/include/OVR -I../libHerveOvr/include/OVR/Kernel -I../libHerveOvr/include/OVR/Util
LDFLAGS = -L../libHerve/lib -L../libHerveOvr/lib
LIBS = -lHerve -lHerveOvr -pthread
MAINSRC = ./src/DemoSdl1.cpp

CXXFLAGS += `pkg-config --cflags $(PACKAGES)`
LDFLAGS +=`pkg-config --libs-only-L --libs-only-other $(PACKAGES)`
LIBS +=`pkg-config --libs-only-l $(PACKAGES)`

BINDIR = ./bin
OBJDIR = ./obj
SRCDIR = ./src

SRC = $(filter-out $(MAINSRC), $(shell find $(SRCDIR) -name *.cpp))
OBJ = $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRC:.cpp=.o))
BIN = $(subst $(SRCDIR)/, $(BINDIR)/, $(MAINSRC:.cpp=.out))

.PHONY : all clean
.SECONDARY:

all: $(BIN)

$(BINDIR)/%.out: $(OBJ) $(OBJDIR)/%.o
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $<  -o $@

clean:
	find $(OBJDIR) -name "*.o" | xargs rm 
	find . -name "*~" | xargs rm 
	rm $(BIN)

