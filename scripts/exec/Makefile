CXX = clang++
CXXFLAGS = -std=c++17 -O3
PROGRAM = exec
${PROGRAM}: exec.cpp
	${CXX} ${CXXFLAGS} -o ${PROGRAM} exec.cpp

.PHONY: clean

clean:
	rm ${PROGRAM}
