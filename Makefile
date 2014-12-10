CC=g++
CFLAGS=-c 
OBJ=Element.o Vertex.o Line.o Angle.o Fano.o scene_manager.o 
EXE_NAME=final

# IF
SYS:=$(shell gcc -dumpmachine)
#   LINUX
ifneq (, $(findstring linux, $(SYS)))
INCLUDES=-lglut -lGL -lGLU -lGLEW
EXE=$(EXE_NAME)
#   Min-GW
else ifneq (, $(findstring mingw, $(SYS)))
INCLUDES=-lglut -lglu32 -lglew32 -lopengl32
EXE=$(EXE_NAME).exe
#   DEFAULT
else
INCLUDES=-lglut -lGL -lGLU -lGLEW
EXE=a.out
endif
# ENDIF

makeall: $(OBJ) main.o
	$(CC) $^ $(INCLUDES) -o $(EXE)

main.o: main.cpp $(OBJ)
	$(CC) $(CFLAGS) -std=c++0x $<

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -std=c++0x $<
  
debug: $(OBJ) main.o
	$(CC) -g $^ $(INCLUDES) -o $(EXE)

run: makeall
	$(EXE)

clean:
	rm -rf *.o $(EXE)
  