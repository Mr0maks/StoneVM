#include <stdio.h>
#include "vm.h"

int get_number(FILE* fp)
{
    unsigned int num;
    if (fread(&num, 4, 1, fp) != 1) 
    {
      if (feof(fp))
        printf("ERROR: Premature end of file.");
      else 
        printf("ERROR: File read error.");
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
  
  fseek(fp, 0, SEEK_END);
  size_t f_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  vm_chunk_t *ptr = calloc(f_size/16, sizeof(vm_chunk_t));
  unsigned int num;
  unsigned int chunk_num;
  while (!feof(fp))
  {
    chunk_num = ftell(fp)/16;
    num = get_number(fp);
    ptr[chunk_num].opcode = num;
    
    for (unsigned char reg_num = VM_R0; reg_num <= VM_R2; reg_num++)
    {
      num = get_number(fp);
      ptr[chunk_num].registers[reg_num].unsigned_interger = num;
    }

    scanf("%d %x %x %x", &ptr[chunk_num].opcode, 
                         &ptr[chunk_num].registers[VM_R0].unsigned_interger,
                         &ptr[chunk_num].registers[VM_R1].unsigned_interger,
                         &ptr[chunk_num].registers[VM_R2].unsigned_interger);
  }

  fclose(fp);

  vm_struct_t *vm = vm_init();

  vm->code = ptr;
  vm->instruction_count = f_size/16;

  if(!ptr)
  {
    vm_clean(vm);
    printf("ERROR: VM creation failed.");
    return 1;
  }

  vm_execute(vm);

  vm_clean(vm);

  free(ptr);

  return 0;
}