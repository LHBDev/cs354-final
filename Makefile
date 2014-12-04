CC=g++ -g
CFLAGS=-c -Wall
INCLUDES=-lglut -lGL -lGLU -lGLEW
OBJ=main.o Element.o Vertex.o Line.o scene_manager.o
ELEMENTS=Element.cpp Line.cpp Vertex.cpp

makeall: $(OBJ)
	$(CC) -o final $(OBJ) $(INCLUDES)

main.o: main.cpp 
	$(CC) $(CFLAGS) main.cpp

scene: scene_manager.cpp scene_manager.h
	$(CC) $(CFLAGS) scene_manager.cpp

elements: Element.h
	$(CC) $(CFLAGS) $(ELEMENTS)

clean:
	rm -rf *o final
  