CC=g++
FILENAME=./main.cpp
NAME=main
MANFILE=dfuzz.1
MANSOURCE=$(wildcard *.md)
PANDOC=pandoc
PANDOCARG=-s $(MANSOURCE) -t man -o $(MANFILE)

run: build buildman setup

clean:
	rm -rvf ./$(NAME)
	rm -rvf /Users/mrdog233o5/.local/bin/$(NAME)

build:
	$(CC) $(FILENAME) -o $(NAME) -lcurl
	cp ./$(NAME) /Users/mrdog233o5/.local/bin

buildman:
	$(PANDOC) $(PANDOCARG)
	echo "build man from md" | lolcat

setup:
	./install.sh
	echo "done"
