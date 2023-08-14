.PHONY: test build

test: build
	./lemon

build: lemon.c
	gcc -o lemon -DDEBUG lemon.c -lglfw -lvulkan
