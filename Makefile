CXX = g++-11
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

SOURCES = $(wildcard *.cc)
OBJECTS = ${SOURCES:.cc=.o}
DEPENDS = ${OBJECTS:.o=.d}
EXEC = chess

.PHONY : clean

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJECTS} : ${MAKEFILE_NAME}

-include ${DEPENDS}

clean :
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}