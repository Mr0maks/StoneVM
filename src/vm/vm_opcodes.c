#include "vm.h"
#include "vm_bytecode.h"
#include "vm_opcodes.h"
#include "vm_stack.h"
#include <math.h>

void vm_nop( vm_struct_t *vm )
{
    vm->pc++;
}

void vm_halt( vm_struct_t *vm )
{
    vm->halt = true;
    vm->pc++;
}

void vm_cmp( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    if(vm->registers[reg0].unsigned_interger == vm->registers[reg1].unsigned_interger)
    {
        vm->zflag = true;
    }

    vm->pc++;
}

void vm_load( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint32_t imm = vm_read_uint32_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg0].unsigned_interger = imm;
    vm->pc++;
}

void vm_load_string( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint32_t imm = vm_read_uint32_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg0].unsigned_interger = imm;
    vm->pc++;
}

void vm_load_data( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint32_t imm = vm_read_uint32_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg0].unsigned_interger = imm;
    vm->pc++;
}

void vm_swap( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1))
    {
        vm_error(vm, "register overflow");
        return;
    }

    uint32_t register0 = vm->registers[reg0].unsigned_interger, register1 = vm->registers[reg1].unsigned_interger;

    vm->registers[reg0].unsigned_interger = register1;
    vm->registers[reg1].unsigned_interger = register0;

    vm->pc++;
}

void vm_mov( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg1] = vm->registers[reg0];
    vm->pc++;
}

void vm_add( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].signed_interger = (vm->registers[reg0].signed_interger + vm->registers[reg1].signed_interger);
    vm->pc++;
}

void vm_sub( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].signed_interger = (vm->registers[reg0].signed_interger - vm->registers[reg1].signed_interger);
    vm->pc++;
}

void vm_mul( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].signed_interger = (vm->registers[reg0].signed_interger * vm->registers[reg1].signed_interger);
    vm->pc++;
}

void vm_div( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    if(vm->registers[reg0].signed_interger == 0 || vm->registers[reg1].signed_interger == 0)
    {
        vm_error(vm, "divide by zero");
        return;
    }

    vm->registers[reg2].signed_interger = (vm->registers[reg0].signed_interger / vm->registers[reg1].signed_interger);
    vm->pc++;
}

void vm_inc( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg0].unsigned_interger++;
    vm->pc++;
}

void vm_dec( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg0].unsigned_interger--;
    vm->pc++;
}

void vm_and( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].signed_interger = (vm->registers[reg0].signed_interger & vm->registers[reg1].signed_interger);
    vm->pc++;
}

void vm_or( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].unsigned_interger = (vm->registers[reg0].unsigned_interger | vm->registers[reg1].unsigned_interger);
    vm->pc++;
}

void vm_xor( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].unsigned_interger = (vm->registers[reg0].unsigned_interger ^ vm->registers[reg1].unsigned_interger);
    vm->pc++;
}

void vm_shr( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint32_t imm = vm_read_uint32_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg1].unsigned_interger = (vm->registers[reg0].unsigned_interger >> imm);
    vm->pc++;
}

void vm_shl( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint32_t imm = vm_read_uint32_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg1].unsigned_interger = (vm->registers[reg0].unsigned_interger << imm);
    vm->pc++;
}

void vm_fadd( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].float_32bit = (vm->registers[reg0].float_32bit + vm->registers[reg1].float_32bit);
    vm->pc++;
}

void vm_fsub( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].float_32bit = (vm->registers[reg0].float_32bit - vm->registers[reg1].float_32bit);
    vm->pc++;
}

void vm_fmul( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].float_32bit = (vm->registers[reg0].float_32bit * vm->registers[reg1].float_32bit);
    vm->pc++;
}

void vm_fdiv( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );
    uint8_t reg2 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1) || VM_CHECK_REG_OVERFLOW(reg2))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg2].float_32bit = (vm->registers[reg0].float_32bit / vm->registers[reg1].float_32bit);
    vm->pc++;
}

void vm_fsqrt( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg1].float_32bit = sqrtf( vm->registers[reg0].float_32bit );
    vm->pc++;
}

void vm_frsqrt( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );
    uint8_t reg1 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0) || VM_CHECK_REG_OVERFLOW(reg1))
    {
        vm_error(vm, "register overflow");
        return;
    }

    if( isnan(vm->registers[reg0].float_32bit) || isinf(vm->registers[reg0].float_32bit) )
    {
        vm_error(vm, "infinite number");
        return;
    }

    intfloat32_t intfloat = { 0 };

    intfloat.f = vm->registers[reg0].float_32bit;

    float x2, y;
    const float threehalfs = 1.5F;

    x2 = vm->registers[reg0].float_32bit * 0.5F;
    intfloat.f  = vm->registers[reg0].float_32bit;
    intfloat.i  = 0x5f3759df - ( intfloat.i >> 1 );
    y  = intfloat.f;
    y  = y * ( threehalfs - ( x2 * y * y ) );

    vm->registers[reg1].float_32bit = y;

    vm->pc++;
}

void vm_call( vm_struct_t *vm )
{
    uint32_t imm = vm_read_uint32_t( vm );

    if( imm >= vm->bytecode_len )
    {
        vm_error(vm, "imm overflow\nMax:%u\nIMM:%u\n", vm->bytecode_len, imm);
        return;
    }

    vm_stack_push( vm, vm->pc );

    vm->pc = imm;
}

void vm_ret( vm_struct_t *vm )
{
    uint32_t pc = vm_stack_pop( vm );

    if( pc >= vm->bytecode_len )
    {
        vm_error(vm, "pc overflow\nMax:%u\nPC:%u\n", vm->bytecode_len, pc);
        return;
    }

    vm->pc = pc;
}

void vm_jump( vm_struct_t *vm )
{
    uint32_t imm = vm_read_uint32_t( vm );

    if( imm >= vm->bytecode_len )
    {
        vm_error(vm, "imm overflow\nMax:%u\nIMM:%u\n", vm->bytecode_len, imm);
        return;
    }

    vm->pc = imm;
}

void vm_loop( vm_struct_t *vm )
{
    uint32_t imm = vm_read_uint32_t( vm );

    if( imm >= vm->bytecode_len )
    {
        vm_error(vm, "imm overflow\nMax:%u\nIMM:%u\n", vm->bytecode_len, imm);
        return;
    }

    if( vm->registers[VM_R16].unsigned_interger == 0 )
    {
        vm->pc++;
        return;
    }

    if( vm->registers[VM_R16].unsigned_interger > 0 )
    {
        vm->registers[VM_R16].unsigned_interger--;
        vm->pc = imm;
    }
}

void vm_jz( vm_struct_t *vm )
{
    if( !vm->zflag )
        return;

    uint32_t imm = vm_read_uint32_t( vm );

    if( imm >= vm->bytecode_len )
    {
        vm_error(vm, "imm overflow\nMax:%u\nIMM:%u\n", vm->bytecode_len, imm);
        return;
    }

    vm->pc = imm;
}

void vm_jnz( vm_struct_t *vm )
{
    if( vm->zflag )
        return;

    uint32_t imm = vm_read_uint32_t( vm );

    if( imm >= vm->bytecode_len )
    {
        vm_error(vm, "imm overflow\nBytecode len:%u\nIMM:%u\n", vm->bytecode_len, imm);
        return;
    }

    vm->pc = imm;
}

void vm_push( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm_stack_push( vm, vm->registers[reg0].unsigned_interger );

    if( vm->error )
        return;

    vm->pc++;
}

void vm_pop( vm_struct_t *vm )
{
    uint8_t reg0 = vm_read_uint8_t( vm );

    if(VM_CHECK_REG_OVERFLOW(reg0))
    {
        vm_error(vm, "register overflow");
        return;
    }

    vm->registers[reg0].unsigned_interger = vm_stack_pop( vm );

    if( vm->error )
        return;

    vm->pc++;
}

const instruction_t opcodes[VM_MAX_OPCODES] ={
    vm_nop,
    vm_halt,

    vm_load,
    vm_load_string,
    vm_load_data,
    vm_swap,
    vm_mov,

    vm_add,
    vm_sub,
    vm_mul,
    vm_div,
    vm_inc,
    vm_dec,
    vm_and,
    vm_or,
    vm_xor,
    vm_shr,
    vm_shl,

    vm_fadd,
    vm_fsub,
    vm_fmul,
    vm_fdiv,
    vm_fsqrt,
    vm_frsqrt,

    vm_cmp,
    vm_call,
    vm_ret,
    vm_jump,
    vm_loop,
    vm_jz,
    vm_jnz,

    vm_push,
    vm_pop,
};
