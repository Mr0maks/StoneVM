#include "vm.h"
#include "vm_opcodes.h"
#include <string.h>

#define VM_CHECK_REG_OVERFLOW(reg) ( (reg) >= VM_REG_COUNT )

uint8_t vm_read_register( vm_struct_t *vm )
{
    uint8_t result = 0;

    if( (vm->bytecode_len - vm->pc) < sizeof ( result ))
    {
        vm_error(vm, "not enough bytecode to read %u bytes", sizeof (result));
    }

    memcpy( &result, vm->bytecode + vm->pc, sizeof( result ) );

    if(VM_CHECK_REG_OVERFLOW(result))
    {
        vm_error(vm, "register index overflow");
    }

    vm->pc += sizeof( result );
    return result;
}

uint8_t vm_read_uint8_t( vm_struct_t *vm )
{
    uint8_t result = 0;

    if( (vm->bytecode_len - vm->pc) < sizeof ( result ))
    {
        vm_error(vm, "not enough bytecode to read %u bytes", sizeof (result));
    }

    memcpy( &result, vm->bytecode + vm->pc, sizeof( result ) );
    vm->pc += sizeof( result );
    return result;
}

uint16_t vm_read_uint16_t( vm_struct_t *vm )
{
    uint16_t result = 0;

    if( (vm->bytecode_len - vm->pc) < sizeof ( result ))
    {
        vm_error(vm, "not enough bytecode to read %u bytes", sizeof (result));
    }

    memcpy( &result, vm->bytecode + vm->pc, sizeof( result ) );
    vm->pc += sizeof( result );
    return result;
}

uint32_t vm_read_uint32_t( vm_struct_t *vm )
{
    uint32_t result = 0;

    if( (vm->bytecode_len - vm->pc) < sizeof ( result ))
    {
        vm_error(vm, "not enough bytecode to read %u bytes", sizeof (result));
    }

    memcpy( &result, vm->bytecode + vm->pc, sizeof( result ) );
    vm->pc += sizeof( result );
    return result;
}
