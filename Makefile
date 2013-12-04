# Author : Adrien Luxey
# Date : oct. 2013

# c++ compiler
CXX	        = 	g++

# project dirs
BIN_DIR		= 	bin/
SRC_DIR		=	src/
OBJ_DIR		= 	obj/

# target name
TARGET_R	=	$(addprefix $(BIN_DIR), neurones)
TARGET_D	=	$(addprefix $(BIN_DIR), neurones_d)

# linker flags
LDFLAG		=	-lm
LDFLAGS_R	=	-lsfml-graphics -lsfml-window -lsfml-system
LDFLAGS_D	=	-lsfml-graphics-d -lsfml-window-d -lsfml-system-d

# CXX flags
CXXFLAGS	=	-Wall -std=c++11
CXXFLAGS_R	= 	-O2
CXXFLAGS_D	=	-g

# find sources and create objects' names
SOURCES 	= 	$(wildcard $(SRC_DIR)*.cpp)
PRE_OBJ 	=	$(notdir $(SOURCES))
OBJECTS		=	$(addprefix $(OBJ_DIR), $(PRE_OBJ:.cpp=.o))

# default perspective is debug (should be changed before release)
all: debug

# release configuration
release: CXXFLAGS+=$(CXXFLAGS_R)
release: LDFLAGS+=$(LDFLAGS_R)
release: TARGET=$(TARGET_R)
release: compile

# debug configuration
debug: CXXFLAGS+=$(CXXFLAGS_D)
debug: LDFLAGS+=$(LDFLAGS_D)
debug: TARGET=$(TARGET_D)
debug: compile

# linking all dependencies and binary
compile: $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# compiling dependencies
$(addprefix $(OBJ_DIR), %.o): $(addprefix $(SRC_DIR), %.cpp)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# clean objects
clean:
	rm -rf $(OBJECTS)

# clean objects and binaries
superclean : clean
	rm -rf $(TARGET_D) $(TARGET_R)

