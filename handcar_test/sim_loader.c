//
// Copyright (C) [2020] Futurewei Technologies, Inc.
//
// FORCE-RISCV is licensed under the Apache License, Version 2.0 (the
// "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE. See the
// License for the specific language governing permissions and limitations under
// the License.
//

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sim_loader.h"

int check_sim_op(const char *which_op);

int open_sim_api(const char *sim_dll_path, struct SimAPI *api_ptrs) {
    //!< load the simulator dll...
    void *my_sim_lib = (*api_ptrs).sim_lib = dlopen(sim_dll_path, RTLD_LAZY);
    char *errmsg = dlerror();
    if (errmsg != NULL) {
        fprintf(
            stderr,
            "ERROR: Problems loading simulator DLL. Error msg from dlopen: "
            "'%s'\n",
            errmsg);
        return -1;
    }

    (*api_ptrs).initialize_simulator =
        (void (*)(int, char **))dlsym(my_sim_lib, "initialize_simulator");
    if (check_sim_op("initialize_simulator")) return -1;

    (*api_ptrs).step_simulator =
        (void (*)(int, int, int))dlsym(my_sim_lib, "step_simulator");
    if (check_sim_op("step_simulator")) return -1;

    (*api_ptrs).read_simulator_register =
        (int (*)(int, const char *, uint8_t *, int))dlsym(
            my_sim_lib, "read_simulator_register");
    if (check_sim_op("read_simulator_register")) return -1;

    (*api_ptrs).write_simulator_register =
        (int (*)(int, const char *, const uint8_t *, int))dlsym(
            my_sim_lib, "write_simulator_register");
    if (check_sim_op("write_simulator_register")) return -1;

    (*api_ptrs).read_simulator_memory =
        (int (*)(int, const uint64_t *, int, uint8_t *))dlsym(
            my_sim_lib, "read_simulator_memory");
    if (check_sim_op("read_simulator_memory")) return -1;

    (*api_ptrs).write_simulator_memory =
        (int (*)(int, const uint64_t *, int, const uint8_t *))dlsym(
            my_sim_lib, "write_simulator_memory");
    if (check_sim_op("write_simulator_memory")) return -1;

    return 0;
}

void close_sim_api(struct SimAPI *api_ptrs) {
    if ((*api_ptrs).sim_lib != NULL) {
        dlclose((*api_ptrs).sim_lib);
        (*api_ptrs).sim_lib = NULL;
    }
}

int check_sim_op(const char *which_op) {
    char *errmsg = dlerror();
    if (errmsg != NULL) {
        fprintf(
            stderr,
            "ERROR: Problems loading simulator DLL '%s' symbol. Error msg from "
            "dlsym: '%s'\n",
            which_op, errmsg);
        return -1;
    }
    return 0;
}
