#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
//leif
#include <stdio.h>
#include <inttypes.h>
//leif

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_xpr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_register.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    int found = 0;
    /* NOTE: it is supposed to reach payload within 50 steps */
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "tp", (uint8_t*)&x, sizeof(x));
        if (x == 0xa) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    sim_api.step_simulator(0, 1, 0);
    {
        uint64_t x;
        sim_api.read_simulator_register(0, "a0", (uint8_t*)&x, sizeof(x));
        assert(x == 0x12345678);
        sim_api.read_simulator_register(0, "a1", (uint8_t*)&x, sizeof(x));
        assert(x != 0x1234);
        sim_api.step_simulator(0, 1, 0);
        sim_api.read_simulator_register(0, "a1", (uint8_t*)&x, sizeof(x));
        assert(x == 0x1234);
    }
    {
        int8_t x = -1;
        sim_api.write_simulator_register(0, "a1", (uint8_t*)&x, sizeof(x));
        sim_api.step_simulator(0, 1, 0);
        sim_api.read_simulator_register(0, "a1", (uint8_t*)&x, sizeof(x));
        assert(x == 0);
    }
    close_sim_api(&sim_api);
}

void test_fpr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_register.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    int found = 0;
    /* NOTE: it is supposed to reach payload within 50 steps */
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "tp", (uint8_t*)&x, sizeof(x));
        if (x == 0xb) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    {
        /* NOTE: read/write double */
        sim_api.step_simulator(0, 1, 0);
        double x = 0.0d;
        sim_api.read_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        assert(fabs(x - 2.0) < 1e-5);
        x = 3.0d;
        sim_api.write_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        sim_api.step_simulator(0, 1, 0);
        sim_api.read_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        assert(fabs(x - 6.0) < 1e-5);
    }
    {
        /* NOTE: read/write nan and inf */
        float x = NAN;
        sim_api.write_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        x = 0.0f;
        sim_api.read_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        assert(isnan(x));

        x = INFINITY;
        sim_api.write_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        sim_api.step_simulator(0, 1, 0);
        x = 0.0f;
        sim_api.read_simulator_register(0, "ft0", (uint8_t*)&x, sizeof(x));
        assert(x == INFINITY);
    }
    {
        /* NOTE: read/write float */
        sim_api.step_simulator(0, 1, 0);
        float x = 0.0f;
        sim_api.read_simulator_register(0, "ft1", (uint8_t*)&x, sizeof(x));
        assert(fabs(x - 1.0) < 1e-5);

        x = -1.0f;
        sim_api.write_simulator_register(0, "ft1", (uint8_t*)&x, sizeof(x));

        sim_api.step_simulator(0, 1, 0);
        sim_api.read_simulator_register(0, "ft2", (uint8_t*)&x, sizeof(x));
        assert(fabs(x) < 1e-5);
    }

    close_sim_api(&sim_api);
}

// void test_csr() {
//     struct SimAPI sim_api;
//     open_sim_api(handcar_path, &sim_api);
//     char* argv[] = {"test_register.elf"};
//     sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
//     int found = 0;
//     /* NOTE: it is supposed to reach payload within 50 steps */
//     for (int i = 0; i < 50; i++) {
//         sim_api.step_simulator(0, 1, 0);
//         uint64_t x;
//         sim_api.read_simulator_register(0, "tp", (uint8_t*)&x, 8);
//         if (x == 0xc) {
//             found = 1;
//             break;
//         }
//     }
//     assert(found == 1);
//     {
//         int32_t x = 1 << 3;
//         sim_api.write_simulator_register(0, "mip", (uint8_t*)&x, sizeof(x));
//     }
//     {
//         uint64_t x;
//         sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
//         assert(x != 0);
//     }
//     sim_api.step_simulator(0, 1, 0);
//     {
//         uint64_t x;
//         sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
//         assert(x == 0);
//     }
//     close_sim_api(&sim_api);
// }


void test_csr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_register.elf"};
    sim_api.initialize_simulator(sizeof(argv) / sizeof(char*), argv);
    int found = 0;
    /* NOTE: it is supposed to reach payload within 50 steps */
    for (int i = 0; i < 50; i++) {
        sim_api.step_simulator(0, 1, 0);
        uint64_t x;
        sim_api.read_simulator_register(0, "tp", (uint8_t*)&x, 8);
        if (x == 0xc) {
            found = 1;
            break;
        }
    }
    assert(found == 1);
    {
        int32_t x = 1 << 3;
        sim_api.write_simulator_register(0, "mip", (uint8_t*)&x, sizeof(x));
    }
    {
        uint64_t x;
        sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
        assert(x != 0);
    }
    sim_api.step_simulator(0, 1, 0);
    {
        uint64_t x,y;
        sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
        sim_api.read_simulator_register(0, "t0", (uint8_t*)&y, sizeof(y));
        printf("PC, is: %" PRIu64 "\n", x);
        printf("handle_soft_interrupt address, is: %" PRIu64 "\n", y);

        for (int i = 0; i < 100; i++) {
            sim_api.step_simulator(0, 1, 0);
            sim_api.read_simulator_register(0, "pc", (uint8_t*)&x, sizeof(x));
            if (x == y) {
                found = 1;
                printf("handle_soft_interrupt, step n is: %d\n", i);
                break;
            }
        }
        assert(x == y);
    }
    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_xpr();
    test_fpr();
    test_csr();
    return 0;
}
