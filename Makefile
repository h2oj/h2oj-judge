CC = g++-10
CFLAGS = -Wall -lm -std=c++17

target: build/main.o build/language_cpp.o build/run.o build/config_loader.o
	${CC} -o build/hoj-judger build/main.o build/language_cpp.o build/run.o build/config_loader.o
	cp build/hoj-judger node/bin/hoj-judger

build/main.o: src/main.cpp
	${CC} -c src/main.cpp -o build/main.o ${CFLAGS}

build/run.o: src/run.h src/run.cpp
	${CC} -c src/run.cpp -o build/run.o ${CFLAGS}

build/config_loader.o: src/config_loader.h src/config_loader.cpp
	${CC} -c src/config_loader.cpp -o build/config_loader.o ${CFLAGS}

build/language_cpp.o: src/language/base.h src/language/cpp.h src/language/cpp.cpp
	${CC} -c src/language/cpp.cpp -o build/language_cpp.o ${CFLAGS}
