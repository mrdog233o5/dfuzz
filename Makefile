CC=g++
FILENAME=./main.cpp
NAME=dfuzz

run: build

clean:
	rm -rvf ./$(NAME)
	rm -rvf /Users/mrdog233o5/.local/bin/$(NAME)

build:
	$(CC) $(FILENAME) -o $(NAME) -lcurl
	cp ./$(NAME) /Users/mrdog233o5/.local/bin
