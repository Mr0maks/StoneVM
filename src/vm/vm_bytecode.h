#pragma once

#include "vm.h"

uint8_t vm_read_register( vm_struct_t *vm );
uint8_t vm_read_uint8_t(vm_struct_t *vm);
uint16_t vm_read_uint16_t(vm_struct_t *vm);
uint32_t vm_read_uint32_t(vm_struct_t *vm);
