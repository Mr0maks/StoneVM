#pragma once

#include "vm.h"

void vm_stack_push( vm_struct_t *vm, uint32_t data );
uint32_t vm_stack_pop( vm_struct_t *vm );
