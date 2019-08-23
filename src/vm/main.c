#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

uint64_t get_instruction(FILE* fp)
{
  uint64_t result = 0;
  if (fread(&result, sizeof(result), 1, fp) != 1)
    {
      if (feof(fp))
	printf("ERROR: Premature end of file.\n");
      else
        printf("ERROR: File read error.\n");
      exit(1);
    }
  return result;
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
  
  fseek(fp, 0, SEEK_END);
  size_t f_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  int inst_size = VM_INSTRUCTION_SIZE_BYTES;

  vm_chunk_t *ptr = calloc(f_size/inst_size, sizeof(vm_chunk_t));

  printf("%d %ld\n", VM_INSTRUCTION_SIZE_BYTES, f_size);

  printf("start loading\n");
  for(int chunk_num = 0; chunk_num < f_size/inst_size; chunk_num++)
  {
    uint64_t code = 0;
    char opcode, reg0, reg1, reg2;
    int imm;

    code = get_instruction(fp);

    opcode = (char)VM_GET_OPCODE(code);
    reg0 = (char)VM_GET_REG0(code);
    reg1 = (char)VM_GET_REG1(code);
    reg2 = (char)VM_GET_REG2(code);
    imm = (int)VM_GET_IMM(code);

    ptr[chunk_num].code = code;

    printf("%d %d %d %d %d\n", opcode,reg0,reg1,reg2,imm);
    //printf("%d %d\n", ftell(fp), feof(fp));
  }
  printf("instructions loaded\n");

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
