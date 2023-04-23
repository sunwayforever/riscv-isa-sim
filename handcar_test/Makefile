BIN := handcar_test
all:${BIN}
CXXFLAGS := -MMD -O0 -g3
LDLIBS := -ldl -lstdc++
LDFLAGS := -rdynamic

SRC := $(wildcard *.cc)
OBJ := $(patsubst %.cc,%.o,${SRC})
DEP := $(OBJ:.o=.d)
-include ${DEP}

${BIN}:${OBJ}

run:${BIN}
	./${BIN}

clean:
	rm -f ${BIN} ${OBJ} ${DEP}

.PHONY: run clean
