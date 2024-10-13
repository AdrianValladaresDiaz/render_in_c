build:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -o renderer

# build with second level optimizations, we can go from O0 to O3
build_optimized:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -O2 -o renderer

# build with aggressive math optimizations, breaks compliance with IEEE745. Google for more info
build_aggro:
	gcc -Wall -std=c99 ./src/*.c -lSDL2 -lm -O2 -ffast-math -o renderer

run:
	./renderer

clean:
	rm ./renderer

punt:
	make build && make run