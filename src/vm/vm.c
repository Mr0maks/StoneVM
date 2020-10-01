#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "vm.h"
#include "vm_opcodes.h"
#include "vm_bytecode.h"

vm_struct_t *vm_init( void )
{
  vm_struct_t *ptr = malloc(sizeof(vm_struct_t));
  memset(ptr, 0, sizeof (*ptr));
  return ptr;
}

void vm_load_bytecode( vm_struct_t *vm, uint8_t *bytecode, size_t bytecode_len )
{
    assert(vm);
    assert(bytecode);
    assert(bytecode_len != 0);

    vm->bytecode = bytecode;
    vm->bytecode_len = bytecode_len;
}

extern const instruction_t opcodes[VM_MAX_OPCODES];

void vm_exec( vm_struct_t *vm )
{
    assert(vm);
    assert(vm->bytecode);

    while (vm->halt == false && vm->error == false && vm->pc != vm->bytecode_len)
    {
        uint8_t opcode = vm_read_uint8_t( vm );

        if(opcode >= VM_MAX_OPCODES)
        {
            vm_error(vm, "opcode overflow");
            break;
        }

        if( opcodes[opcode] != NULL )
        {
#ifdef DEBUG
            printf("%u\n", opcode);
#endif
            opcodes[opcode]( vm );
        } else {
            vm_error(vm, "opcode unimplemented");
            break;
        }
    }

    if(vm->halt)
        printf("vm error: halt call\n");
    else if(vm->error && vm->error_string != NULL)
        printf("vm error: %s\n", vm->error_string);
}

void vm_error( vm_struct_t *vm, const char *fmt, ... )
{
    va_list va;
    char buffer[8192];
    va_start(va, fmt);
    int len = vsnprintf(buffer, sizeof (buffer), fmt, va);
    va_end(va);

    if (len < 0)
        return;

    vm->error = true;
    vm->error_string = buffer;
}

void vm_free( vm_struct_t *vm )
{
    assert(vm);
    free(vm);
}
