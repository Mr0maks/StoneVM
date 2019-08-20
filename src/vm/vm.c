#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vm.h"

vm_chunk_t *vm_parse(vm_struct_t *vm, int argc, char **argv )
{
  if((argc - 1) % 5)
    {
      return NULL;
    }

  size_t need_alloc = (argc - 1) / 5;

  printf("argc: %i, need alloc %lu\n", argc, need_alloc);

  vm_chunk_t *ptr = calloc(need_alloc, sizeof(vm_chunk_t));

  if(!ptr)
  return NULL;

  unsigned int chunk = 0;

  for (int i = 1; i < argc ; i++ )
  {
    int opcode, reg0, reg1, reg2, imm;

    sscanf(argv[i], "%i", &opcode);
    i++;
    sscanf(argv[i], "%x", &reg0);
    i++;
    sscanf(argv[i], "%x", &reg1);
    i++;
    sscanf(argv[i], "%x", &reg2);
    i++;
    sscanf(argv[i], "%x", &imm);

    printf("vm: %i %X %X %X %X\n", opcode, reg0, reg1, reg2, imm );

    VM_SET_OPCODE(ptr[chunk].code, opcode);
    VM_SET_REG0(ptr[chunk].code, reg0);
    VM_SET_REG1(ptr[chunk].code, reg1);
    VM_SET_REG2(ptr[chunk].code, reg2);
    VM_SET_IMM(ptr[chunk].code, imm);

    printf("%lX\n", ptr[chunk].code);

    chunk++;
  }

  vm->code = ptr;
  vm->instruction_count = chunk;
  return ptr;
}

void vm_exec_instruction(vm_struct_t *vm, vm_chunk_t *inst)
{
  int opcode = (int)VM_GET_OPCODE(inst->code);
  int reg0 =   (int)VM_GET_REG0(inst->code);
  int reg1 =   (int)VM_GET_REG1(inst->code);
  int reg2 =   (int)VM_GET_REG2(inst->code);
  int imm =    (int)VM_GET_IMM(inst->code);

  switch (opcode)
    {
      case VM_HALT:
	{
	  printf("halt\n");
	  vm->halt = 1;
	  break;
	}

      case VM_NOP:
	{
	  printf("nop\n");
	  break;
	}

      case VM_LOADI:
	{
	  printf("loadi %d, r%d\n", imm, reg0);
	  vm->registers[reg0].signed_interger = imm;
	  break;
	}
      case VM_LOADF:
	{
	  intfloat32_t intfloat = { 0 };
	  intfloat.i = imm;

	  printf("loadf %f, r%d\n", intfloat.f, reg0);
	  vm->registers[reg0].float_32bit = intfloat.f;
	  break;
	}

      case VM_MOV:
	{
	  printf("mov r%d, r%d\n", reg0, reg1 );
	  memcpy(&vm->registers[reg1], &vm->registers[reg1], sizeof(vm_register_data_t));
	  break;
	}

      case VM_CMP:
	{
	  int result = 0;
	  printf( "cmp r%d, r%d\n", reg0, reg1 );

	  if(vm->registers[reg0].unsigned_interger == vm->registers[reg1].unsigned_interger)
	    {
	      result = 1;
	    }

	  vm->zflag = result;
	  break;
	}

      case VM_SHR:
	{
	  printf("shr r%d, %d\n", reg0, imm);
	  vm->registers[reg0].unsigned_interger = (vm->registers[reg0].unsigned_interger >> imm);
	  break;
	}

      case VM_SHL:
	{
	  printf("shl r%d, %d\n", reg0, imm);
	  vm->registers[reg0].unsigned_interger = vm->registers[reg0].unsigned_interger << imm;
	  break;
	}

      case VM_XOR:
	{
	  printf("xor r%d, r%d, r%d\n", reg0, reg1, reg2);
	  vm->registers[reg2].unsigned_interger = vm->registers[reg0].unsigned_interger ^ vm->registers[reg1].unsigned_interger;
	  break;
	}

      case VM_ADD:
	{
	  printf("add r%d, r%d, r%d\n", reg0, reg1, reg2);
	  vm->registers[reg2].signed_interger = vm->registers[reg0].signed_interger + vm->registers[reg1].signed_interger;
	  break;
	}

      case VM_SUB:
	{
	  printf("sub r%d, r%d, r%d\n", reg0, reg1, reg2);
	  vm->registers[reg2].signed_interger = vm->registers[reg0].signed_interger - vm->registers[reg1].signed_interger;
	  break;
	}

      case VM_MUL:
	{
	  printf("mul r%d, r%d, r%d\n", reg0, reg1, reg2);
	  vm->registers[reg2].signed_interger = vm->registers[reg0].signed_interger * vm->registers[reg1].signed_interger;
	  break;
	}

      case VM_INC:
	{
	  printf("inc r%d\n", reg0);
	  vm->registers[reg0].signed_interger++;
	  break;
	}
      case VM_DEC:
	{
	  printf("dec r%d\n", reg0 );
	  vm->registers[reg0].signed_interger--;
	  break;
	}

      case VM_AND:
	{
	  printf("and r%d, r%d, r%d\n", reg0, reg1, reg2 );
	  vm->registers[reg2].signed_interger = vm->registers[reg0].signed_interger & vm->registers[reg1].signed_interger;
	  break;
	}

      case VM_OR:
	{
	  printf("or r%d, r%d, r%d\n", reg0, reg1, reg2);
	  vm->registers[reg2].unsigned_interger = vm->registers[reg0].unsigned_interger | vm->registers[reg1].unsigned_interger;
	  break;
	}

      case VM_CALL:
	{
	  if((unsigned int)imm > vm->instruction_count)
	    {
	      vm->halt = 1;
	      break;
	    }

	  printf("call %u\n", (unsigned int)imm);
	  vm->ip_old = vm->ip;
	  vm->ip = (unsigned int)imm;
	  break;
	}
      case VM_RET:
	{
	  printf("ret\n");
	  vm->ip = vm->ip_old;
	  break;
	}
      case VM_JUMP:
	{
	  if((unsigned int)imm > vm->instruction_count)
	    {
	      vm->halt = 1;
	      break;
	    }

	  printf("jump %u\n", (unsigned int)imm);
	  vm->ip = (unsigned int)imm;
	  break;
	}
      case VM_LOOP:
	{
	  printf("loop %i (r16 is %i)\n", imm, vm->registers[VM_R16].unsigned_interger );

	  if((unsigned int)imm > vm->instruction_count)
	    {
	      vm->halt = 1;
	      break;
	    }

	  if(vm->registers[VM_R16].unsigned_interger > 0)
	    {
	      vm->registers[VM_R16].unsigned_interger--;
	      vm->ip = (unsigned int)imm;
	    }

	  break;
	}
      case VM_JZ:
	{
	  if(vm->zflag == 0)
	    break;

	  if((unsigned int)imm > vm->instruction_count)
	    {
	      vm->halt = 1;
	      break;
	    }

	  printf("jz %i\n", imm);
	  vm->ip = (unsigned int)imm;
	  break;
	}
      case VM_JNZ:
	{
	  if(vm->zflag == 1)
	    break;

	  if((unsigned int)imm > vm->instruction_count)
	    {
	      vm->halt = 1;
	      break;
	    }

	  printf("jnz %i\n", imm);
	  vm->ip = (unsigned int)imm;
	  break;
	}

      default:
	{
	  printf("invalid opcode: 0x%X", opcode);
	  vm->halt = 1;
	  break;
	}

    }
}

vm_struct_t *vm_init(void)
{
  vm_struct_t *ptr = calloc(1, sizeof(vm_struct_t));
  return ptr;
}

void vm_clean(vm_struct_t *ptr)
{
  free(ptr);
}

void vm_execute( vm_struct_t *vm )
{
  while (vm->ip != vm->instruction_count && vm->halt == 0)
    {
      vm_chunk_t *ptr = &vm->code[vm->ip];
      vm_exec_instruction(vm, ptr);
      vm->ip++;
    }

  if(vm->halt == 1)
    printf("vm: halt call/instruction exeption\n");

  printf("REG DUMP:\n");

  for (int i = 0; i < VM_REG_COUNT ; i++) {
      printf("r%i = 0x%X\n", i, vm->registers[i].unsigned_interger );
    }
}
