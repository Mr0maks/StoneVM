#include "vm.h"

void vm_stack_push( vm_struct_t *vm, uint32_t data )
{
    if(vm->registers[VM_R17].unsigned_interger >= VM_MAX_STACK_SIZE)
    {
        vm_error( vm, "stack overflow" );
        return;
    }

    vm->stack[vm->registers[VM_R17].unsigned_interger++] = data;
}

uint32_t vm_stack_pop( vm_struct_t *vm )
{
    if(vm->registers[VM_R17].unsigned_interger == 0)
    {
        vm_error( vm, "stack underflow" );
        return 0;
    }

    return vm->stack[--vm->registers[VM_R17].unsigned_interger];
}
