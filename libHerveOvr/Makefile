LIBNAME = ./lib/libHerveOvr.a
CXXFLAGS = -std=c++11 -Wall -O2 -fPIC
CXXFLAGS += -I./include -I./include/OVR -I./include/OVR/Kernel -I./include/OVR/Util 
CXXFLAGS += -I../libHerve/include 
CXXFLAGS += -Wno-strict-aliasing -Wno-reorder -Wno-int-to-pointer-cast -Wno-unused-but-set-variable -Wno-unused-variable -Wno-pointer-arith -Wno-sign-compare

OBJDIR = ./obj
SRCDIR = ./src

SRC = $(shell find $(SRCDIR) -name *.cpp)
OBJ = $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRC:.cpp=.o))

.PHONY : all clean
.SECONDARY:

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	mkdir -p lib
	ar rvs $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)/OVR $(OBJDIR)/HerveOvr
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find $(OBJDIR) -name "*.o" | xargs rm 
	find . -name "*~" | xargs rm 
	rm $(LIBNAME)
