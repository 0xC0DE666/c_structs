default:
	rm -f ./bin/*;
	gcc ./src/lib.c ./src/main.c -o ./bin/app;
	gcc ./src/lib.c ./src/test.c -lcriterion -o ./bin/test;

build-app:
	gcc ./src/lib.c ./src/main.c -o ./bin/app;

build-test:
	gcc ./src/lib.c ./src/test.c -lcriterion -o ./bin/test;

clean:
	rm -f ./bin/*;
