build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer

run:
	./renderer

clean:
	rm ./renderer

punt:
	make build && make run