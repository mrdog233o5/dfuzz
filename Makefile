CC=g++
FILENAME=./main.cpp

run: build

clean:
	rm -rvf ./main

build:
	$(CC) $(FILENAME) -o main -lcurl
