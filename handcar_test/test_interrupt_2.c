#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_interrupt_2() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_interrupt_2.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    int found = 0;
    /* NOTE: it is supposed to reach payload of "addi  tp, zero, 0xd"*/
    for (int i = 0; i < 100; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "tp", (uint8_t*)&x, 8);
        if (x == 0xd) {
            found = 1;
            break;
        }
    }

    assert(found == 1);
    {
        int32_t x = 1 << 3;
        uint64_t y = 0;
        found = 0;
        /*To triger the software interrupt*/
        sim_api.write_simulator_register(0, "mip", (uint8_t*)&x, sizeof(x));
    }
    /*Step and start handle the software interrupt*/
    sim_api.step_simulator(0, 1, 0);
    {
        uint64_t x,y;
        sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
        /*la t0,handle_soft_interrupt2; t0 == handle_soft_interrupt2*/
        sim_api.read_simulator_register(0, "t0", (uint8_t*)&y, sizeof(y));
        for (int i = 0; i < 100; i++) {
            sim_api.step_simulator(0, 1, 0);
            sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
            if (x == y) {
                break;
            }
        }
        /*expect that must pc == handle_soft_interrupt2*/
        assert(x == y);
    }
    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_interrupt_2();
    return 0;
}
