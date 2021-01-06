# makefile for client and server programs

all: server client clientman serverman

server: server.o tands.o time.o
	g++ -Wall -std=c++14 server.o tands.o time.o -o server -O -lpthread

server.o: server.cpp header.h
	g++ -Wall -std=c++14 -c server.cpp

tands.o: tands.c header.h
	g++ -Wall -std=c++14 -c tands.c 

time.o: time.cpp header.h
	g++ -Wall -std=c++14 -c time.cpp 	

client: client.o tands.o time.o
	g++ -Wall -std=c++14 client.o tands.o time.o -o client -O

client.o: client.cpp header.h
	g++ -Wall -std=c++14 -c client.cpp 

clientman: 
	man -Tpdf ./client.man > client.pdf

serverman: 
	man -Tpdf ./server.man > server.pdf

clean:
	rm -f *.out *.o

