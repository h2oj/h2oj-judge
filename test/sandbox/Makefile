CXXC = g++
CXXFLAGS = -Wall -lm -O2 -std=c++17

run: test

clean:
	rm test.o sandbox.o child test

child: child.cpp
	${CXXC} -o child child.cpp ${CXXFLAGS}

sandbox.o: ../../src/sandbox.cpp
	${CXXC} -c -o sandbox.o ../../src/sandbox.cpp ${CXXFLAGS}

test.o: test.cpp
	${CXXC} -c -o test.o test.cpp ${CXXFLAGS}

test: test.o sandbox.o
	${CXXC} -o test test.o sandbox.o ${CXXFLAGS}
