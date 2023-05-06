#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "sim_loader.h"

const char* handcar_path = "../build/libspike_api.so";

void test_csr() {
    struct SimAPI sim_api;
    open_sim_api(handcar_path, &sim_api);
    char* argv[] = {"test_register_csr.elf"};
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
    // {
    //     uint32_t x;
    //     sim_api.read_simulator_register(0, "fcsr", (uint8_t*)&x, sizeof(x));
    //     //printf("fcsr ori:%x \n", x);
    // }
    // {
    //     uint32_t x;
    //     sim_api.read_simulator_register(0, "frm", (uint8_t*)&x, sizeof(x));
    //     //printf("frm ori:%x \n", x);
    // }
    // {
    //     uint32_t x;
    //     sim_api.read_simulator_register(0, "fflags", (uint8_t*)&x, sizeof(x));
    //     printf("fflags ori:%x \n", x);
    // }
    //sim_api.write_simulator_register(0, fpr_name[0], (uint8_t*)&x, sizeof(x));
    {
        uint32_t x = 0x0;
        sim_api.write_simulator_register(0, "frm", (uint8_t*)&x, sizeof(x));
        //assert(x == 0);
    }
    // {
    //     uint32_t x;
    //     sim_api.read_simulator_register(0, "frm", (uint8_t*)&x, sizeof(x));
    //     printf("frm after:%x \n", x);
    //     //assert(x == 0);
    // }

    // {
    //     uint32_t x;
    //     sim_api.read_simulator_register(0, "fcsr", (uint8_t*)&x, sizeof(x));
    //     printf("fcsr after:%x \n", x);
    //     //assert(x == 0);
    // }
    const char* fpr_name[] = {
        "ft0", "ft1", "ft2",  "ft3",  "ft4", "ft5", "ft6",  "ft7",
        "fs0", "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4",  "fa5",
        "fa6", "fa7", "fs2",  "fs3",  "fs4", "fs5", "fs6",  "fs7",
        "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"};
        float x2;
        float x3;
        float x4;
        float x5;
        float x6;
    {
        sim_api.read_simulator_register(0, fpr_name[2], (uint8_t*)&x2, sizeof(x2));
        //printf("f2 ori:%f \n", x2);
    }
    {
        sim_api.read_simulator_register(0, fpr_name[3], (uint8_t*)&x3, sizeof(x3));
        //printf("f3 ori:%f \n", x3);
    }
    {
        sim_api.read_simulator_register(0, fpr_name[4], (uint8_t*)&x4, sizeof(x4));
        //printf("f4 ori:%f \n", x4);
    }
    // printf("result is : %f\n", x2*x3 + x4);
    sim_api.step_simulator(0, 1, 0);
    {
        sim_api.read_simulator_register(0, fpr_name[5], (uint8_t*)&x5, sizeof(x5));
        //printf("f5 after:%f \n", x5);
    }
    assert(fabs(x5 - (float)(x2*x3 + x4)) < 1e-5);
    {
        // FRM values list
        // 000: RNE Round to Nearest, ties to Even
        // 001: RTZ Round towards Zero
        // 010: RDN Round Down
        // 011: RUP Round Up
        // 100: RMM Round to Nearest, ties to Magnitude
        uint32_t x = 0x1;
        sim_api.write_simulator_register(0, "frm", (uint8_t*)&x, sizeof(x));
    }
    sim_api.step_simulator(0, 1, 0);
    {
        sim_api.read_simulator_register(0, fpr_name[6], (uint8_t*)&x6, sizeof(x6));
        //printf("f6 after:%f \n", x6);
    }
    assert(fabs(x6 - x5) > 0.0000001);

    close_sim_api(&sim_api);
}

int main(int argc, char* argv[]) {
    test_csr();
    return 0;
}
