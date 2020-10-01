#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <err.h>

#include "vm.h"
#include "vm_opcodes.h"
#include "vm_bytecode.h"

vm_struct_t *vm_init( void )
{
  vm_struct_t *ptr = calloc(1, sizeof(vm_struct_t));
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

    switch (setjmp(vm->jump_buffer)) {
    case 0: break;
    case 1:
    {
        printf("vm error: %s\n", vm->error_string);
        return;
    }
    default:
    {
        printf("unknown vm error\n");
        return;
    }
    }

    while (vm->pc < vm->bytecode_len)
    {
        uint8_t opcode = vm_read_uint8_t( vm );

        if(opcode >= VM_MAX_OPCODES)
        {
            vm_error(vm, "opcode overflow");
        }

        if( opcodes[opcode] != NULL )
        {
            opcodes[opcode]( vm );
        } else {
            vm_error(vm, "opcode unimplemented");
            break;
        }
    }
}

void vm_error( vm_struct_t *vm, const char *fmt, ... )
{
    assert(vm);
    assert(fmt);

    vm->error = true;

    va_list va;
    va_start(va, fmt);
    int len = vsnprintf(vm->error_string, sizeof (vm->error_string), fmt, va);
    va_end(va);

    if (len < 0)
    {
        err(EXIT_FAILURE, "vsnprintf");
    }

    longjmp(vm->jump_buffer, 1);
}

void vm_free( vm_struct_t *vm )
{
    assert(vm);
    free(vm);
}
