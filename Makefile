final: final.c++
	g++ -o final final.c++ -lglut -lGL -lGLU -lGLEW

clean:
	rm final