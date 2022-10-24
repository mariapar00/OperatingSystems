# paths
HEADER = header
SRC = src

server: server.o operation.o myexit.o client
	gcc -o server server.o operation.o myexit.o

client: client.o operation.o
	gcc -o client client.o operation.o

server.o: $(SRC)/server.c
	gcc -g -Wall -I$(HEADER) -c $(SRC)/server.c

client.o: $(SRC)/client.c
	gcc -g -Wall -I$(HEADER) -c $(SRC)/client.c

operation.o: $(SRC)/operation.c
	gcc -g -Wall -I$(HEADER) -c $(SRC)/operation.c

myexit.o: $(SRC)/myexit.c
	gcc -g -Wall -I$(HEADER) -c $(SRC)/myexit.c

run: server 
	./server medusa.txt 4 3

clean:
	rm -f server client server.o client.o operation.o myexit.o




