default:
	rm -f ./bin/*;
	# main bin
	gcc ./src/array/lib.c ./src/matrix/lib.c ./src/main.c -o ./bin/app;
	# test bin
	gcc ./src/array/lib.c ./src/array/lib.test.c ./src/matrix/lib.c ./src/matrix/lib.test.c -lcriterion -o ./bin/test;

build-app:
	gcc ./src/lib.c ./src/main.c -o ./bin/app;
	gcc ./src/array/lib.c ./src/matrix/lib.c ./src/main.c -o ./bin/app;

build-test:
	gcc ./src/array/lib.c ./src/array/lib.test.c ./src/matrix/lib.c ./src/matrix/lib.test.c -lcriterion -o ./bin/test;

clean:
	rm -f ./bin/*;
