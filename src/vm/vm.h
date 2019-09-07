#pragma once

#include <inttypes.h>

typedef enum
{
  //OTHER SHIT
  VM_HALT,
  VM_NOP,

  VM_LOADI,
  VM_DUMP,
  VM_MOV,

  //MATH
  VM_SHR,
  VM_SHL,
  VM_XOR,
  VM_ADD,
  VM_SUB,
  VM_MUL,
  VM_DIV,
  VM_INC,
  VM_DEC,
  VM_AND,
  VM_OR,

  //FPU MATH
  VM_FLOAD,
  VM_FDUMP,
  VM_FADD,
  VM_FSUB,
  VM_FMUL,
  VM_FDIV,
  //
  VM_FSQRT,
  VM_FRSQRT,

  VM_CMP,
  VM_CALL,
  VM_RET,
  VM_JUMP,
  VM_LOOP,
  VM_JZ,
  VM_JNZ,

  VM_XCHG,
  VM_PUSH,
  VM_POP,
  VM_LOAD,
  VM_STORE
} vm_opcodes_t;

enum
{
  VM_R0,
  VM_R1,
  VM_R2,
  VM_R3,
  VM_R4,
  VM_R5,
  VM_R6,
  VM_R7,
  VM_R8,
  VM_R9,
  VM_R10,
  VM_R11,
  VM_R12,
  VM_R13,
  VM_R14,
  VM_R15,
  VM_R16, // pc
  VM_R17, // stack pointer
  VM_R18,
  VM_R19,
  VM_R20,
  VM_R21,
  VM_R22,
  VM_R23,
  VM_R24,
  VM_R25,
  VM_R26,
  VM_R27,
  VM_R28,
  VM_R29,
  VM_R30,
  VM_R31,
  VM_R32,
  VM_REG_COUNT = VM_R32
};

typedef union
{
  char signed_char[4];
  unsigned char unsigned_char[4];
  short signed_short[2];
  unsigned short unsigned_short[2];
  int signed_interger;
  unsigned int unsigned_interger;
  float float_32bit;
} vm_register_data_t;

typedef struct
{
  uint64_t code;
} vm_chunk_t;

typedef union
{
  int i;
  float f;
} intfloat32_t;

typedef uint32_t vm_stack_t;
typedef uint32_t vm_memory_t;

#define VM_MAGIC 0x58301337

typedef struct
{
 uint32_t magic;
 uint32_t memory_len;
 uint32_t bytecode_len;
 uint32_t reserved;
} vm_header_t;

#define VM_MAX_STACK_SIZE 1024 // 1024 * 4 ( uint32_t size ) = 4096 bytes
#define VM_MAX_MEMORY_SIZE 16384 // 16384 * 4 ( uint32_t size ) = 65536 bytes

typedef struct
{
  int halt;
  int zflag;
  unsigned int ip;
  unsigned int instruction_count;
  vm_chunk_t *code;
  vm_stack_t stack[VM_MAX_STACK_SIZE];
  vm_memory_t memory[VM_MAX_MEMORY_SIZE];
  vm_register_data_t registers[VM_REG_COUNT];
} vm_struct_t;

#define VM_MASK1(n,p)   ((~((~(uint64_t)0)<<(n)))<<(p))
#define VM_MASK0(n,p)   (~VM_MASK1(n,p))

#define VM_OPCODE_SIZE 8
#define VM_REG0_SIZE 8
#define VM_REG1_SIZE 8
#define VM_REG2_SIZE 8
#define VM_IMM_SIZE 32
#define VM_INSTRUCTION_SIZE ( VM_OPCODE_SIZE + VM_REG0_SIZE + VM_REG1_SIZE + VM_REG2_SIZE + VM_IMM_SIZE )
#define VM_INSTRUCTION_SIZE_BYTES ( VM_INSTRUCTION_SIZE / 8 )

#define VM_OPCODE_POS 0
#define VM_REG0_POS (VM_OPCODE_POS + VM_OPCODE_SIZE)
#define VM_REG1_POS (VM_REG0_POS + VM_REG0_SIZE)
#define VM_REG2_POS (VM_REG1_POS + VM_REG1_SIZE)
#define VM_IMM_POS (VM_REG2_POS + VM_REG2_SIZE)

#define getarg(i,pos,size) (uint64_t)(((i)>>pos) & VM_MASK1(size, 0))
#define setarg(i,v,pos,size)    ((i) = (uint64_t)(((i)&VM_MASK0(size,pos)) | (((uint64_t)(v))<<pos) & VM_MASK1(size, pos)))

#define VM_GET_OPCODE(code) getarg(code, VM_OPCODE_POS, VM_OPCODE_SIZE)
#define VM_GET_REG0(code) getarg(code, VM_REG0_POS, VM_REG0_SIZE)
#define VM_GET_REG1(code) getarg(code, VM_REG1_POS, VM_REG1_SIZE)
#define VM_GET_REG2(code) getarg(code, VM_REG2_POS, VM_REG2_SIZE)
#define VM_GET_IMM(code) getarg(code, VM_IMM_POS, VM_IMM_SIZE)

#define VM_SET_OPCODE(code, v) setarg(code, v, VM_OPCODE_POS, VM_OPCODE_SIZE)
#define VM_SET_REG0(code, v) setarg(code, v, VM_REG0_POS, VM_REG0_SIZE)
#define VM_SET_REG1(code, v) setarg(code, v, VM_REG1_POS, VM_REG1_SIZE)
#define VM_SET_REG2(code, v) setarg(code, v, VM_REG2_POS, VM_REG2_SIZE)
#define VM_SET_IMM(code, v) setarg(code, v, VM_IMM_POS, VM_IMM_SIZE)

#define VM_CHECK_REG_OVERFLOW(reg) ( reg < VM_R0 || reg > VM_REG_COUNT )

vm_chunk_t *vm_parse( vm_struct_t *vm, int argc, char **argv );
vm_struct_t *vm_init(void);
void vm_clean(vm_struct_t *ptr);
void vm_execute( vm_struct_t *vm );
