#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_all_xpr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_all_register.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    int found = 0;
    /* NOTE: it is supposed to reach payload within 50 steps */
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "t6", (uint8_t*)&x, sizeof(x));
        if (x == 31) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    char* xpr_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                        "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                        "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                        "s8",   "s9", "s10", "s11", "t3", "t4", "t5"};
    for (int i = 0; i < 31; i++) {
        uint64_t x;
        sim_api.read_simulator_register(
            0, xpr_name[i], (uint8_t*)&x, sizeof(x));
        assert(x == i);
        x += 1;
        sim_api.write_simulator_register(
            0, xpr_name[i], (uint8_t*)&x, sizeof(x));
    }
    found = 0;
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "t6", (uint8_t*)&x, sizeof(x));
        if (x == 32) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    uint64_t x;
    sim_api.read_simulator_register(0, "zero", (uint8_t*)&x, sizeof(x));
    assert(x == 0);

    for (int i = 1; i < 31; i++) {
        uint64_t x;
        sim_api.read_simulator_register(
            0, xpr_name[i], (uint8_t*)&x, sizeof(x));
        assert(x == i + 2);
    }

    close_sim_api(&sim_api);
}

void test_all_fpr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_all_register.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    int found = 0;
    for (int i = 0; i < 200; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "t6", (uint8_t*)&x, sizeof(x));
        if (x == 0xa) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    const char* fpr_name[] = {
        "ft0", "ft1", "ft2",  "ft3",  "ft4", "ft5", "ft6",  "ft7",
        "fs0", "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4",  "fa5",
        "fa6", "fa7", "fs2",  "fs3",  "fs4", "fs5", "fs6",  "fs7",
        "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"};

    float x;
    sim_api.read_simulator_register(0, fpr_name[0], (uint8_t*)&x, sizeof(x));
    assert(fabs(x - (float)(0)) < 1e-5);
    x = 1.0;
    sim_api.write_simulator_register(0, fpr_name[0], (uint8_t*)&x, sizeof(x));
    for (int i = 1; i < 31; i++) {
        float x;
        sim_api.read_simulator_register(
            0, fpr_name[i], (uint8_t*)&x, sizeof(x));
        assert(fabs(x - (float)(i + 1)) < 1e-5);
        x += 1.0;
        sim_api.write_simulator_register(
            0, fpr_name[i], (uint8_t*)&x, sizeof(x));
    }
    found = 0;
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "t6", (uint8_t*)&x, sizeof(x));
        if (x == 0xb) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    for (int i = 1; i < 31; i++) {
        float x;
        sim_api.read_simulator_register(
            0, fpr_name[i], (uint8_t*)&x, sizeof(x));
        assert(fabs(x - (float)(i + 3)) < 1e-5);
    }
    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_all_xpr();
    test_all_fpr();
    return 0;
}
