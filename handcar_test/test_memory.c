#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_read_memory() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_memory.elf"};
    sim_api.initialize_simulator(1, argv);
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
    sim_api.initialize_simulator(1, argv);
    uint64_t addr = 0x90000000;
    char* goodbye = "goodbye";
    sim_api.write_simulator_memory(0, &addr, strlen(goodbye) + 1, goodbye);

    char y[13] = {0};
    sim_api.read_simulator_memory(0, &addr, strlen(goodbye) + 1, y);
    assert(strcmp(y, goodbye) == 0);
    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_read_memory();
    test_write_memory();
    return 0;
}
