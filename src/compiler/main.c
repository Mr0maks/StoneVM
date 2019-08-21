#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    printf("ERROR: No file provided.\n");
    exit(1);
  }

  FILE *f_in;
  FILE *f_out;

  if ((f_in=fopen(argv[1], "r")) == NULL) {
    printf("ERROR: Cannot open input file.\n");
    exit(1);
  }
  if ((f_out=fopen(argv[1], "wb")) == NULL) {
    printf("ERROR: Cannot open output file.\n");
    exit(1);
  }

  char operators[23][8] = {"halt", "nop", "loadi", "loadf", "mov", "cmp", 
                           "shr", "shl", "xor", "add", "sub", 
                           "mul", "div", "inc", "dec", "and", 
                           "or", "call", "ret", "jump", "loop", 
                           "jz", "jnz"};

  for (int i = 0; i < op_num; i++)
  {
    char op[8];
    unsigned int op_code;
    signed int r1, r2, r3;
    scanf("%s %x %x %x", &op, &r1, &r2, &r3);

    for (int j = 0; j < 23; j++)
    {
      if (strcmp(op, operators[j]) == 0)
      {
        op_code = j;
        break;
      }
    }


  }
}
