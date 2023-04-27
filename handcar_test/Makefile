all: payload test

PAYLOAD_SRC := $(shell find ./ -name "test_*.s" -printf '%P\n')
PAYLOAD_APP := $(patsubst %.s,%.elf,${PAYLOAD_SRC})

${PAYLOAD_APP}:%.elf:%.s
	/opt/riscv64_gnu_toolchain/bin/riscv64-linux-gnu-gcc $< -o $@ -nostdlib -static -Wl,--script=test.ld

payload:${PAYLOAD_APP}

TEST_SRC := $(shell find ./ -name "test_*.c" -printf '%P\n')
TEST_APP := $(patsubst %.c,%,${TEST_SRC})
RUN_TEST_APP := $(patsubst %, run_%,${TEST_APP})

${RUN_TEST_APP}:run_%:%.elf

${TEST_APP}:%:%.c sim_loader.c
	gcc $^ -o $@ -O0 -g3 -ldl -rdynamic

test:${TEST_APP}

run:${RUN_TEST_APP} 

${RUN_TEST_APP}:run_%:%
	./$<

clean:
	rm -f ${TEST_APP} ${PAYLOAD_APP}
