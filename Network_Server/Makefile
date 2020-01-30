COMPILER = g++
CPPFLAGS = -g -Wall -std=c++11

all: server
	-@echo "---All Built!---"

server: echo-server.c
	$(COMPILER) $(CPPFLAGS) echo-server.c -o server -pthread

run: server
	./server config.txt

valgrind: server
	valgrind --tool=memcheck --leak-check=yes ./server config.txt

clean:
	rm -f server *.o 
	-@echo "---All Clean!---"
