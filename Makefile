CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES= DblyConnectedEdgeList.cpp EdgeSearchTree.cpp MonotonePolygon.cpp MonotoneTriangulation.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=PolygonTriangulation

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@	