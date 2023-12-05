origami.e: origami.c
	gcc @opcje origami.c -lm -o origami.e

test.e: test.c
	gcc @opcje test.c -o test.e

clean:
	rm -f *.e
