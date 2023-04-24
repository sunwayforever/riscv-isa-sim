#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_read_xpr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_register.elf"};
    sim_api.initialize_simulator(1, argv);
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
    uint64_t x;
    sim_api.read_simulator_register(0, "a0", (uint8_t*)&x, 8);
    assert(x == 0x12345678);
    sim_api.read_simulator_register(0, "a1", (uint8_t*)&x, 8);
    assert(x != 0x1234);
    sim_api.step_simulator(0, 1, 0);
    sim_api.read_simulator_register(0, "a1", (uint8_t*)&x, 8);
    assert(x == 0x1234);
    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_read_xpr();
    return 0;
}
