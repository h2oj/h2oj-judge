CC = g++-10
CFLAGS = -Wall -lm -std=c++17

target: ../build/main.o ../build/language_cpp.o ../build/run.o
	${CC} -o ../build/hoj-judger ../build/main.o ../build/language_cpp.o ../build/run.o

../build/main.o: main.cpp run.cpp
	${CC} -c main.cpp -o ../build/main.o ${CFLAGS}

../build/run.o: run.h run.cpp
	${CC} -c run.cpp -o ../build/run.o ${CFLAGS}

../build/language_cpp.o: language/base.h language/cpp.h language/cpp.cpp
	${CC} -c language/cpp.cpp -o ../build/language_cpp.o ${CFLAGS}
