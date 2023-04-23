#include <alloca.h>

#include <algorithm>
#include <cstring>
#include <iostream>

#include "SimLoader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_step_elf() {
    printf("----------------- test step ----------------\n");
    SimDllApi sim_api;
    open_sim_dll(handcar_path, &sim_api);
    char* argv[] = {"test.elf"};
    sim_api.initialize_simulator(1, argv);
    // NOTE: step
    for (int i = 0; i < 20; i++) {
        sim_api.step_simulator(0, 1, 0);
    }
    uint8_t x[8] = {0x0};
    sim_api.read_simulator_register(0, "pc", x, 8);
    printf("pc: ");
    for (int i = 0; i < 8; i++) {
        printf("0x%x ", x[i]);
    }
    printf("\n");

    // NOTE: reg
    sim_api.read_simulator_register(0, "t0", x, 8);
    printf("t0 before: ");
    for (int i = 0; i < 8; i++) {
        printf("0x%x ", x[i]);
    }
    printf("\n");
    x[0] = 0x1;
    sim_api.write_simulator_register(0, "t0", x, 8);

    memset(x, 0, sizeof(x));
    sim_api.read_simulator_register(0, "t0", x, 8);
    printf("t0 after: ");
    for (int i = 0; i < 8; i++) {
        printf("0x%x ", x[i]);
    }
    printf("\n");

    // NOTE: memory
    memset(x, 0, sizeof(x));
    sim_api.read_simulator_register(0, "pc", x, 8);
    printf("pc at: ");
    for (int i = 0; i < 8; i++) {
        printf("0x%x ", x[i]);
    }
    printf("\n");
    uint8_t y[4] = {0x0};
    sim_api.read_simulator_memory(0, (uint64_t*)x, 4, y);
    printf("memory: ");
    for (int i = 0; i < 4; i++) {
        printf("0x%x ", y[i]);
    }
    printf("\n");
    close_sim_dll(&sim_api);
}

void test_step() {
    printf("----------------- test step ----------------\n");
    SimDllApi sim_api;
    open_sim_dll(handcar_path, &sim_api);
    char* argv[] = {"test.elf"};
    sim_api.initialize_simulator(1, argv);

    uint8_t target_addr[8] = {0x0, 0x0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0};
    sim_api.write_simulator_register(0, "ra", target_addr, 8);

    uint8_t memory_data[8] = {0x01, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2};
    sim_api.write_simulator_memory(0, (uint64_t*)target_addr, 8, memory_data);

    uint64_t instr_addr = 0x80000040;
    uint8_t instr_data[4] = {0x03, 0xB1, 0x0, 0x0};  // LD sp 0(ra)
    sim_api.write_simulator_memory(0, &instr_addr, 4, instr_data);

    uint8_t x[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    sim_api.read_simulator_register(0, "sp", x, 8);
    printf("before: sp = ");
    for (int i = 0; i < 8; i++) {
        printf("0x%x ", x[i]);
    }
    printf("\n");

    for (int i = 0; i < 6; i++) {
        sim_api.step_simulator(0, 1, 0);
    }
    sim_api.read_simulator_register(0, "sp", x, 8);
    printf("after : sp = ");
    for (int i = 0; i < 8; i++) {
        printf("0x%x ", x[i]);
    }
    printf("\n");

    close_sim_dll(&sim_api);
}

int main(int argc, char* argv[]) {
    test_step_elf();
    test_step();
    return 0;
}
