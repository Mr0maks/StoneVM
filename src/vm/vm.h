#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

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
  VM_LOOP,
  VM_JZ,
  VM_JNZ,

  VM_PUSH,
  VM_POP,
  VM_MAX_OPCODES = VM_POP + 1
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
  VM_REG_COUNT
};

enum
{
    TYPE_INTERGER,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_DATA,
};

typedef struct {
union
{
  int32_t signed_interger;
  uint32_t unsigned_interger;
  float float_32bit;
  const char *string;
  void *cdata;
};
int type;
size_t len;
} vm_register_data_t;

typedef union
{
  int i;
  float f;
} intfloat32_t;

// StoneVM Bytecode - SVMB
//#define VM_BYTECODE_MAGIC 'SVMB'
#define VM_BYTECODE_MAGIC 0x424D5653
// StoneVM Bytecode Version 0.1
#define VM_BYTECODE_VERSION 1

typedef struct
{
 uint32_t bytecode_magic;
 uint32_t bytecode_version;
 uint32_t bytecode_len;
 uint32_t bytecode_reversed;
} vm_bytecode_header_t;

#define VM_MAX_STACK_SIZE 16384 // 1024 * 4 ( uint32_t size ) = 65536 bytes

typedef struct
{
  bool halt;
  bool zflag;
  bool error;
  const char *error_string;
  uint32_t pc;
  uint8_t *bytecode;
  uint32_t bytecode_len;
  uint32_t stack[VM_MAX_STACK_SIZE];
  vm_register_data_t registers[VM_REG_COUNT];
} vm_struct_t;

#define VM_CHECK_REG_OVERFLOW(reg) ( reg >= VM_REG_COUNT )

vm_struct_t *vm_init( void );
void vm_load_bytecode( vm_struct_t *vm, uint8_t *bytecode, size_t bytecode_len );
void vm_exec( vm_struct_t *vm );
void vm_error( vm_struct_t *vm, const char *fmt, ... );
void vm_free( vm_struct_t *vm );
