#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

uint32_t get_memory_chunk(FILE* fp)
{
  uint32_t result = 0;
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

  vm_header_t header;

  if( fread(&header, sizeof(header), 1, fp) != 1 )
  {
      printf("ERROR: File size less then 16 byte\n");
      fclose(fp);
      exit(1);
  }

  if(header.magic != VM_MAGIC)
  {
      printf("ERROR: bytecode magic mismatch.\n");
      fclose(fp);
      exit(1);
  }

  size_t size_need = ((header.bytecode_len * VM_INSTRUCTION_SIZE_BYTES) + ( header.memory_len * sizeof(vm_memory_t) ) + sizeof(header));

  if( f_size != size_need )
  {
      printf("ERROR: file size mismatch.\n");
      fclose(fp);
      exit(1);
  }

  if(header.memory_len >= VM_MAX_MEMORY_SIZE)
  {
      printf("ERROR: memory too big.\n");
      fclose(fp);
      exit(1);
  }

  vm_chunk_t *ptr = calloc(header.bytecode_len, sizeof(vm_chunk_t));
  vm_memory_t *mem_ptr = calloc(header.memory_len, sizeof(vm_memory_t));

  printf("%d %ld\n", VM_INSTRUCTION_SIZE_BYTES, f_size);

  printf("start loading memory\n");
  for(uint32_t mem_chnk = 0; mem_chnk < header.memory_len; mem_chnk++)
  {
      uint32_t shit = get_memory_chunk(fp);
      printf("memory[%u] = 0x%x\n", mem_chnk, shit);
      mem_ptr[mem_chnk] = shit;
  }
  printf("memory loaded\n");

  printf("start loading instruction\n");
  for(uint32_t chunk_num = 0; chunk_num < header.bytecode_len; chunk_num++)
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
  }
  printf("instructions loaded\n");

  fclose(fp);

  vm_struct_t *vm = vm_init();

  if(!vm)
  {
    vm_clean(vm);
    printf("ERROR: VM creation failed.\n");
    free(ptr);
    free(mem_ptr);
    return 1;
  }

  vm->code = ptr;
  memcpy(&vm->memory, mem_ptr, header.memory_len  * sizeof(vm_memory_t));
  vm->instruction_count = header.bytecode_len;

  vm_execute(vm);
  vm_clean(vm);

  free(ptr);
  free(mem_ptr);

  return 0;
}
