all:
	g++ -Wall -g -o run *.cpp utils/*.cpp -lglut -lGLU -lGL -lm
