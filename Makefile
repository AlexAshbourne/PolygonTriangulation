CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES= FTIDblyConnectedEdgeList.cpp FTIEdgeSearchTree.cpp FTIMonotonePolygon.cpp FTIMonotoneTriangulation.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=PolygonTriangulation

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@	