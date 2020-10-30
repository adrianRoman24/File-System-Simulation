build:filesystem.c
	gcc filesystem.c filesystem.h -o filesystem -Wall -lm

run:filesystem.c
	gcc filesystem.c filesystem.h -o filesystem -Wall -lm
	./filesystem
	rm -f filesystem
clean:
	rm -f filesystem