#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_read_memory() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_memory.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    uint64_t addr = 0x90000000;
    char y[13] = {0};
    sim_api.read_simulator_memory(0, &addr, 13, y);
    assert(strcmp(y, "Hello World!\n") == 0);
    close_sim_api(&sim_api);
}

void test_write_memory() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_memory.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    uint64_t addr = 0x90000000;
    char* goodbye = "goodbye";
    sim_api.write_simulator_memory(0, &addr, strlen(goodbye) + 1, goodbye);

    int found = 0;
    /* NOTE: it is supposed to reach payload within 50 steps */
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "tp", (uint8_t*)&x, 8);
        if (x == 0xa) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    sim_api.step_simulator(0, 1, 0);
    char y[13] = {0};
    sim_api.read_simulator_memory(0, &addr, strlen(goodbye) + 1, y);
    assert(strcmp(y, "Goodbye") == 0);
    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_read_memory();
    test_write_memory();
    return 0;
}
