all : Client.o Server.o Socket.o
	g++-7 -o Client.out Client.o Socket.o
	g++-7 -o Server.out Server.o Socket.o
	rm -f *.o

Server : Server.o Socket.o
	g++-7 -o Server.out Server.o Socket.o
	rm -f *.o

Client : Client.o Socket.o
	g++-7 -o Client.out Client.o Socket.o
	rm -f *.o

Client.o : Client.cpp Socket.cpp
	g++-7 -Wall -fpermissive -c Client.cpp Socket.cpp

Server.o : Server.cpp Socket.cpp
	g++-7 -Wall -fpermissive -c Server.cpp Socket.cpp 

clean : 
	rm -f *.out *.o