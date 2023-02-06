# Rules
all: compile linker

compile:
	g++ -I include/ -c src/rafdb.cpp -o objs/rafdb.o
	g++ -I include/ -c src/main.cpp -o objs/main.o

linker:
	g++ objs/*.o -o bin/program

clean:
	rm -rf bin/program objs/*.o
