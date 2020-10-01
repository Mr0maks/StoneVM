#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

int main(int argc, char* argv[])
{
  if( argc < 2 )
  {
    printf( "ERROR: No file provided.\n" );
    exit( 1 );
  }

  FILE *fp;

  if( (fp=fopen(argv[1], "rb")) == NULL ) {
    printf( "ERROR: Cannot open file %s.\n", argv[1] );
    exit( 1 );
  }
  
  fseek( fp, 0, SEEK_END );
  size_t file_size = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  vm_bytecode_header_t header;

  if( fread( &header, sizeof(header), 1, fp ) != 1 )
  {
      printf( "ERROR: File size less then 16 byte\n" );
      fclose( fp );
      exit( 1 );
  }

  if( header.bytecode_magic != VM_BYTECODE_MAGIC )
  {
      printf( "ERROR: bytecode magic mismatch.\n" );
      fclose( fp );
      exit( 1 );
  }

  if( header.bytecode_version != VM_BYTECODE_VERSION )
  {
      printf( "ERROR: bytecode version mismatch.\n" );
      fclose( fp );
      exit( 1 );
  }

  if( header.bytecode_len == 0 )
  {
      printf( "ERROR: bytecode len is zero.\n" );
      fclose( fp );
      exit( 1 );
  }

  size_t size_need = ((header.bytecode_len * sizeof (uint8_t)) + sizeof(header));

  if( file_size != size_need )
  {
      printf( "ERROR: file size mismatch.\n" );
      fclose( fp );
      exit( 1 );
  }

  uint8_t *bytecode = malloc(header.bytecode_len);

  fread( bytecode, sizeof (*bytecode), header.bytecode_len, fp );

  fclose( fp );

  vm_struct_t *vm = vm_init();

  if( !vm )
  {
    printf( "ERROR: VM creation failed.\n" );
    return 1;
  }

  vm_load_bytecode(vm, bytecode, header.bytecode_len);
  vm_exec( vm );
  vm_free( vm );
  free(bytecode);
  return 0;
}
