CC = g++
CFLAGS = -Wall -std=c++11
EXEC_NAME = visibleSimToGraphAnalyzer
INCLUDES =
LIBS = -ltinyxml
OBJ_FILES = arguments.o node.o vector3D.o lattice.o visibleSimToGraphAnalyzer.o

all : $(EXEC_NAME)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)
