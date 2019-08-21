#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

int get_number(FILE* fp, size_t bits_to_read)
{
    int num;
    if (fread(&num, bits_to_read/8, 1, fp) != 1) 
    {
      if (feof(fp))
        printf("ERROR: Premature end of file.\n");
      else 
        printf("ERROR: File read error.\n");
      exit(1);
    }
    return num;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    printf("ERROR: No file provided.\n");
    exit(1);
  }

  FILE *fp;

  if ((fp=fopen(argv[1], "rb")) == NULL) {
    printf("ERROR: Cannot open file.\n");
    exit(1);
  }
  
  int inst_size = (VM_OPCODE_SIZE + VM_REG0_SIZE + VM_REG1_SIZE + VM_REG2_SIZE + VM_IMM_SIZE) / 8;

  fseek(fp, 0, SEEK_END);
  size_t f_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  vm_chunk_t *ptr = calloc(f_size/inst_size, sizeof(vm_chunk_t));

  printf("%d %d\n", inst_size, f_size);

  for(int chunk_num = 0; chunk_num < f_size/inst_size; chunk_num++)
  {
    char opcode, reg0, reg1, reg2;
    int imm;
    opcode = get_number(fp, VM_OPCODE_SIZE);
    reg0 = get_number(fp, VM_REG0_SIZE);
    reg1 = get_number(fp, VM_REG1_SIZE);
    reg2 = get_number(fp, VM_REG2_SIZE);
    imm = get_number(fp, VM_IMM_SIZE);

    VM_SET_OPCODE(ptr[chunk_num].code, opcode);
    VM_SET_REG0(ptr[chunk_num].code, reg0);
    VM_SET_REG1(ptr[chunk_num].code, reg1);
    VM_SET_REG2(ptr[chunk_num].code, reg2);
    VM_SET_IMM(ptr[chunk_num].code, imm);

    //printf("load_inst: %d %d %d %d %d\n", opcode,reg0,reg1,reg2,imm);
    //printf("%d %d\n", ftell(fp), feof(fp));
  }

  fclose(fp);

  vm_struct_t *vm = vm_init();

  vm->code = ptr;
  vm->instruction_count = f_size/inst_size;

  if(!ptr)
  {
    vm_clean(vm);
    printf("ERROR: VM creation failed.\n");
    return 1;
  }

  vm_execute(vm);

  vm_clean(vm);

  free(ptr);

  return 0;
}
