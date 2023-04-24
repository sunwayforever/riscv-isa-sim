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

#ifndef SIM_LOADER_H
#define SIM_LOADER_H

#include <stddef.h>
#include <stdint.h>

struct SimAPI {
    void *sim_lib;
    void (*initialize_simulator)(int argc, char **argv);
    void (*step_simulator)(int, int, int);
    int (*read_simulator_register)(int, const char *, uint8_t *, int);
    int (*write_simulator_register)(int, const char *, const uint8_t *, int);
    int (*read_simulator_memory)(int, const uint64_t *, int, uint8_t *);
    int (*write_simulator_memory)(int, const uint64_t *, int, const uint8_t *);
};

int open_sim_api(const char *sim_dll_path, struct SimAPI *api_ptrs);
void close_sim_api(struct SimAPI *api_ptrs);

#endif
