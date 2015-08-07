LIBNAME = ./lib/libHerve.a
PACKAGES = alure gl glu glew libpng libvlc 
CXXFLAGS = -std=c++11 -Wall -O2 -fPIC 
CXXFLAGS += -I./include `pkg-config --cflags $(PACKAGES)`

OBJDIR = ./obj
SRCDIR = ./src

SRC = $(shell find $(SRCDIR) -name "*.cpp")
OBJ = $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRC:.cpp=.o))

.PHONY : all clean
.SECONDARY:

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	mkdir -p lib
	ar rvs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find $(OBJDIR) -name "*.o" | xargs rm 
	find . -name "*~" | xargs rm 
	rm $(LIBNAME)
