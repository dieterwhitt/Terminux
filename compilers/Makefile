CXX = clang++
# homebrew include path (mac)
# will need to modify makefile to support linux as well.
CXXFLAGS = -std=c++17 -g -Wall -MMD -I/opt/homebrew/include  `pkg-config --cflags opencv4` 
LDFLAGS = -L/opt/homebrew/lib -lpng `pkg-config --libs opencv4`
EXEC = ./terminux
# i think theres a way to do this better but idc
OBJECTS = ./src/Frame.o ./src/util.o ./src/StaticCanvas.o ./tests/module-1-tests.o ./src/BrightnessVector.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} ${CXXFLAGS} ${LDFLAGS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
