#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  unsigned int op_num = 0;
  scanf("%d", &op_num);
  vm_chunk_t *ptr = calloc(op_num, sizeof(vm_chunk_t));

  char operators[23][8] = {"halt", "nop", "loadi", "loadf", "mov", "cmp", 
                           "shr", "shl", "xor", "add", "sub", 
                           "mul", "div", "inc", "dec", "and", 
                           "or", "call", "ret", "jump", "loop", 
                           "jz", "jnz"};

  printf("* input start *\n");
  for (int i = 0; i < op_num; i++)
  {
    char op[8];
    int op_code;
    scanf("%s %x %x %x", &op, 
                         &ptr[i].registers[VM_R0].unsigned_interger,
                         &ptr[i].registers[VM_R1].unsigned_interger,
                         &ptr[i].registers[VM_R2].unsigned_interger);

    for (int j = 0; j < 23; j++)
    {
      if (strcmp(op, operators[j]) == 0)
      {
        op_code = j;
        break;
      }
    }

    //printf("vm: %d %X %X %X\n", op_code, ptr[i].registers[VM_R0].unsigned_interger, ptr[i].registers[VM_R1].unsigned_interger, ptr[i].registers[VM_R2].unsigned_interger);

    ptr[i].opcode = op_code;
  }

  printf("* input ended *\n");

  vm_struct_t *vm = vm_init();

  vm->code = ptr;
  vm->instruction_count = op_num;

  if(!ptr)
  {
    vm_clean(vm);
    return;
  }

  vm_execute(vm);

  vm_clean(vm);

  free(ptr);
}
