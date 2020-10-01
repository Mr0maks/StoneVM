#pragma once

#include "vm.h"


typedef enum
{
    VM_NOP,
    VM_HALT,

    VM_LOAD_REGISTER,
    VM_LOAD_STRING,
    VM_LOAD_DATA,
    VM_SWAP,
    VM_MOV,

    //MATH
    VM_ADD,
    VM_SUB,
    VM_MUL,
    VM_DIV,
    VM_INC,
    VM_DEC,
    VM_AND,
    VM_OR,
    VM_XOR,
    VM_SHR,
    VM_SHL,

    //FLOAT MATH
    VM_FADD,
    VM_FSUB,
    VM_FMUL,
    VM_FDIV,
    VM_FSQRT,
    VM_FRSQRT,

    VM_CMP,
    VM_CALL,
    VM_RET,
    VM_JUMP,
    VM_JZ,
    VM_JNZ,

    VM_PUSH,
    VM_POP,
    VM_MAX_OPCODES
} vm_opcodes_t;

typedef void (*instruction_t)(vm_struct_t *);
